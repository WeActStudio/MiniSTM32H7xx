#include "ov7725.h"
#include "ov7725_regs.h"
#include <stdbool.h>

static uint8_t ov7725_WR_Reg(uint8_t reg, uint8_t data)
{
    Camera_WriteReg(&hcamera, reg, &data);
    return 0;
}

static uint8_t ov7725_RD_Reg(uint8_t reg,uint8_t *data)
{
    return Camera_ReadReg(&hcamera,reg,data);
}

static int ov7725_reset(void)
{
    // Reset all registers
    int ret = ov7725_WR_Reg( COM7, COM7_RESET);

    // Delay 2 ms
    Camera_delay(2);

    // Write default regsiters
    for (int i = 0; ov7725_default_regs[i][0]; i++) {
        ret |= ov7725_WR_Reg(ov7725_default_regs[i][0], ov7725_default_regs[i][1]);
    }

    // Delay 300 ms
    Camera_delay(300);

    return ret;
}

static int ov7725_set_pixformat(pixformat_t pixformat)
{
    uint8_t reg;
    int ret = ov7725_RD_Reg( COM7, &reg);

    switch (pixformat) {
        case PIXFORMAT_RGB565:
            reg = COM7_SET_FMT(reg, COM7_FMT_RGB);
            ret |= ov7725_WR_Reg( DSP_CTRL4, DSP_CTRL4_YUV_RGB);
            break;
        case PIXFORMAT_YUV422:
        case PIXFORMAT_GRAYSCALE:
            reg = COM7_SET_FMT(reg, COM7_FMT_YUV);
            ret |= ov7725_WR_Reg(DSP_CTRL4, DSP_CTRL4_YUV_RGB);
            break;
        case PIXFORMAT_BAYER:
            reg = COM7_SET_FMT(reg, COM7_FMT_P_BAYER);
            ret |= ov7725_WR_Reg(DSP_CTRL4, DSP_CTRL4_RAW8);
            break;
        default:
            return -1;
    }

    // Write back register
    return ov7725_WR_Reg(COM7, reg) | ret;
}

static int ov7725_set_framesize(framesize_t framesize)
{
    uint8_t reg;
    int ret=0;
    uint16_t w = dvp_cam_resolution[framesize][0];
    uint16_t h = dvp_cam_resolution[framesize][1];
    bool vflip;

    if ((w > 640) || (h > 480)) {
        return -1;
    }

    // Write MSBs
    ret |= ov7725_WR_Reg(HOUTSIZE, w>>2);
    ret |= ov7725_WR_Reg(VOUTSIZE, h>>1);

    // Write LSBs
    ret |= ov7725_WR_Reg(EXHCH, ((w&0x3) | ((h&0x1) << 2)));

    // Sample VFLIP
    ret |= ov7725_RD_Reg(COM3, &reg);
    vflip = reg & COM3_VFLIP;
    ret |= ov7725_RD_Reg(HREF, &reg);
    ret |= ov7725_WR_Reg(HREF, (reg & 0xBF) | (vflip ? 0x40 : 0x00));

    if ((w <= 320) && (h <= 240)) {
        // Set QVGA Resolution
        uint8_t reg;
        int ret = ov7725_RD_Reg(COM7, &reg);
        reg = COM7_SET_RES(reg, COM7_RES_QVGA);
        ret |= ov7725_WR_Reg(COM7, reg);

        // Set QVGA Window Size
        ret |= ov7725_WR_Reg(HSTART, 0x3F);
        ret |= ov7725_WR_Reg(HSIZE,  0x50);
        ret |= ov7725_WR_Reg(VSTART, 0x03 - vflip);
        ret |= ov7725_WR_Reg(VSIZE,  0x78);

        // Enable auto-scaling/zooming factors
        ret |= ov7725_WR_Reg(DSPAUTO, 0xFF);
    } else {
        // Set VGA Resolution
        uint8_t reg;
        int ret = ov7725_RD_Reg(COM7, &reg);
        reg = COM7_SET_RES(reg, COM7_RES_VGA);
        ret |= ov7725_WR_Reg(COM7, reg);

        // Set VGA Window Size
        ret |= ov7725_WR_Reg(HSTART, 0x23);
        ret |= ov7725_WR_Reg(HSIZE,  0xA0);
        ret |= ov7725_WR_Reg(VSTART, 0x07 - vflip);
        ret |= ov7725_WR_Reg(VSIZE,  0xF0);

        // Disable auto-scaling/zooming factors
        ret |= ov7725_WR_Reg(DSPAUTO, 0xF3);

        // Clear auto-scaling/zooming factors
        ret |= ov7725_WR_Reg(SCAL0, 0x00);
        ret |= ov7725_WR_Reg(SCAL1, 0x40);
        ret |= ov7725_WR_Reg(SCAL2, 0x40);
    }

    return ret;
}
static int ov7725_set_hmirror(int enable)
{
    uint8_t reg;
    int ret = ov7725_RD_Reg(COM3, &reg);
    ret |= ov7725_WR_Reg(COM3, COM3_SET_MIRROR(reg, enable)) ;

    return ret;
}

static int ov7725_set_vflip(int enable)
{
    uint8_t reg;
    int ret = ov7725_RD_Reg(COM3, &reg);
    ret |= ov7725_WR_Reg(COM3, COM3_SET_FLIP(reg, enable));
    // Apply new vertical flip setting.
    ret |= ov7725_set_framesize(hcamera.framesize);

    return ret;
}

int ov7725_init(framesize_t framesize)
{
	ov7725_reset();
	hcamera.framesize = framesize;
	hcamera.pixformat = PIXFORMAT_RGB565;
	ov7725_set_pixformat(hcamera.pixformat);
	ov7725_set_framesize(hcamera.framesize);
	ov7725_set_hmirror(1);
	ov7725_set_vflip(1);
	
	return 1;
}

