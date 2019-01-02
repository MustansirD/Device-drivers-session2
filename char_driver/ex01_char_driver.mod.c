#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

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
	{ 0xeadeda83, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x35512611, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xeb9c2039, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xdc80cbdc, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xe066d0c0, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xce240e75, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x511bbb00, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xb587ca13, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x2f784409, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xad35c3c0, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xbc939a07, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5B7D100E484FED216B52C5E");
