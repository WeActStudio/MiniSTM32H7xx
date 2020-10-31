#include "ov5640.h"
#include "ov5640_regs.h"
#include <stdbool.h>

// Need GNU extensions
#define IM_MAX(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define IM_MIN(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#define BLANK_LINES 8
#define DUMMY_LINES 6

#define BLANK_COLUMNS 0
#define DUMMY_COLUMNS 16

#define SENSOR_WIDTH 2624
#define SENSOR_HEIGHT 1964

#define ACTIVE_SENSOR_WIDTH (SENSOR_WIDTH - BLANK_COLUMNS - (2 * DUMMY_COLUMNS))
#define ACTIVE_SENSOR_HEIGHT (SENSOR_HEIGHT - BLANK_LINES - (2 * DUMMY_LINES))

#define DUMMY_WIDTH_BUFFER 16
#define DUMMY_HEIGHT_BUFFER 8

#define HSYNC_TIME 252
#define VYSNC_TIME 24

static uint16_t hts_target = 0;

static int16_t readout_x = 0;
static int16_t readout_y = 0;

static uint16_t readout_w = ACTIVE_SENSOR_WIDTH;
static uint16_t readout_h = ACTIVE_SENSOR_HEIGHT;

static uint8_t ov5640_WR_Reg(uint16_t reg, uint8_t data)
{
    Camera_WriteRegb2(&hcamera, reg, data);
    return 0;
}

static uint8_t ov5640_RD_Reg(uint16_t reg, uint8_t *data)
{

    return Camera_ReadRegb2(&hcamera, reg, data);
}

static int ov5640_reset(void)
{
    // Reset all registers
    int ret = ov5640_WR_Reg(SCCB_SYSTEM_CTRL_1, 0x11);
    ret |= ov5640_WR_Reg(SYSTEM_CTROL0, 0x82);

    // Delay 5 ms
    Camera_delay(5);

    // Write default regsiters
    for (int i = 0; ov5640_default_regs[i][0]; i++)
    {
        ret |= ov5640_WR_Reg((ov5640_default_regs[i][0] << 8) | (ov5640_default_regs[i][1] << 0), ov5640_default_regs[i][2]);
    }

    // Delay 300 ms
    Camera_delay(300);

    return ret;
}

// HTS (Horizontal Time) is the readout width plus the HSYNC_TIME time. However, if this value gets
// too low the OV5640 will crash. The minimum was determined empirically with testing...
// Additionally, when the image width gets too large we need to slow down the line transfer rate by
// increasing HTS so that DCMI_DMAConvCpltUser() can keep up with the data rate.
//
// WARNING! IF YOU CHANGE ANYTHING HERE RETEST WITH **ALL** RESOLUTIONS FOR THE AFFECTED MODE!
static int calculate_hts(uint16_t width)
{
    uint16_t hts = hts_target;

    if ((hcamera.pixformat == PIXFORMAT_GRAYSCALE) || (hcamera.pixformat == PIXFORMAT_BAYER) || (hcamera.pixformat == PIXFORMAT_JPEG))
    {
        if (width <= 1280)
            hts = IM_MAX((width * 2) + 8, hts_target);
    }
    else
    {
        if (width > 640)
            hts = IM_MAX((width * 2) + 8, hts_target);
    }

    return IM_MAX(hts + HSYNC_TIME, (SENSOR_WIDTH + HSYNC_TIME) / 2); // Fix to prevent crashing.
}

// VTS (Vertical Time) is the readout height plus the VYSNC_TIME time. However, if this value gets
// too low the OV5640 will crash. The minimum was determined empirically with testing...
//
// WARNING! IF YOU CHANGE ANYTHING HERE RETEST WITH **ALL** RESOLUTIONS FOR THE AFFECTED MODE!
static int calculate_vts(uint16_t readout_height)
{
    return IM_MAX(readout_height + VYSNC_TIME, (SENSOR_HEIGHT + VYSNC_TIME) / 8); // Fix to prevent crashing.
}

