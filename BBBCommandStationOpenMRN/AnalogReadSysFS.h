// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 27 19:51:05 2019
//  Last Modified : <221117.1550>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//! @copyright
//!    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//!			51 Locke Hill Road
//!			Wendell, MA 01379-9728
//!
//!    This program is free software; you can redistribute it and/or modify
//!    it under the terms of the GNU General Public License as published by
//!    the Free Software Foundation; either version 2 of the License, or
//!    (at your option) any later version.
//!
//!    This program is distributed in the hope that it will be useful,
//!    but WITHOUT ANY WARRANTY; without even the implied warranty of
//!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//!    GNU General Public License for more details.
//!
//!    You should have received a copy of the GNU General Public License
//!    along with this program; if not, write to the Free Software
//!    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//!
//!
//! @file AnalogReadSysFS.h
//! Reads the analog inputs, using the SysFS filesystem.
//!
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANALOGREADSYSFS_H
#define __ANALOGREADSYSFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BUF 255
#define SYSFS_ADC_DIR "/sys/devices/platform/ocp/44e0d000.tscadc/TI-am335x-adc.0.auto/iio:device0/"

uint32_t sysfs_adc_getvalue(uint32_t channel);

#define AIN0 0
#define AIN1 1
#define AIN2 2
#define AIN3 3
#define AIN4 4
#define AIN5 5
#define AIN6 6
/* AIN7 not available -- it is wired to a 50/50 voltage divider on the 3.3V supply */

#ifdef __cplusplus
}
#endif

#endif // __ANALOGREADSYSFS_H

