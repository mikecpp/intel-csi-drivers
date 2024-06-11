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
#define DEVICE_NAME                     "max9296" 
#define DEVICE_ADDR                     0x48
#define RES_128_64                      0 

static struct i2c_board_info max9296_info = {
    I2C_BOARD_INFO(DEVICE_NAME, DEVICE_ADDR)    
};

static const struct i2c_device_id max9296_id[] = {
  { DEVICE_NAME, 0 },
  { }
};

MODULE_DEVICE_TABLE(i2c, max9296_id);

static struct i2c_adapter *_adapter = NULL; 
static struct i2c_client  *_client  = NULL; 

static uint8_t max9296_read(uint16_t reg, uint8_t val)
{
    return 0;
}

static int max9296_write(uint16_t reg, uint8_t val)
{
    uint8_t data[3] = {0};    

    data[0] = (reg & 0xFF00) >> 8; 
    data[1] = (reg & 0x00FF); 
    data[2] = val;

    pr_info("max9296_write: %02X %02X %02X\n", data[0], data[1], data[2]); 

    if (i2c_master_send(_client, data, 3) < 0) {
        pr_err("i2c_master_send() fail!\n");
        return -1;
    }

    return 0;
}

static int max9296_probe(struct i2c_client *client)
{
    max9296_write(0x0010, 0x31);

    pr_info("max9296_probe() OK!\n");    

	return 0; 
}

static void max9296_remove(struct i2c_client *client)
{
    pr_info("max9296_remove() OK!\n");    
}

static struct i2c_driver max9296_driver = {
	.driver = {
		.name  = DEVICE_NAME,
        .owner = THIS_MODULE,        
	},
	.probe_new = max9296_probe,
	.remove    = max9296_remove,
    .id_table  = max9296_id,    
};

static int max9296_init(void)
{
    _adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);
    if (_adapter == NULL) {
        pr_err("i2c_get_adaptor() fail! \n"); 
        return -1;
    }

    _client = i2c_new_client_device(_adapter, &max9296_info);
    if (_client == NULL) {
        pr_err("i2c_new_client_device() fail! \n"); 
        return -1;
    }

    i2c_add_driver(&max9296_driver);
    i2c_put_adapter(_adapter);

    pr_info("max9296_init() OK!\n");

    return 0;
}

static void max9296_exit(void)
{
    i2c_unregister_device(_client);
    i2c_del_driver(&max9296_driver);

    pr_info("ssd1306_exit() OK!\n");
}   

module_init(max9296_init);
module_exit(max9296_exit);

MODULE_DESCRIPTION("oToCAM MAx9296 Driver");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0.0");
