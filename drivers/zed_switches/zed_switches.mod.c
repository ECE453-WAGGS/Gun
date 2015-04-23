#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xcf79a31, "module_layout" },
	{ 0x1976aa06, "param_ops_bool" },
	{ 0x534cad44, "platform_driver_unregister" },
	{ 0xb339b217, "platform_driver_register" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x91715312, "sprintf" },
	{ 0xdcb83c11, "kernel_kobj" },
	{ 0xfe5cf2d2, "sysfs_create_group" },
	{ 0x496fa379, "kobject_create_and_add" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xadf42bd5, "__request_region" },
	{ 0x98d96aa2, "_dev_info" },
	{ 0xa9d7c341, "dev_err" },
	{ 0x878dd66b, "platform_get_resource" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x6818a5, "kobject_put" },
	{ 0x9bce482f, "__release_region" },
	{ 0xc2165d85, "__arm_iounmap" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

