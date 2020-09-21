#include "ov2640.h"
#include "ov2640_regs.h"

#define OV2640_XCLK_FREQUENCY       (20000000)
#define OV2640_NUM_ALLOWED_SIZES    (19)
#define NUM_BRIGHTNESS_LEVELS       (5)
#define NUM_CONTRAST_LEVELS         (5)
#define NUM_SATURATION_LEVELS       (5)
#define NUM_EFFECTS                 (9)
#define REGLENGTH 8

#define ov2640_delay HAL_Delay

static const uint8_t allowed_sizes[OV2640_NUM_ALLOWED_SIZES] = {
        FRAMESIZE_CIF,      // 352x288
        FRAMESIZE_SIF,      // 352x240
        FRAMESIZE_QQVGA,    // 160x120
        FRAMESIZE_128X64,   // 128x64
        FRAMESIZE_QVGA,     // 320x240
        FRAMESIZE_VGA,      // 640x480
        FRAMESIZE_HVGA,     // 480 * 320
        FRAMESIZE_WVGA2,    // 752x480
        FRAMESIZE_SVGA,     // 800x600
        FRAMESIZE_XGA,      // 1024x768
        FRAMESIZE_SXGA,     // 1280x1024
        FRAMESIZE_UXGA,     // 1600x1200
};

//---------------------------------------
static const uint8_t rgb565_regs[][2] = {
    { BANK_SEL, BANK_SEL_DSP },
    { REG_RESET,   REG_RESET_DVP},
    { IMAGE_MODE, IMAGE_MODE_RGB565 },
    { 0xD7,     0x03 },
    { 0xE1,     0x77 },
    { REG_RESET,    0x00 },
    {0, 0},
};

//-------------------------------------
static const uint8_t jpeg_regs[][2] = {
    {BANK_SEL,      BANK_SEL_DSP},
    {REG_RESET,         REG_RESET_DVP},
    {IMAGE_MODE,    IMAGE_MODE_JPEG_EN | IMAGE_MODE_RGB565}, // | IMAGE_MODE_HREF_VSYNC
    {0xd7,          0x03},
    {0xe1,          0x77},
    //{QS,            0x0c},
    {REG_RESET,         0x00},
    {0,             0},
};

static const uint8_t yuyv_regs[][2] = {
	{BANK_SEL,      BANK_SEL_DSP},
  {REG_RESET,     REG_RESET_DVP},
	{ IMAGE_MODE, IMAGE_MODE_YUV422 },
	{ 0xd7, 0x03 },
	{ 0x33, 0xa0 },
	{ 0xe5, 0x1f },
	{ 0xe1, 0x67 },
	{ REG_RESET,  0x00 },
	{0,             0},
};

//--------------------------------------------------------------------
static const uint8_t brightness_regs[NUM_BRIGHTNESS_LEVELS + 1][5] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA},
    {0x00, 0x04, 0x09, 0x00, 0x00}, /* -2 */
    {0x00, 0x04, 0x09, 0x10, 0x00}, /* -1 */
    {0x00, 0x04, 0x09, 0x20, 0x00}, /*  0 */
    {0x00, 0x04, 0x09, 0x30, 0x00}, /* +1 */
    {0x00, 0x04, 0x09, 0x40, 0x00}, /* +2 */
};

//----------------------------------------------------------------
static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS + 1][7] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA, BPDATA, BPDATA},
    {0x00, 0x04, 0x07, 0x20, 0x18, 0x34, 0x06}, /* -2 */
    {0x00, 0x04, 0x07, 0x20, 0x1c, 0x2a, 0x06}, /* -1 */
    {0x00, 0x04, 0x07, 0x20, 0x20, 0x20, 0x06}, /*  0 */
    {0x00, 0x04, 0x07, 0x20, 0x24, 0x16, 0x06}, /* +1 */
    {0x00, 0x04, 0x07, 0x20, 0x28, 0x0c, 0x06}, /* +2 */
};

