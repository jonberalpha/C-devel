/******************************************************************************/
/* Filename:      ppmp6io.h                                                   */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 25.11.2020                                                  */
/* Description:   Header-File for ppmp6io.c                                   */
/******************************************************************************/
#ifndef _ppmp6io_h_
#define _ppmp6io_h_

/******************************** DEFINES *************************************/
#define RGB_PACKAGE         3
#define CREATOR_SIGNATURE   "JONAS BERGER"

/******************************* STRUCTURES ***********************************/
typedef struct PPMP6Data
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} PPMP6Data;

typedef struct PPMP6Image
{
  char signature[2];          // e.g. P6
  unsigned int xlength;       // e.g. 800px
  unsigned int ylength;       // e.g. 400px
  unsigned int colordepth;    // e.g. 255
  PPMP6Data *data;            // contains image data
} PPMP6Image;

/******************************* PROTOTYPES ***********************************/
PPMP6Image *readPPMP6(char *inputimgname);
int writePPMP6(char *outputimgname, PPMP6Image *wimg);

#endif

