/******************************************************************************/
/* Filename:      ppmp6io.c                                                   */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 19.12.2020                                                  */
/* Description:   Contains readPPMP6(), writePPMP6()                          */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdlib.h>
#include <stdio.h>

#include <main.h>
#include <ppmp6io.h>

/******************************************************************************/
/*            S U B R O U T I N E :     readPPMP6()                           */
/*----------------------------------------------------------------------------*/
/* Purpose:      Reads from an image-file and returns a pointer               */ 
/*               on created image-structure                                   */
/* Input(s):     (char *)       - String that contains the imagename          */
/* Return value: (PPMP6Image *) - Returns pointer of read image-data in memory*/
/******************************************************************************/
PPMP6Image *readPPMP6(char *inputimgname)
{
  FILE *ifp = NULL;
  PPMP6Image *rimg = NULL;
  char buffer[16] = "";
  char *retfg = NULL;
  int ch = -1;
  int retgc = -1, retfs = -1, retug = -1, retfc = -1;
  size_t retfr = -1;

  // open file
  ifp = fopen(inputimgname, "rb");
  if(ifp == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! File cannot be opened!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // read image format
  retfg = fgets(buffer, sizeof(buffer), ifp);
  if(retfg == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot read from file!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  // check image format
  if((buffer[0] != 'P') || (buffer[1] != '6'))
  {
    printf(ANSI_COLOR_RED "ERROR! Invalid image format (not 'P6')\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // allocate memory for image
  rimg = (PPMP6Image *)malloc(sizeof(PPMP6Image));
  if(rimg == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // set signature
  rimg->signature[0] = buffer[0];  // P
  rimg->signature[1] = buffer[1];  // 6

  // check if comments are present, getc cause undefined line length
  ch = getc(ifp);
  while(ch == '#')
  {
    do
    {
      retgc = getc(ifp);
    }
    while(retgc != '\n');
    ch = getc(ifp);
  }
  retug = ungetc(ch, ifp); // put read char in file stream
  if(retug == EOF)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot put back character in file stream!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // read size of given image
  retfs = fscanf(ifp, "%u %u", &rimg->xlength, &rimg->ylength);
  if(retfs != 2)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot determine image size!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  //read color depth
  retfs = fscanf(ifp, "%u", &rimg->colordepth);
  if(retfs != 1)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot determine color depth!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  do // check for \n
  {
    ch = fgetc(ifp);
  }
  while(ch != '\n');

  // allocate memory for image data
  rimg->data = (PPMP6Data *)malloc(rimg->xlength * rimg->ylength * sizeof(PPMP6Data));
  if(rimg->data == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // read all pixel data
  retfr = fread(rimg->data, RGB_PACKAGE * (rimg->xlength), rimg->ylength, ifp);
  if(retfr != (size_t)(rimg->ylength))
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot read from file!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  // close file
  if(ifp != NULL)
  {
    retfc = fclose(ifp);
    if(retfc != 0)
    {
      printf(ANSI_COLOR_RED "ERROR! Cannot close file!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
  }

  return rimg;
}

/******************************************************************************/
/*            S U B R O U T I N E :     writePPMP6()                          */
/*----------------------------------------------------------------------------*/
/* Purpose:      Writes image-data to created PPM-file                        */
/* Input(s):     (char *)       - String that contains the file-name          */
/*               (PPMP6Image *) - Pointer on image to write                   */
/* Return value: (int)          - Returns 0 on success and a negative number  */
/*                                on failure                                  */
/******************************************************************************/
int writePPMP6(char *outputimgname, PPMP6Image *wimg)
{
  FILE *ofp = NULL;
  int retwr = -1, retfp = -1, retfc = -1;
  size_t retfw = -1;

  //open file for output
  ofp = fopen(outputimgname, "wb");
  if(ofp == NULL)
  {
    retwr = -2;
  }

  // Insert header
  retfp = fprintf(ofp, "%c%c\n", wimg->signature[0], wimg->signature[1]);
  if(retfp < 0)
  {
    retwr = -3;
  }
  retfp = fprintf(ofp, "# CREATED BY %s\n", CREATOR_SIGNATURE);
  if(retfp < 0)
  {
    retwr = -4;
  }
  retfp = fprintf(ofp, "%d %d\n", wimg->xlength, wimg->ylength);
  if(retfp < 0)
  {
    retwr = -5;
  }
  retfp = fprintf(ofp, "%d\n", wimg->colordepth);
  if(retfp < 0)
  {
    retwr = -6;
  }

  // Image data
  retfw = fwrite(wimg->data, RGB_PACKAGE * (wimg->xlength), wimg->ylength, ofp);
  if(retfw < (size_t)(wimg->ylength))
  {
    retwr = -7;
  }

  if(ofp != NULL)
  {
    retfc = fclose(ofp);
    if(retfc != 0)
    {
      retwr = -8;
    }
    retwr = 0;
  }

  return retwr;
}