//--------------------------------------------------------------------
static const uint8_t saturation_regs[NUM_SATURATION_LEVELS + 1][5] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA},
    {0x00, 0x02, 0x03, 0x28, 0x28}, /* -2 */
    {0x00, 0x02, 0x03, 0x38, 0x38}, /* -1 */
    {0x00, 0x02, 0x03, 0x48, 0x48}, /*  0 */
    {0x00, 0x02, 0x03, 0x58, 0x58}, /* +1 */
    {0x00, 0x02, 0x03, 0x68, 0x68}, /* +2 */
};

//--------------------------------------------------------
static const uint8_t OV2640_EFFECTS_TBL[NUM_EFFECTS][3]= {
    //00-0 05-0  05-1
    {0x00, 0x80, 0x80}, // Normal
    {0x18, 0xA0, 0x40}, // Blueish (cool light)
    {0x18, 0x40, 0xC0}, // Redish (warm)
    {0x18, 0x80, 0x80}, // Black and white
    {0x18, 0x40, 0xA6}, // Sepia
    {0x40, 0x80, 0x80}, // Negative
    {0x18, 0x50, 0x50}, // Greenish
    {0x58, 0x80, 0x80}, // Black and white negative
    {0x00, 0x80, 0x80}, // Normal
};

//-----------------------------------------------------
static uint8_t OV2640_WR_Reg(uint8_t reg, uint8_t data)
{
    Camera_WriteReg(&hcamera, reg, &data);
    return 0;
}

//---------------------------------------
static uint8_t OV2640_RD_Reg(uint8_t reg)
{
	uint8_t data;
  Camera_ReadReg(&hcamera,reg,&data);
    return data;
}

//------------------------------------------------
static void wrSensorRegs(const uint8_t (*regs)[2])
{
    for (int i = 0; regs[i][0]; i++) {
        Camera_WriteReg(&hcamera, regs[i][0], &regs[i][1]);
    }
}

//----------------
static int reset()
{
	  ov2640_delay(100);
    // Reset all registers
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    OV2640_WR_Reg(COM7, COM7_SRST);
    // Delay 5 ms
    ov2640_delay(5);
    wrSensorRegs(ov2640_Slow_regs);
    // 30 ms
    ov2640_delay(30);

    return 0;
}

//----------------------------------------
static int set_special_effect(uint8_t sde)
{
    if (sde >= NUM_EFFECTS) return -1;
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(BPADDR, 0x00);
    OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][0]);
    OV2640_WR_Reg(BPADDR, 0x05);
    OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][1]);
    OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][2]);

    return 0;
}

//-----------------------------------
static int set_exposure(int exposure)
{
    int ret = 0;
    // Disable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    if (exposure == -1) {
        // get exposure
        uint16_t exp = (uint16_t)(OV2640_RD_Reg(REG45) & 0x3f) << 10;
        exp |= (uint16_t)OV2640_RD_Reg(AEC) << 2;
        exp |= (uint16_t)OV2640_RD_Reg(REG04) & 0x03;
        ret = (int)exp;
    }
    else if (exposure == -2) {
        // disable auto exposure and gain
        OV2640_WR_Reg(COM8, COM8_SET(0));
    }
    else if (exposure > 0) {
        // set manual exposure
        int max_exp = (dvp_cam_resolution[hcamera.framesize][0] <= 800) ? 672 : 1248;
        if (exposure > max_exp) exposure = max_exp;
        OV2640_WR_Reg(COM8, COM8_SET(0));
        OV2640_WR_Reg(REG45, (uint8_t)((exposure >> 10) & 0x3f));
        OV2640_WR_Reg(AEC, (uint8_t)((exposure >> 2) & 0xff));
        OV2640_WR_Reg(REG04, (uint8_t)(exposure & 3));
    }
    else {
        // enable auto exposure and gain
        OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
    }

    // Enable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
    return ret;
}

