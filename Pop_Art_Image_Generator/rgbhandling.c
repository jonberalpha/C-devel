/******************************************************************************/
/* Filename:      rgbhandling.c                                               */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 28.11.2020                                                  */
/* Description:   Contains changeRGBValues(), assignByteToRGB()               */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <main.h>
#include <ppmp6io.h>
#include <rgbhandling.h>

/************************** PRIVATE PROTOTYPES ********************************/
static int assignByteToRGB(char currgb, int j, PPMP6Data **picaddr, PPMP6Data **pimg);

/******************************************************************************/
/*            S U B R O U T I N E :     changeRGBValues()                     */
/*----------------------------------------------------------------------------*/
/* Purpose:      Takes given original image and creates new image according   */
/*               given RGB-pattern                                            */
/* Input(s):     (char *)       - String that contains RGB-pattern            */
/*               (PPMP6Image *) - Pointer of image that should be changed     */
/* Return value: (PPMP6Image *) - Returns pointer of changed image            */
/******************************************************************************/
PPMP6Image *arrangeRGBValues(char *rgbstring, PPMP6Image *imgorig)
{
  char *token = NULL;
  int k = 0;
  PPMP6Image *imgnew = NULL;
  PPMP6Data *pimgorigdata1 = NULL, *pimgnewdata = NULL;
  PPMP6Data *pic1addr = NULL, *pic2addr = NULL, *pic3addr = NULL, *pic4addr = NULL;
  char pic[4][4];
  char *retstrcpy = NULL;
  char origpic[] = "rgb";
  int i = 0, j = 0, retas = -1;
  unsigned int m = 0, anz_pix_line = 0;

  token = strtok(rgbstring, ":");
  while(token != NULL)
  {
    retstrcpy = strncpy(pic[k], token, RGB_PACKAGE * sizeof(char));
    if(retstrcpy != pic[k])
    {
      printf(ANSI_COLOR_RED "ERROR! strncpy() of the %d. rgb-pattern failed!\n" ANSI_COLOR_RESET, k + 1);
      exit(EXIT_FAILURE);
    }
    token = strtok(NULL, ":");
    k++;
  }

  // allocate memory for new image
  imgnew = (PPMP6Image *)malloc(sizeof(PPMP6Image));
  if(imgnew == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  imgnew->signature[0] = imgorig->signature[0];
  imgnew->signature[1] = imgorig->signature[1];
  imgnew->colordepth = imgorig->colordepth;
  imgnew->xlength = (POP_ART_SIZE/2) * imgorig->xlength;
  imgnew->ylength = (POP_ART_SIZE/2) * imgorig->ylength;

  // allocate memory for image data four times the size of original image
  imgnew->data = (PPMP6Data *)malloc(imgnew->xlength * imgnew->ylength * sizeof(PPMP6Data));
  if(imgnew->data == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  pimgnewdata = imgnew->data; // set pointer on new image data
  pic1addr = pimgnewdata;

  for(m = 0; m < (imgorig->xlength * imgorig->ylength); m++) // size of origimage
  {
    pimgorigdata1 = imgorig->data + m;

    if(anz_pix_line == imgorig->xlength)
    {
      pic1addr = pic1addr + (imgorig->xlength);
      anz_pix_line = 0;
    }
    // set pointer to sub-images
    pic2addr = pic1addr + imgorig->xlength;
    pic3addr = pic1addr + (imgorig->xlength * (POP_ART_SIZE/2) * imgorig->ylength);
    pic4addr = pic3addr + imgorig->xlength;
    for(i = 0; i < 3; i++)
    {
      for(j = 0; j < 3; j++)
      {
        if(pic[0][j] == origpic[i])
        {
          retas = assignByteToRGB(origpic[i], j, &pic1addr, &pimgorigdata1);
          if(retas < 0)
          {
            printf(ANSI_COLOR_RED "ERROR! assignByteToRGB() failed!\n" ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
          }
        }
        if(pic[1][j] == origpic[i])
        {
          retas = assignByteToRGB(origpic[i], j, &pic2addr, &pimgorigdata1);
          if(retas < 0)
          {
            printf(ANSI_COLOR_RED "ERROR! assignByteToRGB() failed!\n" ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
          }
        }
        if(pic[2][j] == origpic[i])
        {
          retas = assignByteToRGB(origpic[i], j, &pic3addr, &pimgorigdata1);
          if(retas < 0)
          {
            printf(ANSI_COLOR_RED "ERROR! assignByteToRGB() failed!\n" ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
          }
        }
        if(pic[3][j] == origpic[i])
        {
          retas = assignByteToRGB(origpic[i], j, &pic4addr, &pimgorigdata1);
          if(retas < 0)
          {
            printf(ANSI_COLOR_RED "ERROR! assignByteToRGB() failed!\n" ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    pic1addr = pic1addr + 1;
    anz_pix_line = anz_pix_line + 1;
  }
  
  return imgnew;
}

/******************************************************************************/
/*            S U B R O U T I N E :     assignByteToRGB()                     */
/*----------------------------------------------------------------------------*/
/* Purpose:      Assigns Data-Byte to the right RGB-Value                     */
/* Input(s):     (char)        - Current reference-RGB-character              */
/*               (int)         - Loop-Postion                                 */
/*               (PPMP6Data *) - Pointer-position of subimage-address         */
/*               (PPMP6Data *) - Pointer-position of reference-image-address  */
/* Return value: (int)         - Returns 0 on success and -1 on failure       */
/******************************************************************************/
static int assignByteToRGB(char currgb, int pos, PPMP6Data **picaddr, PPMP6Data **pimg)
{
  int ret = -1;

  switch(currgb)
  {
    case 'r':
    {
      switch(pos)
      {
        case 0: { (*picaddr)->red = (*pimg)->red; ret = 0; break; }
        case 1: { (*picaddr)->green = (*pimg)->red; ret = 0; break; }
        case 2: { (*picaddr)->blue = (*pimg)->red; ret = 0; break; }
      }
      break;
    }
    case 'g':
    {
      switch(pos)
      {
        case 0: { (*picaddr)->red = (*pimg)->green; ret = 0; break; }
        case 1: { (*picaddr)->green = (*pimg)->green; ret = 0; break; }
        case 2: { (*picaddr)->blue = (*pimg)->green; ret = 0; break; }
      }
      break;
    }
    case 'b':
    {
      switch(pos)
      {
        case 0: { (*picaddr)->red = (*pimg)->blue; ret = 0; break; }
        case 1: { (*picaddr)->green = (*pimg)->blue; ret = 0; break; }
        case 2: { (*picaddr)->blue = (*pimg)->blue; ret = 0; break; }
      }
      break;
    }
    default:
    {
      ret = -1; // if given character is no r,g or b
      break;
    }
  }

  return ret;
}

