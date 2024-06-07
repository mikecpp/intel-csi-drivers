#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x80df923a, "i2c_del_driver" },
	{ 0xf1d249f1, "devm_kmalloc" },
	{ 0xb62e94ec, "v4l2_i2c_subdev_init" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xe8dd7907, "v4l2_ctrl_handler_init_class" },
	{ 0xb38bf574, "v4l2_ctrl_new_int_menu" },
	{ 0x3d9830c0, "v4l2_ctrl_new_std" },
	{ 0x9e7f9667, "v4l2_ctrl_new_std_menu" },
	{ 0x91e27037, "v4l2_ctrl_new_custom" },
	{ 0x43ee2256, "media_entity_pads_init" },
	{ 0x9d20718d, "devm_gpio_request" },
	{ 0x34d23939, "gpio_to_desc" },
	{ 0x4cf4b29e, "gpiod_direction_input" },
	{ 0xc553e6ae, "gpiod_to_irq" },
	{ 0xa0cdb302, "devm_request_threaded_irq" },
	{ 0xee477acd, "devm_gpio_request_one" },
	{ 0x95873975, "v4l2_async_register_subdev_sensor" },
	{ 0x43d8d5fe, "__pm_runtime_set_status" },
	{ 0x3e51cf5b, "pm_runtime_enable" },
	{ 0x6841cdf4, "__pm_runtime_idle" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x60bd1997, "gpiod_set_raw_value" },
	{ 0xb755d7cd, "gpiod_get_raw_value" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0xb8cc7183, "__v4l2_ctrl_handler_setup" },
	{ 0x69ee9101, "__pm_runtime_resume" },
	{ 0x43763647, "pm_runtime_get_if_active" },
	{ 0xd182d1b9, "_dev_info" },
	{ 0xce14a3c3, "v4l2_subdev_link_validate" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xb55e8ce, "i2c_register_driver" },
	{ 0x88c73071, "i2c_transfer_buffer_flags" },
	{ 0xf9a482f9, "msleep" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xb39c75ca, "_dev_err" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x29ff5941, "__dynamic_dev_dbg" },
	{ 0x58c6fed5, "__v4l2_ctrl_s_ctrl" },
	{ 0xf9c815c5, "__v4l2_ctrl_modify_range" },
	{ 0x378c9f8c, "i2c_transfer" },
	{ 0xc6142812, "v4l2_async_unregister_subdev" },
	{ 0xa40db77f, "v4l2_ctrl_handler_free" },
	{ 0x1bc34352, "__pm_runtime_disable" },
	{ 0xfe7d1286, "module_layout" },
};

MODULE_INFO(depends, "videodev,mc,v4l2-fwnode,v4l2-async");

MODULE_ALIAS("i2c:ar0234");

MODULE_INFO(srcversion, "1742A19DE6BF5F01B6969D3");