//-------------------------------------------
static void _set_framesize(uint8_t framesize)
{
    uint8_t cbar, qsreg, com7;

    // save color bar status and qs register
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    cbar = OV2640_RD_Reg(COM7) & COM7_COLOR_BAR;
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    qsreg = OV2640_RD_Reg(QS);

    uint16_t w = dvp_cam_resolution[framesize][0];
    uint16_t h = dvp_cam_resolution[framesize][1];
    const uint8_t (*regs)[2];

    if (w <= dvp_cam_resolution[FRAMESIZE_SVGA][0]) regs = OV2640_svga_regs;
    else regs = OV2640_uxga_regs;

    // Disable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

    // Write output width
    OV2640_WR_Reg(ZMOW, (w>>2)&0xFF); // OUTW[7:0] (real/4)
    OV2640_WR_Reg(ZMOH, (h>>2)&0xFF); // OUTH[7:0] (real/4)
    OV2640_WR_Reg(ZMHH, ((h>>8)&0x04)|((w>>10)&0x03)); // OUTH[8]/OUTW[9:8]

    // Set CLKRC
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    //OV2640_WR_Reg(CLKRC, ov2640_sensor->night_mode);

    // Write DSP input regsiters
    wrSensorRegs(regs);

    // restore color bar status
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    com7 = OV2640_RD_Reg(COM7) | cbar;
    OV2640_WR_Reg(COM7, com7);

    // restore qs register
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(QS, qsreg);

    // Enable DSP
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
}

//---------------------------------------------
static int set_pixformat(pixformat_t pixformat)
{
    // Disable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

    switch (pixformat) {
        case PIXFORMAT_RGB565:
            //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
            //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
            wrSensorRegs(rgb565_regs);
            break;
        case PIXFORMAT_JPEG:
            //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
            //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN));
            wrSensorRegs(jpeg_regs);
            break;
				case PIXFORMAT_YUV422:
					 wrSensorRegs(yuyv_regs);
						break;
        default:
            // Enable DSP
            OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
            OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
            return -1;
    }
    _set_framesize(hcamera.framesize);
    // Enable DSP (enabled in '_set_framesize'
    //OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    //OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
    // Delay 30 ms
    //ov2640_delay(30);

    return 0;
}

//-----------------------------------------
static int set_framesize(framesize_t framesize)
{
    int i = OV2640_NUM_ALLOWED_SIZES;
    for (i=0; i<OV2640_NUM_ALLOWED_SIZES; i++) {
        if (allowed_sizes[i] == framesize) break;
    }
    if (i >= OV2640_NUM_ALLOWED_SIZES) {
        //LOGW(TAG, "Frame size %d not allowed", framesize);
        return -1;
    }

    hcamera.framesize = framesize;
    _set_framesize(framesize);

    //delay 30 ms
    ov2640_delay(30);

    return 0;
}

//-------------------------------------------
int ov2640_check_framesize(uint8_t framesize)
{
    int i = OV2640_NUM_ALLOWED_SIZES;
    for (i=0; i<OV2640_NUM_ALLOWED_SIZES; i++) {
        if (allowed_sizes[i] == framesize) break;
    }
    if (i >= OV2640_NUM_ALLOWED_SIZES) return -1;
    return 0;
}

//--------------------------------
static int set_contrast(int level)
{
    level += (NUM_CONTRAST_LEVELS / 2) + 1;
    if (level < 0 || level > NUM_CONTRAST_LEVELS) {
        return -1;
    }

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
    // Write contrast registers
    for (int i=0; i<sizeof(contrast_regs[0])/sizeof(contrast_regs[0][0]); i++) {
        OV2640_WR_Reg(contrast_regs[0][i], contrast_regs[level][i]);
    }
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

    return 0;
}

//----------------------------------
static int set_brightness(int level)
{
    level += (NUM_BRIGHTNESS_LEVELS / 2) + 1;
    if ((level < 0) || (level > NUM_BRIGHTNESS_LEVELS)) return -1;

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
    // Write brightness registers
    for (int i=0; i<sizeof(brightness_regs[0])/sizeof(brightness_regs[0][0]); i++) {
        OV2640_WR_Reg(brightness_regs[0][i], brightness_regs[level][i]);
    }
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

    return 0;
}

