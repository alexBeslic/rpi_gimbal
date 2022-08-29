#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x121f1e9, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x247686f0, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x2c519da9, __VMLINUX_SYMBOL_STR(pwm_free) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x2d1c153c, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x39db6fbb, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xb1ca4c3, __VMLINUX_SYMBOL_STR(pwm_request) },
	{ 0x4b40d4d3, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x35fe3760, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xfe206318, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x5e55a319, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0xbee66127, __VMLINUX_SYMBOL_STR(pwm_apply_state) },
	{ 0x11a13e31, __VMLINUX_SYMBOL_STR(_kstrtol) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8F3447044EE133D2D980048");
