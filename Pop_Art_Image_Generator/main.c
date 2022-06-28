/******************************************************************************/
/* Task:          Pop-Art Image Generator (Medium Task)                       */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 25.11.2020                                                  */
/* Description:   Creates Pop-Art Image from given ppm-image                  */
/*                Compatible format: ppm (P6/Binary)-Format                   */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <main.h>
#include <checkprogargs.h>
#include <ppmp6io.h>
#include <rgbhandling.h>

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  // Declarations
  InputArgs userin;
  char *retstrcpy = NULL; // strcpy return-check
  int isvalidinputs = -1, retwr = -1;
  PPMP6Image *imageorig = NULL, *imagenew = NULL;

  // check count of arguments
  if(argc == 4) // when InputImg, OutputImg, rgb-string given
  {
    // copy arguments in structure
    retstrcpy = strncpy(userin.inputimgname, argv[1], sizeof(userin.inputimgname));
    if(retstrcpy != userin.inputimgname)
    {
      printf(ANSI_COLOR_RED "ERROR! strncpy() of first argument failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    retstrcpy = strncpy(userin.outputimgname, argv[2], sizeof(userin.outputimgname));
    if(retstrcpy != userin.outputimgname)
    {
      printf(ANSI_COLOR_RED "ERROR! strncpy() of second argument failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    retstrcpy = strncpy(userin.rgbstring, argv[3], sizeof(userin.rgbstring));
    if(retstrcpy != userin.rgbstring)
    {
      printf(ANSI_COLOR_RED "ERROR! strncpy() of third argument failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    // check if program-args are valid
    isvalidinputs = checkProgArgs(&userin);
    if(isvalidinputs < 0)
    {
      printf(ANSI_COLOR_RED "ERROR! No valid program arguments!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    // Read from image
    imageorig = readPPMP6(userin.inputimgname);

    // arrange rgb-values
    imagenew = arrangeRGBValues(userin.rgbstring, imageorig);

    // free memory of original image
    if(imageorig->data != NULL)
    {
      free(imageorig->data);
    }
    imageorig->data = NULL;
    if(imageorig != NULL)
    {
      free(imageorig);
    }
    imageorig = NULL;

    // write image to file
    retwr = writePPMP6(userin.outputimgname, imagenew);
    if(retwr != 0)
    {
      printf(ANSI_COLOR_RED "ERROR! Function writePPMP6() failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    // free memory from new image
    if(imagenew->data != NULL)
    {
      free(imagenew->data);
    }
    imagenew->data = NULL;
    if(imagenew != NULL)
    {
      free(imagenew);
    }
    imagenew = NULL;

    printf(ANSI_COLOR_GREEN "\nPop-art image successfully generated!\n\n" ANSI_COLOR_RESET);

  }
  else // when too few or too many arguments given
  {
    // error
    printf(ANSI_COLOR_RED "ERROR! Check program arguments!\n" ANSI_COLOR_RESET);
    printf("Usage-Example:\n");
    printf("Unix:    ./popgen input.ppm outputpic.ppm rgb:rbg:grb:gbr\n");
    printf("Windows: popgen.exe inputpic.ppm outputpic.ppm rgb:rbg:grb:gbr\n");
    exit(EXIT_FAILURE); // terminate
  }

  return 0;
}

