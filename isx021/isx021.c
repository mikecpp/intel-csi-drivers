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

#define MAX9295_REG0	0x0000
#define MAX9295_I2C_1	0x004A

#define MAX9296_CTRL0	0x0010
#define RESET_LINK	    (0x1 << 6)
#define RESET_ONESHOT	(0x1 << 5)
#define AUTO_LINK	    (0x1 << 4)
#define DUAL_LINK	    (0x0)
#define LINK_A		    (0x1)
#define LINK_B		    (0x2)
#define SPLITTER	    (0x3)
#define MAX9296_NUM	    (4)

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

static const struct regmap_config isx021_regmap_max9296 = {
	.reg_bits = 16,
	.val_bits = 8,
	.reg_format_endian = REGMAP_ENDIAN_BIG,
	.val_format_endian = REGMAP_ENDIAN_NATIVE,
};

static const struct regmap_config isx021_regmap_max9295 = {
	.reg_bits = 16,
	.val_bits = 8,
	.reg_format_endian = REGMAP_ENDIAN_BIG,
	.val_format_endian = REGMAP_ENDIAN_NATIVE,
};

#define to_isx021(_sd)	container_of(_sd, struct isx021, sd)

static int max9296_write_8(struct ds5 *state, u16 reg, u8 val)
{
	int ret;

	ret = regmap_raw_write(isx021_regmap_max9296, reg, &val, 1);
	if (ret < 0)
		dev_err(&state->client->dev, "%s(): i2c write failed %d, 0x%04x = 0x%x\n",
			__func__, ret, reg, val);
	else
		if (state->dfu_dev.dfu_state_flag == DS5_DFU_IDLE)
			dev_dbg(&state->client->dev, "%s(): i2c write 0x%04x: 0x%x\n",
				 __func__, reg, val);

	dev_dbg(&state->client->dev, "%s(): (%d), 0x%02x 0x%04x = 0x%02x\n",
		__func__, ret, state->client->addr, reg, val);

	return ret;
}

static int max9296_read_8(struct ds5 *state, u16 reg, u8 *val)
{
	int ret;

	ret = regmap_raw_read(isx_remap_max9296, reg, val, 1);
	if (ret < 0)
		dev_err(&state->client->dev, "%s(): i2c read failed %d, 0x%04x\n",
			__func__, ret, reg);
	else
		if (state->dfu_dev.dfu_state_flag == DS5_DFU_IDLE)
			dev_info(&state->client->dev, "%s(): i2c read 0x%04x = 0x%x\n",
				 __func__, reg, *val);

	dev_dbg(&state->client->dev, "%s(): (%d), 0x%02x 0x%04x = 0x%02x\n",
		__func__, ret, state->client->addr, reg, *val);

	return ret;
}

static int isx021_probe(struct i2c_client *client)
{
	struct isx021 *isx021 = NULL;
    int ret = 0;

	isx021 = devm_kzalloc(&client->dev, sizeof(*isx021), GFP_KERNEL);
	if (!isx021)
		return -ENOMEM;

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

static struct i2c_driver isx021_i2c_driver = {
	.driver = {
		.name = "isx021",
	},
	.probe_new = isx021_probe,
	.remove    = isx021_remove,
};
module_i2c_driver(isx021_i2c_driver);

MODULE_DESCRIPTION("oToCAM ISX021 Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0.0");
