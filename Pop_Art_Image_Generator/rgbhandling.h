/******************************************************************************/
/* Filename:      rgbhandling.h                                               */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 28.11.2020                                                  */
/* Description:   Header-File for rgbhandling.c                               */
/******************************************************************************/
#ifndef _rgbhandling_h_
#define _rgbhandling_h_

/******************************** DEFINES *************************************/
#define POP_ART_SIZE 4

/******************************* PROTOTYPES ***********************************/
PPMP6Image *arrangeRGBValues(char *rgbstring, PPMP6Image *img);

#endif

