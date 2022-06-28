/******************************************************************************/
/* Filename:      checkprogargs.c                                             */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 19.12.2020                                                  */
/* Description:   Contains checkProgArgs(), checkNumberType(), checkFilename()*/
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <main.h>
#include <ppmp6io.h>
#include <imgfilter.h>
#include <checkprogargs.h>

/**************************** PRIVATE PROTOYPES *******************************/
static int checkNumberType(const char *str);
static int checkFilename(const char *filename, const char *suffix);

/******************************************************************************/
/*            S U B R O U T I N E :     checkProgArgs()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if the given input arguments are valid                */
/* Input(s):     (const InputArgs *) - Pointer of InputStructure which        */
/*                                     contains all program-arguments         */
/* Return value: (int)               - Returns 0 on success and a negative    */
/*                                     number on failure                      */
/******************************************************************************/
int checkProgArgs(const InputArgs *userin)
{
  char *kstr = NULL, *kstr_o = NULL;
  char *token = NULL, *substr = ".ppm";
  int ret = 0, retif = 0, i = 0;
  
  kstr = strdupl(userin->kernel);
  if(kstr == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Unable to copy string!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  kstr_o = kstr;
  
  // check if commas are present at begin or end
  if((kstr[0] == ',') || ((kstr[strlen(kstr) - 1]) == ','))
  {
    ret = -1;
  }
  
  // check if paramaters are type of float or int
  if(ret == 0)
  {
    token = strtok(kstr, ",");
    while(token != NULL)
    {
      retif = checkNumberType(token);
      if(retif < 0)
      {
        ret = -2;
        break;
      }
      token = strtok(NULL, ",");
      i = i + 1;
    }
  }
  
  /***** free memory of helper string *****/
  if(kstr_o != NULL)
  {
    free(kstr_o);
  }
  kstr_o = NULL;
  kstr = NULL;
  
  if(ret == 0)
  {
    if(i != COEFF_SIZE) // if coeff. size is valid
    {
      ret = -3;
    }
  }
  
  // check inputimagename
  if(ret == 0)
  {
    ret = checkFilename(userin->inputimgname, substr);
  }
  
  // check outputimagename
  if(ret == 0)
  {
    ret = checkFilename(userin->outputimgname, substr);
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     checkNumberType()                     */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if given string is a floating number                  */
/* Input(s):     (const char *) - String that contains floating number        */
/* Return value: (int)          - Returns 0 if flaot, 1 if int and a negative */
/*                                number on failure                           */
/******************************************************************************/
static int checkNumberType(const char *str)
{
  int ret = -1;
  char *endptrd = NULL, *endptri = NULL;
  (void)strtod(str, &endptrd);
  (void)strtol(str, &endptri, 10);
  if(*endptrd == 0) 
  {
    ret = 0;  // is float
  } 
  if(*endptri == 0)
  {
    ret = 1;  // is int
  }

  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     checkFilename()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if the given filename is consistent with suffix       */
/* Input(s):     (const char *) - String that contains filename to check      */
/*               (const char *) - String that contains reference file-suffix  */
/* Return value: (int)          - Returns 0 on success and a negative number  */
/*                                on failure                                  */
/******************************************************************************/
static int checkFilename(const char *filename, const char *suffix)
{
  int ret = -1;
  char *pos = NULL;
  
  // only if strlen(filename) larger than strlen(suffix)
  if(strlen(filename) > strlen(suffix))
  {
    // outputimagename correct?
    pos = strstr(filename, suffix);
    if(pos == NULL)
    {
      ret = -4;
    }
    else
    {
      // .ppm last substring?
      if(strlen(suffix) == ((strlen(filename)) - (size_t)(pos - filename)))
      {
        ret = 0;
      }
      else
      {
        ret = -5;
      }
    }
  }
  else
  {
    ret = -6;
  }

  return ret;
}

