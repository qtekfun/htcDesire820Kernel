/*
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *   Dima Zavin <dima@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __LINUX_TEGRA_SEMA_H
#define __LINUX_TEGRA_SEMA_H

#define TEGRA_SEMA_IOCTL_MAGIC		'r'

#define TEGRA_SEMA_IOCTL_WAIT		_IOW(TEGRA_SEMA_IOCTL_MAGIC, 0x30, long *)
#define TEGRA_SEMA_IOCTL_SIGNAL		_IO(TEGRA_SEMA_IOCTL_MAGIC, 0x31)

#define TEGRA_SEMA_IOCTL_MIN_NR		_IOC_NR(TEGRA_SEMA_IOCTL_WAIT)
#define TEGRA_SEMA_IOCTL_MAX_NR		_IOC_NR(TEGRA_SEMA_IOCTL_SIGNAL)

#endif
