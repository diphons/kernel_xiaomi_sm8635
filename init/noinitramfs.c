// SPDX-License-Identifier: GPL-2.0-only
/*
 * init/noinitramfs.c
 *
 * Copyright (C) 2006, NXP Semiconductors, All Rights Reserved
 * Author: Jean-Paul Saman <jean-paul.saman@nxp.com>
 */
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/kdev_t.h>
#include <linux/syscalls.h>
#include <linux/init_syscalls.h>
#include <linux/umh.h>
#include <linux/initramfs.h>

/*
 * Create a simple rootfs that is similar to the default initramfs
 */
#if !IS_BUILTIN(CONFIG_BLK_DEV_INITRD)
static
#endif
int __init default_rootfs(void)
{
	int err;

	usermodehelper_enable();
	err = init_mkdir("/dev", 0755);
	if (err < 0)
		goto out;

	err = init_mknod("/dev/console", S_IFCHR | S_IRUSR | S_IWUSR,
			new_encode_dev(MKDEV(5, 1)));
	if (err < 0)
		goto out;

	err = init_mkdir("/root", 0700);
	if (err < 0)
		goto out;

	return 0;

out:
	printk(KERN_WARNING "Failed to create a rootfs\n");
	return err;
}
#if !IS_BUILTIN(CONFIG_BLK_DEV_INITRD)
rootfs_initcall(default_rootfs);
#endif
