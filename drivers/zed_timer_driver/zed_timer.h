/*
 * Linux 2.6 and 3.0 'parrot' sample device driver
 *
 * Copyright (c) 2011, Pete Batard <pete@akeo.ie>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* This driver was derived from the parrot driver written by Pete Batard and
 * the example driver provided by digilent for the ZedBoard.
 */

#define DRIVER_NAME "zed_timer"
#define DEVICE_NAME "device"
#define CLASS_NAME "zed_timer"


#define AUTHOR "ECE453 Staff"
#define DESCRIPTION "zedboard device driver for PL"
#define VERSION "0.1"

/* We'll use our own macros for printk */
#define dbg(format, arg...) do { if (debug) pr_info(CLASS_NAME ": %s: " format , __FUNCTION__ , ## arg); } while (0)
#define err(format, arg...) pr_err(CLASS_NAME ": " format, ## arg)
#define info(format, arg...) pr_info(CLASS_NAME ": " format, ## arg)
#define warn(format, arg...) pr_warn(CLASS_NAME ": " format, ## arg)

#define TCSR_CASC_M	(1 << 11)	// Enable Cascade Mode
#define TCSR_ENALL_M	(1 << 10)	// Enable all timers
#define TCSR_PWMA0_M	(1 <<  9)	// PWM Mode
#define TCSR_INT_M	(1 <<  8)	// Interrupt Status
#define TCSR_ENT_M	(1 <<  7)	// Enable Timer
#define TCSR_EINT_M	(1 <<  6)	// Enable Interrupts
#define TCSR_LOAD_M	(1 <<  5)	// Load with value in TLRx
#define TCSR_ARHT_M	(1 <<  4)	// Auto Reload
#define TCSR_EX_CAPT_M	(1 <<  3)	// External Capture Mode
#define TCSR_EX_GEN_M	(1 <<  2)	// External Generate Mode
#define TCSR_UDT_UP_M	(0 <<  1)	// Up/Down Counter
#define TCSR_UDT_DOWN_M	(1 <<  1)	// Up/Down Counter
#define TCSR_MODE_CAP_M	(1 <<  0)	// Capture Mode
#define TCSR_MODE_GEN_M	(0 <<  0)	// Generate Mode

#define SECOND_1	(100 * 1000000)	// Assume a 100MHz clock

