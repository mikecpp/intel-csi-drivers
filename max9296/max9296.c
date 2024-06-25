#include <linux/acpi.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-async.h>

#define DEVICE_NAME_MAX9296             "i2c:max9296" 
#define DEVICE_ADDR_MAX9296             0x48
#define DEVICE_ADDR_MAX9295             0x62

struct max9296_mode {
	u32 width;
	u32 height;
};

static struct max9296_mode supported_modes[] = {
	{
		.width   = 1920,
		.height  = 1200,
	},
};

struct max9296 {
	struct v4l2_subdev       sd;
	struct media_pad         pad;
    struct v4l2_ctrl_handler ctrl_handler;
    struct v4l2_device       v4l2_dev;
    struct media_device      media_dev;
    struct i2c_client        *client;         

    // V4L2 Controls
	struct v4l2_ctrl        *link_freq;
	struct v4l2_ctrl        *pixel_rate;

    // Current Mode 
    struct max9296_mode      *cur_mode;

    // Mutex 
	struct mutex             mutex;

	bool streaming;    
};

#define to_max9296(_sd)	container_of(_sd, struct max9296, sd)

static int max9296_write(struct max9296 *sensor, uint16_t reg, uint8_t val)
{
    int ret = 0;
    struct i2c_client *client = sensor->client;
    uint8_t data[3] = {0};
    
    data[0] = (reg & 0xFF00) >> 8; 
    data[1] = (reg & 0x00FF); 
    data[2] = val;

    client->addr = DEVICE_ADDR_MAX9296;
    ret = i2c_master_send(client, data, 3);
	if (ret < 0) {
		pr_err("%s(): i2c write failed %d, 0x%04x = 0x%x\n", __func__, ret, reg, val);
        return -1;
    }   

    // pr_info("max9296_write: %04x %02x\n", reg, val); 

    return 0;
}

static int max9295_write(struct max9296 *sensor, uint16_t reg, uint8_t val)
{
    int ret = 0;
    struct i2c_client *client = sensor->client;
    uint8_t data[3] = {0};
    
    data[0] = (reg & 0xFF00) >> 8; 
    data[1] = (reg & 0x00FF); 
    data[2] = val;

    client->addr = DEVICE_ADDR_MAX9295;
    ret = i2c_master_send(client, data, 3);
	if (ret < 0) {
		pr_err("%s(): i2c write failed %d, 0x%04x = 0x%x\n", __func__, ret, reg, val);
        return -1;
    }   
    client->addr = DEVICE_ADDR_MAX9296;
    // pr_info("max9295_write: %04x %02x\n", reg, val); 

    return 0;
}

static void registers_init(struct max9296 *sensor)
{
    max9296_write(sensor, 0x0010, 0x31);
    msleep(100);

    max9295_write(sensor, 0x0010, 0x21);
    msleep(100);

    // max9295
    max9295_write(sensor, 0x0330, 0x00);
    max9295_write(sensor, 0x0332, 0xEE);
    max9295_write(sensor, 0x0333, 0xE4);
    max9295_write(sensor, 0x0331, 0x33);
    max9295_write(sensor, 0x0311, 0xF0);
    max9295_write(sensor, 0x0308, 0x6F);
    max9295_write(sensor, 0x0314, 0x5E);
    max9295_write(sensor, 0x0316, 0x75);
    max9295_write(sensor, 0x0318, 0x52);
    max9295_write(sensor, 0x031A, 0x22);
    max9295_write(sensor, 0x0002, 0x73);

    // max9296
    max9296_write(sensor, 0x0330, 0x04);
    max9296_write(sensor, 0x0333, 0x4E);
    max9296_write(sensor, 0x0334, 0xE4);
    max9296_write(sensor, 0x040A, 0x00);
    max9296_write(sensor, 0x044A, 0xD0);
    max9296_write(sensor, 0x048A, 0x00);
    max9296_write(sensor, 0x04CA, 0x00);

    max9296_write(sensor, 0x031D, 0x00);
    max9296_write(sensor, 0x0320, 0x2A);
    max9296_write(sensor, 0x0323, 0x00);
    max9296_write(sensor, 0x0326, 0x00);
    max9296_write(sensor, 0x0050, 0x00);
    max9296_write(sensor, 0x0051, 0x01);
    max9296_write(sensor, 0x0052, 0x02);
    max9296_write(sensor, 0x0053, 0x03);
    max9296_write(sensor, 0x0332, 0xF0);

    max9296_write(sensor, 0x040B, 0x07);
    max9296_write(sensor, 0x042D, 0x15);
    max9296_write(sensor, 0x040D, 0x1E);
    max9296_write(sensor, 0x040E, 0x1E);
    max9296_write(sensor, 0x040F, 0x00);
    max9296_write(sensor, 0x0410, 0x00);
    max9296_write(sensor, 0x0411, 0x01);
    max9296_write(sensor, 0x0412, 0x01);

    max9296_write(sensor, 0x044B, 0x01);
    max9296_write(sensor, 0x046D, 0x01);
    max9296_write(sensor, 0x044D, 0x35);
    max9296_write(sensor, 0x044E, 0x35);
    max9296_write(sensor, 0x048B, 0x01);
    max9296_write(sensor, 0x04AD, 0x01);
    max9296_write(sensor, 0x048D, 0x12);
    max9296_write(sensor, 0x048E, 0x12);

    max9295_write(sensor, 0x02BF, 0x60);
    max9295_write(sensor, 0x02BE, 0x90);
    max9295_write(sensor, 0x02D4, 0x60);
    max9295_write(sensor, 0x02D3, 0x90);
}

