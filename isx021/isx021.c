#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/media.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/videodev2.h>
#include <linux/version.h>

#include <linux/ipu-isys.h>
#include <media/media-entity.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-mediabus.h>

struct isx021 {
	struct v4l2_subdev sd;
	struct media_pad pad;

	struct v4l2_ctrl_handler ctrl_handler;

	struct clk *img_clk;
	struct regulator *avdd;
	struct gpio_desc *reset;

	struct v4l2_ctrl *link_freq;
	struct v4l2_ctrl *pixel_rate;
	struct v4l2_ctrl *vblank;
	struct v4l2_ctrl *hblank;
	struct v4l2_ctrl *exposure;

	struct mutex mutex;
	bool streaming;

	bool identified;
};

#define to_isx021(_sd)	container_of(_sd, struct isx021, sd)

static struct i2c_board_info max9296_info = {
      I2C_BOARD_INFO("max9296", 0x48),
};

struct i2c_client *client = NULL;

static int max9296_write(u8 *data, u16 len)
{
    pr_info("=== max9296_write() enter ===");
/*
    if (i2c_master_send(client, data, len) < len) {
        pr_err("i2c_master_send() fail!");
        return -1;
    }
*/
    pr_info("=== max9296_write() exit ===");

    return 0;
}

static int isx021_probe(struct i2c_client *client)
{
	struct isx021 *isx021 = NULL;
    int ret = 0;

	isx021 = devm_kzalloc(&client->dev, sizeof(*isx021), GFP_KERNEL);
	if (!isx021)
		return -ENOMEM;

    // v4l2_i2c_subdev_init(&ov13b->sd, client, &isx021_subdev_ops);

    ret = media_entity_pads_init(&isx021->sd.entity, 1, &isx021->pad);
	if (ret) {
		dev_err(&client->dev, "%s failed:%d\n", __func__, ret);
        return -1;
	}

	ret = v4l2_async_register_subdev_sensor(&isx021->sd);
	if (ret < 0) {
		dev_err(&client->dev, "%s failed:%d\n", __func__, ret);
        return -1;
    }

	return 0; 
}

static void isx021_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

	v4l2_async_unregister_subdev(sd);
	media_entity_cleanup(&sd->entity);
}

static const unsigned short normal_i2c[] = { 0x48, 0x4A, I2C_CLIENT_END };

static int isx021_init(void)
{
    u8 data[] = { 0x00, 0x00, 0x00 };    
    // struct i2c_client *client = i2c_new_client_device(i2c_get_adapter(1), &max9296_info);
    struct i2c_client *client = i2c_new_scanned_device(i2c_get_adapter(1), &max9296_info, normal_i2c, NULL);

    if (client == NULL) {
        pr_err("i2c_new_scanned_device() fail!");
        return -1;
    }

    pr_info("=== isz021_init ===");

    max9296_write(data, sizeof(data));

    return 0;
}

static void isx021_exit(void)
{

}

static struct i2c_driver isx021_i2c_driver = {
	.driver = {
		.name = "isx021",
	},
	.probe_new = isx021_probe,
	.remove    = isx021_remove,
};
// module_i2c_driver(isx021_i2c_driver);

module_init(isx021_init);
module_exit(isx021_exit);

MODULE_DESCRIPTION("oToCAM ISX021 Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0.0");
