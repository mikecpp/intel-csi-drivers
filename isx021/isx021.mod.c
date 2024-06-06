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
	{ 0xb55e8ce, "i2c_register_driver" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xf1d249f1, "devm_kmalloc" },
	{ 0x43ee2256, "media_entity_pads_init" },
	{ 0x95873975, "v4l2_async_register_subdev_sensor" },
	{ 0xb39c75ca, "_dev_err" },
	{ 0xc6142812, "v4l2_async_unregister_subdev" },
	{ 0x80df923a, "i2c_del_driver" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xfe7d1286, "module_layout" },
};

MODULE_INFO(depends, "mc,v4l2-fwnode,v4l2-async");


MODULE_INFO(srcversion, "7440528DBEE0FEEDE0FF8C2");