static int max9296_set_stream(struct v4l2_subdev *sd, int enable)
{
    // int ret;
    struct max9296 *sensor = to_max9296(sd);

    mutex_lock(&sensor->mutex);
    if (enable) {
        pr_info("--> max9296_set_stream() start!\n");    
        /*
	    ret =  __v4l2_ctrl_handler_setup(sensor->sd.ctrl_handler);
	    if (ret != 0)
            pr_err("__v4l2_ctrl_handler_setup() fail (%d)\n", ret);
		    return ret;
        
        */
    }
    else {
        pr_info("--> max9296_set_stream() stop!\n");    
    }

    mutex_unlock(&sensor->mutex);

    return 0;
}

static int max9296_enum_mbus_code(struct v4l2_subdev *sd, struct v4l2_subdev_state *sd_state, struct v4l2_subdev_mbus_code_enum *code)
{
    pr_info("--> max9296_enum_mbus_code()\n");

    return 0;
}

static void max9296_update_pad_format(const struct max9296_mode *mode,  struct v4l2_subdev_format *fmt)
{
	fmt->format.width  = mode->width;
	fmt->format.height = mode->height;
	fmt->format.code   = MEDIA_BUS_FMT_UYVY8_1X16;
	fmt->format.field  = V4L2_FIELD_NONE;
}

static int max9296_get_pad_format(struct v4l2_subdev *sd,  struct v4l2_subdev_state *sd_state, struct v4l2_subdev_format *fmt)
{
    struct max9296 *sensor = to_max9296(sd);
	struct v4l2_mbus_framefmt *framefmt;

    mutex_lock(&sensor->mutex);    

	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
		framefmt    = v4l2_subdev_get_try_format(sd, sd_state, fmt->pad);
		fmt->format = *framefmt;
	} else {
		max9296_update_pad_format(sensor->cur_mode, fmt);
	}

    mutex_unlock(&sensor->mutex);

    pr_info("--> max9296_get_pad_format()\n");

    return 0;
}

static int max9296_set_pad_format(struct v4l2_subdev *sd, struct v4l2_subdev_state *sd_state, struct v4l2_subdev_format *fmt)
{
    struct max9296 *sensor = to_max9296(sd);

    mutex_lock(&sensor->mutex);    

    mutex_unlock(&sensor->mutex);

    pr_info("--> max9296_set_pad_format()\n");

    return 0;
}

static int max9296_enum_frame_size(struct v4l2_subdev *sd, struct v4l2_subdev_state *sd_state, struct v4l2_subdev_frame_size_enum *fse)
{
    pr_info("--> max9296_enum_frame_size()\n");

    return 0;
}

static int max9296_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
    struct max9296 *sensor = to_max9296(sd);
    struct v4l2_mbus_framefmt *try_fmt = v4l2_subdev_get_try_format(sd, fh->state, 0);    

    mutex_lock(&sensor->mutex);

	try_fmt->width  = 1920;
	try_fmt->height = 1200;
	try_fmt->code   = MEDIA_BUS_FMT_UYVY8_1X16;
	try_fmt->field  = V4L2_FIELD_NONE;

    mutex_unlock(&sensor->mutex);

    pr_info("--> max9296_open()\n");

    return 0;
}

static const struct v4l2_subdev_video_ops max9296_video_ops = {
	.s_stream = max9296_set_stream,
};

static const struct v4l2_subdev_pad_ops max9296_pad_ops = {
	.enum_mbus_code  = max9296_enum_mbus_code,
	.get_fmt         = max9296_get_pad_format,
	.set_fmt         = max9296_set_pad_format,
	.enum_frame_size = max9296_enum_frame_size,
};

static const struct v4l2_subdev_ops max9296_subdev_ops = {
	.video = &max9296_video_ops,
	.pad   = &max9296_pad_ops,
};

