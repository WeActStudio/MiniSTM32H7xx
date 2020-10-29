/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2019 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2019 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Image library configuration.
 */
#ifndef __IMLIB_CONFIG_H__
#define __IMLIB_CONFIG_H__

// Enable LAB LUT
#define IMLIB_ENABLE_LAB_LUT

// Enable YUV LUT
//#define IMLIB_ENABLE_YUV_LUT

// Enable mean pooling
#define IMLIB_ENABLE_MEAN_POOLING

// Enable midpoint pooling
#define IMLIB_ENABLE_MIDPOINT_POOLING

// Enable binary ops
#define IMLIB_ENABLE_BINARY_OPS

// Enable math ops
#define IMLIB_ENABLE_MATH_OPS

// Enable flood_fill()
#define IMLIB_ENABLE_FLOOD_FILL

// Enable mean()
#define IMLIB_ENABLE_MEAN

// Enable median()
#define IMLIB_ENABLE_MEDIAN

// Enable mode()
#define IMLIB_ENABLE_MODE

// Enable midpoint()
#define IMLIB_ENABLE_MIDPOINT

// Enable morph()
#define IMLIB_ENABLE_MORPH

// Enable Gaussian
#define IMLIB_ENABLE_GAUSSIAN

// Enable Laplacian
#define IMLIB_ENABLE_LAPLACIAN

// Enable bilateral()
#define IMLIB_ENABLE_BILATERAL

// Enable cartoon()
// #define IMLIB_ENABLE_CARTOON

// Enable remove_shadows()
// #define IMLIB_ENABLE_REMOVE_SHADOWS

// Enable linpolar()
#define IMLIB_ENABLE_LINPOLAR

// Enable logpolar()
#define IMLIB_ENABLE_LOGPOLAR

// Enable chrominvar()
// #define IMLIB_ENABLE_CHROMINVAR

// Enable illuminvar()
// #define IMLIB_ENABLE_ILLUMINVAR

// Enable invariant table
//#define IMLIB_ENABLE_INVARIANT_TABLE

// Enable lens_corr()
#define IMLIB_ENABLE_LENS_CORR

// Enable rotation_corr()
#define IMLIB_ENABLE_ROTATION_CORR

// Enable phasecorrelate()
#define IMLIB_ENABLE_FIND_DISPLACEMENT

// rotation_corr() is required by phasecorrelate()
#if defined(IMLIB_ENABLE_FIND_DISPLACEMENT)\
    && !defined(IMLIB_ENABLE_ROTATION_CORR)
    #define IMLIB_ENABLE_ROTATION_CORR
#endif

// Enable get_similarity()
#define IMLIB_ENABLE_GET_SIMILARITY

// Enable find_lines()
#define IMLIB_ENABLE_FIND_LINES

// Enable find_line_segments()
#define IMLIB_ENABLE_FIND_LINE_SEGMENTS

// find_lines() is required by the old find_line_segments()
#if defined(IMLIB_ENABLE_FIND_LINE_SEGMENTS)\
    && !defined(IMLIB_ENABLE_FIND_LINES)
    #define IMLIB_ENABLE_FIND_LINES
#endif

// Enable find_circles()
#define IMLIB_ENABLE_FIND_CIRCLES

// Enable find_rects()
#define IMLIB_ENABLE_FIND_RECTS

// Enable find_qrcodes() (14 KB)
#define IMLIB_ENABLE_QRCODES

// Enable find_apriltags() (64 KB)
#define IMLIB_ENABLE_APRILTAGS

// Enable fine find_apriltags() - (8-way connectivity versus 4-way connectivity)
// #define IMLIB_ENABLE_FINE_APRILTAGS

// Enable high res find_apriltags() - uses more RAM
// #define IMLIB_ENABLE_HIGH_RES_APRILTAGS

// Enable find_datamatrices() (26 KB)
#define IMLIB_ENABLE_DATAMATRICES

// Enable find_barcodes() (42 KB)
#define IMLIB_ENABLE_BARCODES

// Enable CMSIS NN
// #if !defined(CUBEAI)
// #define IMLIB_ENABLE_CNN
// #endif

// Enable Tensor Flow
#if !defined(CUBEAI)
#define IMLIB_ENABLE_TF
#endif

// Enable FAST (20+ KBs).
#define IMLIB_ENABLE_FAST

// Enable find_template()
#define IMLIB_FIND_TEMPLATE

// Enable find_lbp()
#define IMLIB_ENABLE_FIND_LBP

// Enable find_keypoints()
#define IMLIB_ENABLE_FIND_KEYPOINTS

#if defined(IMLIB_ENABLE_FIND_LBP) || defined(IMLIB_ENABLE_FIND_KEYPOINTS)
    #define IMLIB_ENABLE_DESCRIPTOR
#endif

// Enable find_hog()
#define IMLIB_ENABLE_HOG

// Enable selective_search()
#define IMLIB_ENABLE_SELECTIVE_SEARCH

#endif //__IMLIB_CONFIG_H__
