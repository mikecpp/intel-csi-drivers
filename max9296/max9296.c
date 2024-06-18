#include <linux/acpi.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/regmap.h>
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

#define I2C_BUS_AVAILABLE               1
#define DEVICE_NAME_MAX9296             "i2c" 
#define DEVICE_ADDR_MAX9296             0x48
#define DEVICE_NAME_MAX9295             "max9295" 
#define DEVICE_ADDR_MAX9295             0x62
#define RES_128_64                      0 

struct max9296 {
	struct v4l2_subdev       sd;
	struct media_pad         pad;
    struct v4l2_ctrl_handler ctrl_handler;
    struct v4l2_device       v4l2_dev;
    struct media_device      media_dev;
    struct i2c_client        *client;         
    struct regmap            *regmap_max9296;
    struct regmap            *regmap_max9295;
	struct mutex             mutex;
	bool                     streaming;
};

static int max9296_write(struct max9296 *sensor, uint16_t reg, uint8_t val)
{
    int ret = 0;
    
    sensor->client->addr = DEVICE_ADDR_MAX9296;
	ret = regmap_write(sensor->regmap_max9296, reg, val);
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

    sensor->client->addr = DEVICE_ADDR_MAX9295;
	ret = regmap_write(sensor->regmap_max9296, reg, val);
	if (ret < 0) {
		pr_err("%s(): i2c write failed %d, 0x%04x = 0x%x\n", __func__, ret, reg, val);
        return -1;
    }   
    sensor->client->addr = DEVICE_ADDR_MAX9296;

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
    pr_info("--> max9296_set_stream()\n");

    return 0;
}

static int max9296_enum_mbus_code(struct v4l2_subdev *sd, struct v4l2_subdev_state *sd_state, struct v4l2_subdev_mbus_code_enum *code)
{
    pr_info("--> max9296_enum_mbus_code()\n");

    return 0;
}

static int max9296_get_pad_format(struct v4l2_subdev *sd,  struct v4l2_subdev_state *sd_state, struct v4l2_subdev_format *fmt)
{
    pr_info("--> max9296_get_pad_format()\n");

    return 0;
}

static int max9296_set_pad_format(struct v4l2_subdev *sd, struct v4l2_subdev_state *sd_state, struct v4l2_subdev_format *fmt)
{
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

static int max9296_init_controls(struct max9296 *sensor)
{
    int ret = 0;

	ret = v4l2_ctrl_handler_init(&sensor->ctrl_handler, 10);
	if (ret != 0) {
        return ret;
    }

	return 0;
}

static const struct regmap_config max9296_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.reg_format_endian = REGMAP_ENDIAN_NATIVE,
	.val_format_endian = REGMAP_ENDIAN_NATIVE,
};

static const struct regmap_config max9295_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.reg_format_endian = REGMAP_ENDIAN_NATIVE,
	.val_format_endian = REGMAP_ENDIAN_NATIVE,
};

struct v4l2_device _v4l2_dev;

static int max9296_probe(struct i2c_client *client)
{
    int ret = 0;
    struct max9296 *sensor;
    // dev_t  *dev_num = &client->dev.devt;

    // _client = client;
    pr_info("max9296_probe() OK!\n");    
	sensor = devm_kzalloc(&client->dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return -ENOMEM;

    mutex_init(&sensor->mutex);

    sensor->client = client;
    v4l2_set_subdevdata(&sensor->sd, sensor);
    
	sensor->regmap_max9296 = devm_regmap_init_i2c(sensor->client, &max9296_regmap_config);
	if (IS_ERR(sensor->regmap_max9296)) {
		pr_err("regmap_max9296 init failed! \n");
		return -1;
	}

    // Initial max9296/max9295 registers 
    registers_init(sensor);

    // Initial subdev
    v4l2_i2c_subdev_init(&sensor->sd, sensor->client, &max9296_subdev_ops);

    // sensor->sd.owner           = NULL;
	sensor->sd.internal_ops    = &max9296_internal_ops;
    // sensor->sd.grp_id          = *dev_num;
	sensor->sd.flags          |= V4L2_SUBDEV_FL_HAS_DEVNODE;
    sensor->sd.entity.ops      = &max9296_subdev_entity_ops;
    // sensor->sd.entity.obj_type = MEDIA_ENTITY_TYPE_V4L2_SUBDEV;
	sensor->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;

	sensor->pad.flags          = MEDIA_PAD_FL_SOURCE;

    ret = media_entity_pads_init(&sensor->sd.entity, 1, &sensor->pad); 
    if (ret < 0) {
        pr_err("media_entity_pads_init() fail! (%d)", ret);        
        return ret;
    }

    max9296_init_controls(sensor);

    // V4L2 Media Device & V4L2 Subdevice Initial 
/*    
    sensor->media_dev.dev  = &client->dev;
    sensor->media_dev.ops  = NULL; 
    strscpy(sensor->media_dev.model, "maxim", 6);  
    
    media_device_init(&sensor->media_dev);
	ret = media_device_register(&sensor->media_dev);    
    if (ret < 0) {
        pr_err("media_device_register() fail! (%d)", ret);        
        return ret;
    }
    sensor->v4l2_dev.mdev = &sensor->media_dev;

    ret = v4l2_device_register(&sensor->client->dev, &sensor->v4l2_dev);
    if (ret < 0) {
        pr_err("v4l2_device_register()) fail! (%d)", ret);        
        return ret;
    }
    pr_info("v4l2 device name = %s\n", sensor->v4l2_dev.name);

    // V4L2 Sub Device initial 
    ret = v4l2_device_register_subdev(&sensor->v4l2_dev, &sensor->sd);
    if (ret < 0) {
        pr_err("v4l2_device_register_subdev() fail! (%d)", ret);        
        return ret;
    }
    pr_info("v4l2_device_register_subdev() OK!\n");

    ret = v4l2_device_register_subdev_nodes(&sensor->v4l2_dev);
	if (ret < 0) {
        pr_err("v4l2_device_register_subdev_nodes() fail! (%d)", ret);
		return -1;
    }
*/

    ret = v4l2_async_register_subdev_sensor(&sensor->sd);
    if (ret != 0) {
        pr_err("v4l2_async_register_subdev_sensor() fail! (%d)", ret); 
        return ret;
    }
    pr_info("v4l2_async_register_subdev_sensor() OK! \n");

	return 0; 
}

static void max9296_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
    // struct max9296 *sensor = v4l2_get_subdevdata(sd);

    v4l2_async_unregister_subdev(sd);
	// v4l2_device_unregister_subdev(sd);
	media_entity_cleanup(&sd->entity);
    /*
    media_devnode_unregister(sensor->media_dev.devnode);
    media_device_unregister(&sensor->media_dev);    
    media_device_cleanup(&sensor->media_dev);    
    */
    pr_info("max9296_remove() OK!\n");    
}

static const struct acpi_device_id max9296_acpi_ids[] = {
	{"MAX9296"},
	{ }
};

MODULE_DEVICE_TABLE(acpi, max9296_acpi_ids);

static struct i2c_driver max9296_driver = {
	.driver = {
		.name  = DEVICE_NAME_MAX9296,
		.acpi_match_table = ACPI_PTR(max9296_acpi_ids),        
	},
	.probe_new = max9296_probe,
	.remove    = max9296_remove,
	.flags     = I2C_DRV_ACPI_WAIVE_D0_PROBE,    
};

module_i2c_driver(max9296_driver); 

MODULE_AUTHOR("Mike Chen <mikechen@otobrite.com>");
MODULE_DESCRIPTION("oToCAM MAx9296 Driver");
MODULE_LICENSE("GPL v2");