static int ov5640_set_pixformat(pixformat_t pixformat)
{
    uint8_t reg;
    int ret = 0;

    // Not a multiple of 8. The JPEG encoder on the OV5640 can't handle this.
    if ((pixformat == PIXFORMAT_JPEG) && ((dvp_cam_resolution[hcamera.framesize][0] % 8) || (dvp_cam_resolution[hcamera.framesize][1] % 8)))
    {
        return -1;
    }

    // Readout speed too fast. The DCMI_DMAConvCpltUser() line callback overhead is too much to handle the line transfer speed.
    // If we were to slow the pixclk down these resolutions would work. As of right now, the image shakes and scrolls with
    // the current line transfer speed. Note that there's an overhead to the DCMI_DMAConvCpltUser() function. It's not the
    // memory copy operation that's too slow. It's that there's too much overhead in the DCMI_DMAConvCpltUser() method
    // to even have time to start the line transfer. If it were possible to slow the line readout speed of the OV5640
    // this would enable these resolutions below. However, there's nothing in the datasheet that when modified does this.
    if (((pixformat == PIXFORMAT_GRAYSCALE) || (pixformat == PIXFORMAT_BAYER) || (pixformat == PIXFORMAT_JPEG)) && ((hcamera.framesize == FRAMESIZE_QQCIF) || (hcamera.framesize == FRAMESIZE_QQSIF) || (hcamera.framesize == FRAMESIZE_HQQQVGA) || (hcamera.framesize == FRAMESIZE_HQQVGA)))
    {
        return -1;
    }

    switch (pixformat)
    {
    case PIXFORMAT_GRAYSCALE:
        ret |= ov5640_WR_Reg(FORMAT_CONTROL, 0x10);
        ret |= ov5640_WR_Reg(FORMAT_CONTROL_MUX, 0x00);
        break;
    case PIXFORMAT_RGB565:
        ret |= ov5640_WR_Reg(FORMAT_CONTROL, 0x61);
        ret |= ov5640_WR_Reg(FORMAT_CONTROL_MUX, 0x01);
        break;
    case PIXFORMAT_YUV422:
        ret |= ov5640_WR_Reg(FORMAT_CONTROL, 0x30);
        ret |= ov5640_WR_Reg(FORMAT_CONTROL_MUX, 0x00);
        break;
    case PIXFORMAT_BAYER:
        ret |= ov5640_WR_Reg(FORMAT_CONTROL, 0x00);
        ret |= ov5640_WR_Reg(FORMAT_CONTROL_MUX, 0x01);
        break;
    case PIXFORMAT_JPEG:
        ret |= ov5640_WR_Reg(FORMAT_CONTROL, 0x30);
        ret |= ov5640_WR_Reg(FORMAT_CONTROL_MUX, 0x00);
        break;
    default:
        return -1;
    }

    ret |= ov5640_RD_Reg(TIMING_TC_REG_21, &reg);
    ret |= ov5640_WR_Reg(TIMING_TC_REG_21, (reg & 0xDF) | ((pixformat == PIXFORMAT_JPEG) ? 0x20 : 0x00));

    ret |= ov5640_RD_Reg(SYSTEM_RESET_02, &reg);
    ret |= ov5640_WR_Reg(SYSTEM_RESET_02, (reg & 0xE3) | ((pixformat == PIXFORMAT_JPEG) ? 0x00 : 0x1C));

    ret |= ov5640_RD_Reg(CLOCK_ENABLE_02, &reg);
    ret |= ov5640_WR_Reg(CLOCK_ENABLE_02, (reg & 0xD7) | ((pixformat == PIXFORMAT_JPEG) ? 0x28 : 0x00));

    if (hts_target)
    {
        uint16_t sensor_hts = calculate_hts(dvp_cam_resolution[hcamera.framesize][0]);

        ret |= ov5640_WR_Reg(TIMING_HTS_H, sensor_hts >> 8);
        ret |= ov5640_WR_Reg(TIMING_HTS_L, sensor_hts);
    }

    return ret;
}

