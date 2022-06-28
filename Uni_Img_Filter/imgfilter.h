/******************************************************************************/
/* Filename:      imgfilter.h                                                 */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 19.12.2020                                                  */
/* Description:   Header-File for imgfilter.c                                 */
/******************************************************************************/
#ifndef _imgfilter_h_
#define _imgfilter_h_

/******************************** DEFINES *************************************/
#define COEFF_SIZE 9

/******************************* PROTOTYPES ***********************************/
PPMP6Image *applyKernelFilter(const char *kernel, PPMP6Image *img);

#endif

