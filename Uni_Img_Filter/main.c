/******************************************************************************/
/* Task:          Universal Image Filter (Hard Task)                          */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 19.12.2020                                                  */
/* Description:   Creates Kernel-filtered image from given ppm-image          */
/*                Compatible format: ppm (P6/Binary)-Format                   */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <main.h>
#include <checkprogargs.h>
#include <ppmp6io.h>
#include <imgfilter.h>

/*************************** PRIVATE PROTOTYPES *******************************/
static void printUsageMsg(void);
static void freeCopiedProgArgs(InputArgs *input);

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  // Declarations
  InputArgs userin = {NULL, NULL, NULL};
  int isvalidinputs = -1, retwr = -1;
  PPMP6Image *imageorig = NULL, *imagenew = NULL;

  // check count of arguments
  if(argc == 4) // when InputImg, OutputImg and kernel given
  {
    // initialize arguments in structure
    userin.inputimgname = strdupl(argv[1]);
    if(userin.inputimgname == NULL)
    {
      printf(ANSI_COLOR_RED "ERROR! Unable to copy program arguments!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    userin.outputimgname = strdupl(argv[2]);
    if(userin.outputimgname == NULL)
    {
      printf(ANSI_COLOR_RED "ERROR! Unable to copy program arguments!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    userin.kernel = strdupl(argv[3]);
    if(userin.kernel == NULL)
    {
      printf(ANSI_COLOR_RED "ERROR! Unable to copy program arguments!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    
    // check if program-args are valid
    isvalidinputs = checkProgArgs(&userin);
    if(isvalidinputs < 0)
    {
      printf(ANSI_COLOR_RED "ERROR! No valid program arguments!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    // Read from image-file
    imageorig = readPPMP6(userin.inputimgname);

    // arrange apply kernel-filter
    imagenew = applyKernelFilter(userin.kernel, imageorig);

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

    // write image to image-file
    retwr = writePPMP6(userin.outputimgname, imagenew);
    if(retwr != 0)
    {
      printf(ANSI_COLOR_RED "ERROR! Function writePPMP6() failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    /***** free memory of new image *****/
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
    
    // free allocated memory of prog-args-structure
    freeCopiedProgArgs(&userin);

    printf(ANSI_COLOR_GREEN "\nFiltered image successfully generated!\n\n" ANSI_COLOR_RESET);

  }
  else // when too few or too many arguments given
  {
    // error
    printf(ANSI_COLOR_RED "ERROR! Check program arguments!\n" ANSI_COLOR_RESET);
    printUsageMsg();
  }

  return 0;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printUsageMsg()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints a usage-message and exits the program                 */
/* Input(s):     none                                                         */
/* Return value: none                                                         */
/******************************************************************************/
static void printUsageMsg(void)
{
  printf("Usage-Example:\n");
  printf("Unix:    ./imfilter input.ppm output.ppm -1,-1,-1,-1,8,-1,-1,-1,-1\n");
  printf("Windows: imfilter.exe input.ppm output.ppm -1,-1,-1,-1,8,-1,-1,-1,-1\n");
  exit(EXIT_FAILURE); // terminate
}

/******************************************************************************/
/*            S U B R O U T I N E :     strdupl()                             */
/*----------------------------------------------------------------------------*/
/* Purpose:      Duplicate string                                             */
/* Input(s):     (const char *) - Pointer of string that should be duplicated */
/* Return value: (char *)       - Returns duplicated pointer of duplicated    */
/*                                string                                      */
/******************************************************************************/
char *strdupl(const char *str)
{
  char *ret = NULL, *retsc = NULL;
  
  ret = (char *)malloc((strlen(str) + 1) * sizeof(char)); // alloc mem
  if(ret == NULL) 
  {
    ret = NULL;
  } 
  retsc = strncpy(ret, str, strlen(str) + 1); // copy string
  if(retsc != ret)
  {
    ret = NULL;
  }
  
  return ret; // Return the duplicated string 
}

/******************************************************************************/
/*            S U B R O U T I N E :     freeCopiedProgArgs()                  */
/*----------------------------------------------------------------------------*/
/* Purpose:      Frees char-pointer which points contains program arguments   */
/* Input(s):     (InputArgs *) - Pointer of structure that contains prog args */
/* Return value: none                                                         */
/******************************************************************************/
static void freeCopiedProgArgs(InputArgs *input)
{
  // free memory
  if(input->inputimgname != NULL)
  {
    free(input->inputimgname);
  }
  input->inputimgname = NULL;
  
  if(input->outputimgname != NULL)
  {
    free(input->outputimgname);
  }
  input->outputimgname = NULL;
  
  if(input->kernel != NULL)
  {
    free(input->kernel);
  }
  input->kernel = NULL;
}

