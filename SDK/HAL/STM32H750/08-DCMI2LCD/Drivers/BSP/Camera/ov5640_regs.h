/*
 * This file is part of the MicroPython K210 project, https://github.com/loboris/MicroPython_K210_LoBo
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 LoBo (https://github.com/loboris)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Based on OV2640 driver from OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV2640 register definitions.
 */

#ifndef __REG_REGS_H__
#define __REG_REGS_H__

#include <stdint.h>

#define SYSTEM_RESET_02     0x3002

#define CLOCK_ENABLE_02     0x3006

#define SYSTEM_CTROL0       0x3008

#define SC_PLL_CONTRL2      0x3036

#define SCCB_SYSTEM_CTRL_1  0x3103

#define AWB_R_GAIN_H        0x3400
#define AWB_R_GAIN_L        0x3401

#define AWB_G_GAIN_H        0x3402
#define AWB_G_GAIN_L        0x3403

#define AWB_B_GAIN_H        0x3404
#define AWB_B_GAIN_L        0x3405

#define AWB_MANUAL_CONTROL  0x3406

#define AEC_PK_EXPOSURE_0   0x3500
#define AEC_PK_EXPOSURE_1   0x3501
#define AEC_PK_EXPOSURE_2   0x3502

#define AEC_PK_MANUAL       0x3503

#define AEC_PK_REAL_GAIN_H  0x350A
#define AEC_PK_REAL_GAIN_L  0x350B

#define TIMING_HS_H         0x3800
#define TIMING_HS_L         0x3801

#define TIMING_VS_H         0x3802
#define TIMING_VS_L         0x3803

#define TIMING_HW_H         0x3804
#define TIMING_HW_L         0x3805

#define TIMING_VH_H         0x3806
#define TIMING_VH_L         0x3807

#define TIMING_DVPHO_H      0x3808
#define TIMING_DVPHO_L      0x3809

#define TIMING_DVPVO_H      0x380A
#define TIMING_DVPVO_L      0x380B

#define TIMING_HTS_H        0x380C
#define TIMING_HTS_L        0x380D

#define TIMING_VTS_H        0x380E
#define TIMING_VTS_L        0x380F

#define TIMING_HOFFSET_H    0x3810
#define TIMING_HOFFSET_L    0x3811

#define TIMING_VOFFSET_H    0x3812
#define TIMING_VOFFSET_L    0x3813

#define TIMING_X_INC        0x3814
#define TIMING_Y_INC        0x3815

#define TIMING_TC_REG_20    0x3820
#define TIMING_TC_REG_21    0x3821

#define AEC_GAIN_CEILING_H  0x3A18
#define AEC_GAIN_CEILING_L  0x3A18

#define FORMAT_CONTROL      0x4300

#define VFIFO_HSIZE_H       0x4602
#define VFIFO_HSIZE_L       0x4603

#define VFIFO_VSIZE_H       0x4604
#define VFIFO_VSIZE_L       0x4605

#define JPEG_CTRL07         0x4407

#define ISP_CONTROL_00      0x5300

#define FORMAT_CONTROL_MUX  0x501F

#define PRE_ISP_TEST        0x503D

extern const uint8_t ov5640_default_regs[][3];

#endif //__REG_REGS_H__
