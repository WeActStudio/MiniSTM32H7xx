#include "ov7725_regs.h"

const uint8_t ov7725_default_regs[][2] = {

// From App Note.

    {COM12,         0x03},
    {HSTART,        0x22},
    {HSIZE,         0xa4},
    {VSTART,        0x07},
    {VSIZE,         0xf0},
    {HREF,          0x00},
    {HOUTSIZE,      0xa0},
    {VOUTSIZE,      0xf0},
    {EXHCH,         0x00},
    {CLKRC,         0x81}, // {CLKRC, 0xc0}, 12Mhz Input / div 2 = 6Mhz

    {TGT_B,         0x7f},
    {FIXGAIN,       0x09},
    {AWB_CTRL0,     0xe0},
    {DSP_CTRL1,     0xff},
    {DSP_CTRL2,     0x20 | DSP_CTRL2_VDCW_EN | DSP_CTRL2_HDCW_EN | DSP_CTRL2_VZOOM_EN | DSP_CTRL2_HZOOM_EN}, // {DSP_CTRL2, 0x20},
    {DSP_CTRL3,     0x00},
    {DSP_CTRL4,     0x48},
		
    {COM10,         COM10_VSYNC_NEG},
    {COM8,          0xf0},
    {COM4,          COM4_PLL_4x}, // 6Mhz x 6 = 36Mhz, Max FPS: 113FPS QQVGA
    {COM6,          0xc5},
    {COM9,          0x11},
    {BDBASE,        0x7f},
    {BDSTEP,        0x03},
    {AEW,           0x40},
    {AEB,           0x30},
    {VPT,           0xa1},
    {EXHCL,         0x00},
    {AWB_CTRL3,     0xaa},
    {COM8,          0xff},

    {EDGE1,         0x05},
    {DNSOFF,        0x01},
    {EDGE2,         0x03},
    {EDGE3,         0x00},
    {MTX1,          0xb0},
    {MTX2,          0x9d},
    {MTX3,          0x13},
    {MTX4,          0x16},
    {MTX5,          0x7b},
    {MTX6,          0x91},
    {MTX_CTRL,      0x1e},
    {BRIGHTNESS,    0x08},
    {CONTRAST,      0x20},
    {UVADJ0,        0x81},
    {SDE,           SDE_CONT_BRIGHT_EN | SDE_SATURATION_EN},

    {GAM1,          0x0c},
    {GAM2,          0x16},
    {GAM3,          0x2a},
    {GAM4,          0x4e},
    {GAM5,          0x61},
    {GAM6,          0x6f},
    {GAM7,          0x7b},
    {GAM8,          0x86},
    {GAM9,          0x8e},
    {GAM10,         0x97},
    {GAM11,         0xa4},
    {GAM12,         0xaf},
    {GAM13,         0xc5},
    {GAM14,         0xd7},
    {GAM15,         0xe8},
    {SLOP,          0x20},

    {DM_LNL,        0x00},
    {BDBASE,        0x7f}, // {BDBASE, 0x7f}
    {BDSTEP,        0x03},

    {LC_RADI,       0x10},
    {LC_COEF,       0x10},
    {LC_COEFB,      0x14},
    {LC_COEFR,      0x17},
    {LC_CTR,        0x01}, // {LC_CTR, 0x05},

    {COM5,          0xf5}, // {COM5, 0x65},

// OpenMV Custom.

    {COM7,          COM7_FMT_RGB565},

// End.

    {0x00,          0x00},
};

