/******************************************************************************/
/* Filename:      imgfilter.c                                                 */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 19.12.2020                                                  */
/* Description:   Contains applyKernelFilter(), inPix(), outPix(), getPixel() */
/*                setPixel()                                                  */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <main.h>
#include <ppmp6io.h>
#include <imgfilter.h>

/************************** PRIVATE PROTOTYPES ********************************/
static double inPix(int rgb, long int x, long int y, PPMP6Image **img);
static int outPix(long int x, long int y, PPMP6Image **img, double rgbvalues[]);
static PPMP6Data getPixel(long int x, long int y, PPMP6Image ***img);
static int setPixel(long int x, long int y, PPMP6Image ***img, PPMP6Data *pix);

/******************************************************************************/
/*            S U B R O U T I N E :     applyKernelFilter()                   */
/*----------------------------------------------------------------------------*/
/* Purpose:      Takes given image and creates new filtered image according   */
/*               to given kernel                                              */
/* Input(s):     (char *)       - String that contains kernel-matrix          */
/*               (PPMP6Image *) - Pointer of original image                   */
/* Return value: (PPMP6Image *) - Returns pointer of filtered image           */
/******************************************************************************/
PPMP6Image *applyKernelFilter(const char *kernel, PPMP6Image *imgorig)
{
  PPMP6Image *imgnew = NULL;
  double kernel_matrix[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  size_t j = 0;
  int i = 0, retop = 0, rgb = 0;
  double f = 0;
  long int x = 0, y = 0;
  double rgbvalues[] = {0, 0, 0};
  
  char *endptr;
  char *p = NULL, *p_o = NULL;
  
  p = strdupl(kernel);
  if(p == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Unable to copy string!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  p_o = p; // remember p for deallocating memory

  // change commas to spaces in preperation for strtod()
  for(j = 0; j < strlen(p); j++)
  {
    if(p[j] == ',')
    {
      p[j] = ' ';
    }
  }

  // convert kernel-string to kernel-matrix-double-array
  i = 0;
  for(f = strtod(p, &endptr); p != endptr; f = strtod(p, &endptr))
  {
    p = endptr;
    if(i < 9)
    {
      kernel_matrix[i] = f;
    }
    else
    {
      printf(ANSI_COLOR_RED "ERROR! Converting given kernel-string failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    i = i + 1;
  }
  
  /***** free memory of helper string *****/
  if(p_o != NULL)
  {
    free(p_o);
  }
  p_o = NULL;
  p = NULL;

  // allocate memory for new image
  imgnew = (PPMP6Image *)malloc(sizeof(PPMP6Image));
  if(imgnew == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory for filtered image!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // Set header of new image
  imgnew->signature[0] = imgorig->signature[0];
  imgnew->signature[1] = imgorig->signature[1];
  imgnew->colordepth = imgorig->colordepth;
  imgnew->xlength = imgorig->xlength;
  imgnew->ylength = imgorig->ylength;

  // allocate memory for image data (same size as original image)
  imgnew->data = (PPMP6Data *)malloc(imgnew->xlength * imgnew->ylength * sizeof(PPMP6Data));
  if(imgnew->data == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory for pixels!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // apply kernel-filter
  for(x = 0; x < imgnew->xlength; x++)
  {
    for(y = 0; y < imgnew->ylength; y++)
    {
      for(rgb = 0; rgb < 3; rgb++)
      {
        rgbvalues[rgb] = inPix(rgb, x-1, y-1, &imgorig) * kernel_matrix[0] +
                         inPix(rgb, x, y-1, &imgorig)   * kernel_matrix[1] +
                         inPix(rgb, x+1, y-1, &imgorig) * kernel_matrix[2] +
                         inPix(rgb, x-1, y, &imgorig)   * kernel_matrix[3] +
                         inPix(rgb, x, y, &imgorig)     * kernel_matrix[4] +
                         inPix(rgb, x+1, y, &imgorig)   * kernel_matrix[5] +
                         inPix(rgb, x-1, y+1, &imgorig) * kernel_matrix[6] +
                         inPix(rgb, x, y+1, &imgorig)   * kernel_matrix[7] +
                         inPix(rgb, x+1, y+1, &imgorig) * kernel_matrix[8];
      }
      retop = outPix(x, y, &imgnew, rgbvalues);
      if(retop < 0)
      {
        printf(ANSI_COLOR_RED "ERROR! Writening pixel to image failed!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
    }
  }
  
  return imgnew;
}

/******************************************************************************/
/*            S U B R O U T I N E :     inPix()                               */
/*----------------------------------------------------------------------------*/
/* Purpose:      Returns rgbvalue according to given rgb and image            */
/* Input(s):     (int)           - RGB-selection: 0-red, 1-green, 2-blue      */
/*               (long int)      - x coordinates                              */
/*               (long int)      - y coordinates                              */
/*               (PPMP6Image **) - Pointer of reference-image-address         */
/* Return value: (double)        - Returns rgb value of selected pixel        */
/******************************************************************************/
static double inPix(int rgb, long int x, long int y, PPMP6Image **img)
{
  double ret = 0;
  PPMP6Data pix = {0, 0, 0};
  
  pix = getPixel(x, y, &img);
  switch(rgb)
  {
    case 0: { ret = pix.red / 255.0; break; }
    case 1: { ret = pix.green / 255.0; break; }
    case 2: { ret = pix.blue / 255.0; break; }
    default: { ret = 0; break; }
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     outPix()                              */
/*----------------------------------------------------------------------------*/
/* Purpose:      Sets rgbvalue according to given rgb and value               */
/* Input(s):     (long int)      - x coordinates                              */
/*               (long int)      - y coordinates                              */
/*               (PPMP6Image **) - Pointer of image-adress to be written      */
/*               (double)        - rgbvalue to write (0 - 255)                */
/* Return value: (int)           - Returns 0 on success and a negative        */
/*                                 number on failure                          */
/******************************************************************************/
static int outPix(long int x, long int y, PPMP6Image **img, double rgbvalues[])
{
  int ret = 0;
  PPMP6Data pix = {0, 0, 0};
  double value = 0;
  int i = 0;
  
  for(i = 0; i < 3; i++) // loop for 3 rgb values
  {
    value = round(rgbvalues[i] * 255.0);
    if(value <= 0) // if < 0 than 0
    {
      switch(i)
      {
        case 0: {pix.red = 0; break;};
        case 1: {pix.green = 0; break;};
        case 2: {pix.blue = 0; break;};
      }
    }
    else
    {
      if(value >= 255) // if > 255 than 255
      {
        switch(i)
        {
          case 0: {pix.red = 255; break;};
          case 1: {pix.green = 255; break;};
          case 2: {pix.blue = 255; break;};
        }
      }
      else // else write calculated value
      {
        switch(i)
        {
          case 0: {pix.red = (unsigned char)value; break;};
          case 1: {pix.green = (unsigned char)value; break;};
          case 2: {pix.blue = (unsigned char)value; break;};
        }
      }
    }
  }
  
  ret = setPixel(x, y, &img, &pix);
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     getPixel()                            */
/*----------------------------------------------------------------------------*/
/* Purpose:      Returns pixel according to given coordinates                 */
/* Input(s):     (long int)       - x coordinates                             */
/*               (long int)       - y coordinates                             */
/*               (PPMP6Image ***) - Pointer of reference-image-address        */
/* Return value: (PPMP6Data)      - Returns selected pixel                    */
/******************************************************************************/
static PPMP6Data getPixel(long int x, long int y, PPMP6Image ***img)
{
  PPMP6Data retpix = {0, 0, 0};
  PPMP6Data *p = NULL;
  
  p = (**img)->data;
  
  if((x >= 0) && (y >= 0) && (x < (**img)->xlength) && (y < (**img)->ylength))
  {
    p = p + (y * ((**img)->xlength) + x);
  
    retpix.red = p->red;
    retpix.green = p->green;
    retpix.blue = p->blue;
  }
  else // if outranged than assume 0 as rgb values
  {
    retpix.red = 0;
    retpix.green = 0;
    retpix.blue = 0;
  }
  
  p = NULL;
  
  return retpix;
}

/******************************************************************************/
/*            S U B R O U T I N E :     setPixel()                            */
/*----------------------------------------------------------------------------*/
/* Purpose:      Set pixel according to given coordinates and pixel-rgbvalues */
/* Input(s):     (long int)       - x coordinates                             */
/*               (long int)       - y coordinates                             */
/*               (PPMP6Image ***) - Pointer of image to write                 */
/*               (PPMP6Data **)   - Pointer of pixel to write                 */
/* Return value: (int)            - Returns 0 on success and a negative       */
/*                                  number on failure                         */
/******************************************************************************/
static int setPixel(long int x, long int y, PPMP6Image ***img, PPMP6Data *pix)
{
  PPMP6Data *p = NULL;
  int ret = 0;
  
  p = (**img)->data;
  
  if((x >= 0) && (y >= 0) && (x < (**img)->xlength) && (y < (**img)->ylength))
  {
    p = p + (y * ((**img)->xlength) + x);
  
    p->red = pix->red;
    p->green = pix->green;
    p->blue = pix->blue;
  }
  else
  {
    ret = -1;
  }
  
  p = NULL;
  
  return ret;
}

