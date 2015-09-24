/*
 * Copyright (C) 2011 Samsung Electronics Co. Ltd. All Rights Reserved.
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

#ifndef _LEDS_AN30259A_H
#define _LEDS_AN30259A_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define LED_LIGHT_OFF	0
#define LED_LIGHT_ON	1
#define LED_LIGHT_PULSE	2
#define LED_LIGHT_SLOPE	3


struct an30259a_pr_control {
	
	__u32			color;
	
	__u32			state;
	
	__u16			start_delay;
	
	__u16			time_slope_up_1;
	
	__u16			time_slope_up_2;
	
	__u16			time_on;
	
	__u16			time_slope_down_1;
	
	__u16			time_slope_down_2;
	
	__u16			time_off;
	
	__u8			mid_brightness;
} __packed;

#define AN30259A_PR_SET_LED	_IOW('S', 42, struct an30259a_pr_control)
#define AN30259A_PR_SET_LEDS	_IOW('S', 43, struct an30259a_pr_control[3])
#define AN30259A_PR_SET_IMAX	_IOW('S', 44, __u8)
#endif						