//----------------------------------
static int set_saturation(int level)
{
    level += (NUM_SATURATION_LEVELS / 2) + 1;
    if ((level < 0) || (level > NUM_SATURATION_LEVELS)) return -1;

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
    // Write saturation registers
    for (int i=0; i<sizeof(saturation_regs[0])/sizeof(saturation_regs[0][0]); i++) {
        OV2640_WR_Reg(saturation_regs[0][i], saturation_regs[level][i]);
    }
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

    return 0;
}

//----------------------------
static int set_quality(int qs)
{
    if ((qs < 2) || (qs > 60)) return -1;

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    // Write QS register
    OV2640_WR_Reg(QS, qs);

    return 0;
}

//---------------------------------
static int set_colorbar(int enable)
{
    uint8_t reg;
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    reg = OV2640_RD_Reg(COM7);

    if (enable) reg |= COM7_COLOR_BAR;
    else reg &= ~COM7_COLOR_BAR;

    return OV2640_WR_Reg(COM7, reg);
}

//--------------------------------
static int set_hmirror(int enable)
{
    uint8_t reg;
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    reg = OV2640_RD_Reg(REG04);

    if (!enable) { // Already mirrored.
        reg |= REG04_HFLIP_IMG;
    }
    else {
        reg &= ~REG04_HFLIP_IMG;
    }

    return OV2640_WR_Reg(REG04, reg);
}

//------------------------------
static int set_vflip(int enable)
{
    uint8_t reg;
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    reg = OV2640_RD_Reg(REG04);

    if (!enable) { // Already flipped.
        reg |= REG04_VFLIP_IMG | REG04_VREF_EN;
    }
    else {
        reg &= ~(REG04_VFLIP_IMG | REG04_VREF_EN);
    }

    return OV2640_WR_Reg(REG04, reg);
}

//---------------------------------------------------------
static int read_id(uint16_t *manuf_id, uint16_t *device_id)
{
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    *manuf_id = ((uint16_t)OV2640_RD_Reg(0x1C) << 8) | OV2640_RD_Reg(0x1D);
    *device_id = ((uint16_t)OV2640_RD_Reg(0x0A) << 8) | OV2640_RD_Reg(0x0B);
    return 0;
}

//------------------------------------
static int read_reg(uint16_t reg_addr)
{
    return (int)OV2640_RD_Reg((uint8_t) reg_addr);
}

//--------------------------------------------------------
static int write_reg(uint16_t reg_addr, uint16_t reg_data)
{
    return (int)OV2640_WR_Reg((uint8_t)reg_addr, (uint8_t)reg_data);
}

static const uint8_t OV2640_LIGHTMODE_TBL[5][3]=
{
    {0x5e, 0x41, 0x54}, //Auto, NOT used
    {0x5e, 0x41, 0x54}, //Sunny
    {0x52, 0x41, 0x66}, //Office
    {0x65, 0x41, 0x4f}, //Cloudy
    {0x42, 0x3f, 0x71}, //Home
};


//-------------------------------------
static int set_light_mode(uint8_t mode)
{
    if (mode > 4) return -1;

    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    if (mode == 0) {
        OV2640_WR_Reg(0xc7, 0x00); // AWB on
    }
    else {
        OV2640_WR_Reg(0xc7, 0x40); // AWB off
        OV2640_WR_Reg(0xcc, OV2640_LIGHTMODE_TBL[mode][0]);
        OV2640_WR_Reg(0xcd, OV2640_LIGHTMODE_TBL[mode][1]);
        OV2640_WR_Reg(0xce, OV2640_LIGHTMODE_TBL[mode][2]);
    }
    return 0;
}

//-----------------------------------
static int set_night_mode(int enable)
{
    // Disable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
    // Set CLKRC
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
    OV2640_WR_Reg(CLKRC, (enable) ? 0 : CLKRC_DOUBLE);
    // Enable DSP
    OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
    OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
    //delay 30 ms
    ov2640_delay(30);
    return 0;
}

//===============================
int ov2640_init(framesize_t framesize)
{
	reset();
	hcamera.framesize = framesize;
	hcamera.pixformat = PIXFORMAT_RGB565;
	//set_framesize(FRAMESIZE_QQVGA);
	set_pixformat(hcamera.pixformat);
	set_hmirror(0);
	set_vflip(0);
  return 0;
}

