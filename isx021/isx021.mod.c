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
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xc52626b9, "i2c_unregister_device" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x1e6088dd, "i2c_get_adapter" },
	{ 0x9a7186c3, "i2c_new_client_device" },
	{ 0x92997ed8, "_printk" },
	{ 0x88c73071, "i2c_transfer_buffer_flags" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xfe7d1286, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A4E14FDDC3B82858A65944F");