static const struct media_entity_operations max9296_subdev_entity_ops = {
	.link_validate = v4l2_subdev_link_validate,
};

static const struct v4l2_subdev_internal_ops max9296_internal_ops = {
	.open = max9296_open,
};

static int max9296_set_ctrl(struct v4l2_ctrl *ctrl)
{
    pr_info("--> max9296_set_ctrl()\n");    

    return 0;
}

static const struct v4l2_ctrl_ops max9296_ctrl_ops = {
	.s_ctrl = max9296_set_ctrl,
};

#define MAX9296_LINK_FREQ   560000000ULL

static const s64 link_freq_menu_items[] = {
	MAX9296_LINK_FREQ,
};

static int max9296_init_controls(struct max9296 *sensor)
{
    int ret = 0;
	struct v4l2_ctrl_handler *ctrl_hdlr = &sensor->ctrl_handler;
    s64 max;
    s64 pixel_rate_max;
    s64 pixel_rate_min;

    max = MAX9296_LINK_FREQ;

	ret = v4l2_ctrl_handler_init(ctrl_hdlr, 10);
	if (ret != 0) {
        return ret;
    }

    mutex_init(&sensor->mutex);
    ctrl_hdlr->lock = &sensor->mutex;

    // V4L2_CID_LINK_FREQ
	sensor->link_freq = v4l2_ctrl_new_int_menu(ctrl_hdlr, &max9296_ctrl_ops, V4L2_CID_LINK_FREQ, 
                                               max, 0, link_freq_menu_items);
	if (sensor->link_freq)
		sensor->link_freq->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	pixel_rate_max = 1000;
	pixel_rate_min = 0;

    // V4L2_CID_PIXEL_RATE 
	sensor->pixel_rate = v4l2_ctrl_new_std(ctrl_hdlr, &max9296_ctrl_ops, V4L2_CID_PIXEL_RATE, 
                                           pixel_rate_min, pixel_rate_max, 1, pixel_rate_max);

    sensor->sd.ctrl_handler = ctrl_hdlr;

	return 0;
}

struct v4l2_device _v4l2_dev;

static int max9296_probe(struct i2c_client *client)
{
    int ret = 0;
    struct max9296 *sensor;
    dev_t  *dev_num = &client->dev.devt;

    // _client = client;
	sensor = devm_kzalloc(&client->dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return -ENOMEM;

    sensor->client = client;
    v4l2_set_subdevdata(&sensor->sd, sensor);

    // Initial max9296/max9295 registers 
    registers_init(sensor);

    // Initial subdev
    v4l2_i2c_subdev_init(&sensor->sd, sensor->client, &max9296_subdev_ops);

    sensor->sd.owner           = NULL;
	sensor->sd.internal_ops    = &max9296_internal_ops;
    sensor->sd.grp_id          = *dev_num;
	sensor->sd.flags          |= V4L2_SUBDEV_FL_HAS_DEVNODE;
    sensor->sd.entity.ops      = &max9296_subdev_entity_ops;
    sensor->sd.entity.obj_type = MEDIA_ENTITY_TYPE_V4L2_SUBDEV;
	sensor->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;

	sensor->pad.flags          = MEDIA_PAD_FL_SOURCE;

    ret = media_entity_pads_init(&sensor->sd.entity, 1, &sensor->pad); 
    if (ret < 0) {
        pr_err("media_entity_pads_init() fail! (%d)", ret);        
        return ret;
    }

    sensor->cur_mode = &supported_modes[0];

    max9296_init_controls(sensor);

    ret = v4l2_async_register_subdev_sensor(&sensor->sd);
    if (ret < 0) {
        pr_err("v4l2_async_register_subdev_sensor() fail! (%d)", ret); 
        return ret;
    }

    pr_info("max9296_probe() OK!\n");    

	return 0; 
}

static void max9296_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

    v4l2_async_unregister_subdev(sd);
	media_entity_cleanup(&sd->entity);
    pr_info("max9296_remove() OK!\n");    
}

static const struct i2c_device_id max9296_id[] = {
	{DEVICE_NAME_MAX9296, 0},
	{ }
};

static struct i2c_driver max9296_driver = {
	.driver = {
		.name  = DEVICE_NAME_MAX9296,
        .owner = THIS_MODULE,
	},
	.probe_new = max9296_probe,
	.remove    = max9296_remove,
    .id_table  = max9296_id,    
};

module_i2c_driver(max9296_driver); 

MODULE_AUTHOR("Mike Chen <mikechen@otobrite.com>");
MODULE_DESCRIPTION("oToCAM MAx9296 Driver");
MODULE_LICENSE("GPL v2");
