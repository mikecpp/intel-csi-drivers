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

#define I2C_BUS_AVAILABLE               1
#define DEVICE_NAME_MAX9296             "max9296" 
#define DEVICE_ADDR_MAX9296             0x48
#define DEVICE_NAME_MAX9295             "max9295" 
#define DEVICE_ADDR_MAX9295             0x62
#define RES_128_64                      0 

static struct i2c_board_info max9296_info = {
    I2C_BOARD_INFO(DEVICE_NAME_MAX9296, DEVICE_ADDR_MAX9296)    
};

static const struct i2c_device_id max9296_id[] = {
  { DEVICE_NAME_MAX9296, 0 },
  { }
};

MODULE_DEVICE_TABLE(i2c, max9296_id);

static struct i2c_board_info max9295_info = {
    I2C_BOARD_INFO(DEVICE_NAME_MAX9295, DEVICE_ADDR_MAX9295)    
};

static const struct i2c_device_id max9295_id[] = {
  { DEVICE_NAME_MAX9295, 0 },
  { }
};

MODULE_DEVICE_TABLE(i2c, max9295_id);

static struct i2c_adapter *_adapter  = NULL; 
static struct i2c_client  *_max9296  = NULL; 
static struct i2c_client  *_max9295  = NULL; 

static int max9296_write(uint16_t reg, uint8_t val)
{
    int ret = 0;
    uint8_t data[3] = {0};    

    data[0] = (reg & 0xFF00) >> 8; 
    data[1] = (reg & 0x00FF); 
    data[2] = val;

    pr_info("max9296_write: %02X %02X %02X\n", data[0], data[1], data[2]); 

    ret = i2c_master_send(_max9296, data, 3);
    if ( ret < 0) {
        pr_err("max9296_write: i2c_master_send() fail! (%d) \n", ret);
        return ret;
    }

    return 0;
}

static int max9295_write(uint16_t reg, uint8_t val)
{
    int ret = 0;
    uint8_t data[3] = {0};    

    data[0] = (reg & 0xFF00) >> 8; 
    data[1] = (reg & 0x00FF); 
    data[2] = val;

    pr_info("max9295_write: %02X %02X %02X\n", data[0], data[1], data[2]); 

    ret = i2c_master_send(_max9295, data, 3);
    if ( ret < 0) {
        pr_err("max9295_write: i2c_master_send() fail! (%d) \n", ret);
        return ret;
    }

    return 0;
}

static void register_init(void)
{
    max9296_write(0x0010, 0x31);
    msleep(100);

    max9295_write(0x0010, 0x21);
    msleep(100);

    // max9295
    max9295_write(0x0330, 0x00);
    max9295_write(0x0332, 0xEE);
    max9295_write(0x0333, 0xE4);
    max9295_write(0x0331, 0x33);
    max9295_write(0x0311, 0xF0);
    max9295_write(0x0308, 0x6F);
    max9295_write(0x0314, 0x5E);
    max9295_write(0x0316, 0x75);
    max9295_write(0x0318, 0x52);
    max9295_write(0x031A, 0x22);
    max9295_write(0x0002, 0x73);

    // max9296
    max9296_write(0x0330, 0x04);
    max9296_write(0x0333, 0x4E);
    max9296_write(0x0334, 0xE4);
    max9296_write(0x040A, 0x00);
    max9296_write(0x044A, 0xD0);
    max9296_write(0x048A, 0x00);
    max9296_write(0x04CA, 0x00);

    max9296_write(0x031D, 0x00);
    max9296_write(0x0320, 0x2A);
    max9296_write(0x0323, 0x00);
    max9296_write(0x0326, 0x00);
    max9296_write(0x0050, 0x00);
    max9296_write(0x0051, 0x01);
    max9296_write(0x0052, 0x02);
    max9296_write(0x0053, 0x03);
    max9296_write(0x0332, 0xF0);

    max9296_write(0x040B, 0x07);
    max9296_write(0x042D, 0x15);
    max9296_write(0x040D, 0x1E);
    max9296_write(0x040E, 0x1E);
    max9296_write(0x040F, 0x00);
    max9296_write(0x0410, 0x00);
    max9296_write(0x0411, 0x01);
    max9296_write(0x0412, 0x01);

    max9296_write(0x044B, 0x01);
    max9296_write(0x046D, 0x01);
    max9296_write(0x044D, 0x35);
    max9296_write(0x044E, 0x35);
    max9296_write(0x048B, 0x01);
    max9296_write(0x04AD, 0x01);
    max9296_write(0x048D, 0x12);
    max9296_write(0x048E, 0x12);

    max9295_write(0x02BF, 0x60);
    max9295_write(0x02BE, 0x90);
    max9295_write(0x02D4, 0x60);
    max9295_write(0x02D3, 0x90);
}

static int max9296_probe(struct i2c_client *client)
{
    pr_info("max9296_probe() OK!\n");    

	return 0; 
}

static void max9296_remove(struct i2c_client *client)
{
    pr_info("max9296_remove() OK!\n");    
}

static int max9295_probe(struct i2c_client *client)
{
    pr_info("max9295_probe() OK!\n");    

	return 0; 
}

static void max9295_remove(struct i2c_client *client)
{
    pr_info("max9295_remove() OK!\n");    
}

static struct i2c_driver max9296_driver = {
	.driver = {
		.name  = DEVICE_NAME_MAX9296,
        .owner = THIS_MODULE,        
	},
	.probe_new = max9296_probe,
	.remove    = max9296_remove,
    .id_table  = max9296_id,    
};

static struct i2c_driver max9295_driver = {
	.driver = {
		.name  = DEVICE_NAME_MAX9295,
        .owner = THIS_MODULE,        
	},
	.probe_new = max9295_probe,
	.remove    = max9295_remove,
    .id_table  = max9295_id,    
};

static int max9296_init(void)
{
    _adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (_adapter == NULL) {
        pr_err("i2c_get_adaptor() fail! \n"); 
        return -1;
    }

    _max9296 = i2c_new_client_device(_adapter, &max9296_info);
    if (_max9296 == NULL) {
        pr_err("i2c_new_client_device() fail! \n"); 
        return -1;
    }

    _max9295 = i2c_new_client_device(_adapter, &max9295_info);
    if (_max9295 == NULL) {
        pr_err("i2c_new_client_device() fail! \n"); 
        return -1;
    }

    i2c_add_driver(&max9296_driver);
    i2c_add_driver(&max9295_driver);
    i2c_put_adapter(_adapter);

    register_init();

    pr_info("max9296_init() OK!\n");

    return 0;
}

static void max9296_exit(void)
{
    i2c_unregister_device(_max9295);
    i2c_unregister_device(_max9296);

    i2c_del_driver(&max9295_driver);
    i2c_del_driver(&max9296_driver);

    pr_info("max9296_exit() OK!\n");
}   

module_init(max9296_init);
module_exit(max9296_exit);

MODULE_DESCRIPTION("oToCAM MAx9296 Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0.0");
