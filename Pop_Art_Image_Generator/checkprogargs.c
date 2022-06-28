/******************************************************************************/
/* Filename:      checkprogargs.c                                             */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 25.11.2020                                                  */
/* Description:   Contains checkProgArgs(), checkFilename()                   */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <string.h>

#include <main.h>

/**************************** PRIVATE PROTOYPES *******************************/
static int checkFilename(char *filename, char *suffix);

/******************************************************************************/
/*            S U B R O U T I N E :     checkProgArgs()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if the given input arguments are valid                */
/* Input(s):     (InputArgs *) - Pointer of InputStructure which contains     */
/*                               all program-arguments                        */
/* Return value: (int)         - Returns 0 on success and a negative number   */
/*                               on failure                                   */
/******************************************************************************/
int checkProgArgs(InputArgs *userin)
{
  char *token = NULL, *substr = ".ppm";
  size_t i = 0, chrcount = 0;
  int ret = -1;

  token = userin->rgbstring;
  chrcount = strlen(token);
  
  if(chrcount == 15) // if rgbstring is 15 than continue
  {
    for(i = 0; i < chrcount; i++)
    {
      if((i+1) % 4 == 0)
      {
        if(token[i] == ':')
        {
          ret = 0;
        } 
        else
        {
          ret = -1;
          break;
        }
      }
      else
      {
        if((token[i] == 'r') || (token[i] == 'g') || (token[i] == 'b'))
        {
          ret = 0;
        }
        else
        {
          ret = -2;
          break;
        } 
      }
    }
  }
  else
  {
    ret = -3;
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
/*            S U B R O U T I N E :     checkFilename()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks given filename, if suffix is correct                  */
/* Input(s):     (char *) - String that contains filename to check            */
/*               (char *) - String that contains reference-file-suffix        */
/* Return value: (int)    - Returns 0 on success and a negative number        */
/*                          on failure                                        */
/******************************************************************************/
static int checkFilename(char *filename, char *suffix)
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