static int ov5640_set_framesize(framesize_t framesize)
{
    uint8_t reg;
    int ret = 0;
    uint16_t w = dvp_cam_resolution[framesize][0];
    uint16_t h = dvp_cam_resolution[framesize][1];

    // Not a multiple of 8. The JPEG encoder on the OV5640 can't handle this.
    if ((hcamera.pixformat == PIXFORMAT_JPEG) && ((w % 8) || (h % 8)))
    {
        return -1;
    }

    // Readout speed too fast. The DCMI_DMAConvCpltUser() line callback overhead is too much to handle the line transfer speed.
    // If we were to slow the pixclk down these resolutions would work. As of right now, the image shakes and scrolls with
    // the current line transfer speed. Note that there's an overhead to the DCMI_DMAConvCpltUser() function. It's not the
    // memory copy operation that's too slow. It's that there's too much overhead in the DCMI_DMAConvCpltUser() method
    // to even have time to start the line transfer. If it were possible to slow the line readout speed of the OV5640
    // this would enable these resolutions below. However, there's nothing in the datasheet that when modified does this.
    if (((hcamera.pixformat == PIXFORMAT_GRAYSCALE) || (hcamera.pixformat == PIXFORMAT_BAYER) || (hcamera.pixformat == PIXFORMAT_JPEG)) && ((framesize == FRAMESIZE_QQCIF) || (framesize == FRAMESIZE_QQSIF) || (framesize == FRAMESIZE_HQQQVGA) || (framesize == FRAMESIZE_HQQVGA)))
    {
        return -1;
    }

    // Generally doesn't work for anything.
    if (framesize == FRAMESIZE_QQQQVGA)
    {
        return -1;
    }

    // Invalid resolution.
    if ((w > ACTIVE_SENSOR_WIDTH) || (h > ACTIVE_SENSOR_HEIGHT))
    {
        return -1;
    }

    // Step 0: Clamp readout settings.

    readout_w = IM_MAX(readout_w, w);
    readout_h = IM_MAX(readout_h, h);

    int readout_x_max = (ACTIVE_SENSOR_WIDTH - readout_w) / 2;
    int readout_y_max = (ACTIVE_SENSOR_HEIGHT - readout_h) / 2;
    readout_x = IM_MAX(IM_MIN(readout_x, readout_x_max), -readout_x_max);
    readout_y = IM_MAX(IM_MIN(readout_y, readout_y_max), -readout_y_max);

    // Step 1: Determine readout area and subsampling amount.

    uint16_t sensor_div = 0;

    if ((w > (readout_w / 2)) || (h > (readout_h / 2)))
    {
        sensor_div = 1;
    }
    else
    {
        sensor_div = 2;
    }

    // Step 2: Determine horizontal and vertical start and end points.

    uint16_t sensor_w = readout_w + DUMMY_WIDTH_BUFFER;  // camera hardware needs dummy pixels to sync
    uint16_t sensor_h = readout_h + DUMMY_HEIGHT_BUFFER; // camera hardware needs dummy lines to sync

    uint16_t sensor_ws = IM_MAX(IM_MIN((((ACTIVE_SENSOR_WIDTH - sensor_w) / 4) + (readout_x / 2)) * 2, ACTIVE_SENSOR_WIDTH - sensor_w), -(DUMMY_WIDTH_BUFFER / 2)) + DUMMY_COLUMNS; // must be multiple of 2
    uint16_t sensor_we = sensor_ws + sensor_w - 1;

    uint16_t sensor_hs = IM_MAX(IM_MIN((((ACTIVE_SENSOR_HEIGHT - sensor_h) / 4) - (readout_y / 2)) * 2, ACTIVE_SENSOR_HEIGHT - sensor_h), -(DUMMY_HEIGHT_BUFFER / 2)) + DUMMY_LINES; // must be multiple of 2
    uint16_t sensor_he = sensor_hs + sensor_h - 1;

    // Step 3: Determine scaling window offset.

    float ratio = IM_MIN((readout_w / sensor_div) / ((float)w), (readout_h / sensor_div) / ((float)h));

    uint16_t w_mul = w * ratio;
    uint16_t h_mul = h * ratio;
    uint16_t x_off = ((sensor_w / sensor_div) - w_mul) / 2;
    uint16_t y_off = ((sensor_h / sensor_div) - h_mul) / 2;

    // Step 4: Compute total frame time.

    hts_target = sensor_w / sensor_div;

    uint16_t sensor_hts = calculate_hts(w);
    uint16_t sensor_vts = calculate_vts(sensor_h / sensor_div);

    uint16_t sensor_x_inc = (((sensor_div * 2) - 1) << 4) | (1 << 0); // odd[7:4]/even[3:0] pixel inc on the bayer pattern
    uint16_t sensor_y_inc = (((sensor_div * 2) - 1) << 4) | (1 << 0); // odd[7:4]/even[3:0] pixel inc on the bayer pattern

    // Step 5: Write regs.

    ret |= ov5640_WR_Reg(TIMING_HS_H, sensor_ws >> 8);
    ret |= ov5640_WR_Reg(TIMING_HS_L, sensor_ws);

    ret |= ov5640_WR_Reg(TIMING_VS_H, sensor_hs >> 8);
    ret |= ov5640_WR_Reg(TIMING_VS_L, sensor_hs);

    ret |= ov5640_WR_Reg(TIMING_HW_H, sensor_we >> 8);
    ret |= ov5640_WR_Reg(TIMING_HW_L, sensor_we);

    ret |= ov5640_WR_Reg(TIMING_VH_H, sensor_he >> 8);
    ret |= ov5640_WR_Reg(TIMING_VH_L, sensor_he);

    ret |= ov5640_WR_Reg(TIMING_DVPHO_H, w >> 8);
    ret |= ov5640_WR_Reg(TIMING_DVPHO_L, w);

    ret |= ov5640_WR_Reg(TIMING_DVPVO_H, h >> 8);
    ret |= ov5640_WR_Reg(TIMING_DVPVO_L, h);

    ret |= ov5640_WR_Reg(TIMING_HTS_H, sensor_hts >> 8);
    ret |= ov5640_WR_Reg(TIMING_HTS_L, sensor_hts);

    ret |= ov5640_WR_Reg(TIMING_VTS_H, sensor_vts >> 8);
    ret |= ov5640_WR_Reg(TIMING_VTS_L, sensor_vts);

    ret |= ov5640_WR_Reg(TIMING_HOFFSET_H, x_off >> 8);
    ret |= ov5640_WR_Reg(TIMING_HOFFSET_L, x_off);

    ret |= ov5640_WR_Reg(TIMING_VOFFSET_H, y_off >> 8);
    ret |= ov5640_WR_Reg(TIMING_VOFFSET_L, y_off);

    ret |= ov5640_WR_Reg(TIMING_X_INC, sensor_x_inc);
    ret |= ov5640_WR_Reg(TIMING_Y_INC, sensor_y_inc);

    ret |= ov5640_RD_Reg(TIMING_TC_REG_20, &reg);
    ret |= ov5640_WR_Reg(TIMING_TC_REG_20, (reg & 0xFE) | (sensor_div > 1));

    ret |= ov5640_RD_Reg(TIMING_TC_REG_21, &reg);
    ret |= ov5640_WR_Reg(TIMING_TC_REG_21, (reg & 0xFE) | (sensor_div > 1));

    ret |= ov5640_WR_Reg(VFIFO_HSIZE_H, w >> 8);
    ret |= ov5640_WR_Reg(VFIFO_HSIZE_L, w);

    ret |= ov5640_WR_Reg(VFIFO_VSIZE_H, h >> 8);
    ret |= ov5640_WR_Reg(VFIFO_VSIZE_L, h);

    return ret;
}

static int ov5640_set_hmirror(int enable)
{
    uint8_t reg;
    int ret = ov5640_RD_Reg(TIMING_TC_REG_21, &reg);
    if (enable)
    {
        ret |= ov5640_WR_Reg(TIMING_TC_REG_21, reg | 0x06);
    }
    else
    {
        ret |= ov5640_WR_Reg(TIMING_TC_REG_21, reg & 0xF9);
    }
    return ret;
}

static int ov5640_set_vflip(int enable)
{
    uint8_t reg;
    int ret = ov5640_RD_Reg(TIMING_TC_REG_20, &reg);
    if (!enable)
    {
        ret |= ov5640_WR_Reg(TIMING_TC_REG_20, reg | 0x06);
    }
    else
    {
        ret |= ov5640_WR_Reg(TIMING_TC_REG_20, reg & 0xF9);
    }
    return ret;
}

int ov5640_init(framesize_t framesize)
{
    ov5640_reset();
    hcamera.framesize = framesize;
    hcamera.pixformat = PIXFORMAT_RGB565;
    ov5640_set_pixformat(hcamera.pixformat);
    ov5640_set_framesize(hcamera.framesize);
    ov5640_set_hmirror(0);
    ov5640_set_vflip(0);

    return 1;
}
