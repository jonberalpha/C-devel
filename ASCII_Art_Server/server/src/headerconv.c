/******************************************************************************/
/* Filename:      headerconv.c                                                */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Contains setHTMLflag(), convDimension(), setINVflag()       */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <headerconv.h>

/******************************************************************************/
/*            S U B R O U T I N E :     setHTMLflag()                         */
/*----------------------------------------------------------------------------*/
/* Purpose:      Returns 1 if the given format-string equals to "HTML"        */
/* Input(s):     (char *) - String with the format-information                */
/* Return value: (int)    - Returns 1 if format equals "HTML", if it equals   */
/*                          "TEXT" than 2 will be returned else 0 will be     */
/*                          returned                                          */
/******************************************************************************/
int setHTMLflag(char *format)
{
  int ret = 0, retscmp = 0;
  
  retscmp = strncmp(format, "HTML", strlen(format));
  if(retscmp == 0)
  {
    ret = 1;
  }
  
  retscmp = strncmp(format, "TEXT", strlen(format));
  if(retscmp == 0)
  {
    ret = 2;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     convDimension()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Converts given dimension-string into corresponding variables */
/* Input(s):     (char *) - String with the format-content                    */
/*               (int *)  - Pointer to width                                  */
/*               (int *)  - Pointer to height                                 */
/* Return value: (int)    - Returns 0 on success and a negative number        */
/*                          on failure                                        */
/******************************************************************************/
int convDimension(char *dimension, int *Wflag, int *Hflag, int *width, int *height)
{
  int ret = 0, i = 0, retscmp = 0, done = 0;
  char delimiter[] = ":";
  char *ptr = NULL;

  ptr = strtok(dimension, delimiter);

  while(ptr != NULL)
  {
    switch(i)
    {
      case 0: // unit
        retscmp = strncmp(ptr, "DEFAULT", strlen(ptr));
        if((retscmp == 0) && (done == 0))
        {
          *Wflag = 0;
          *Hflag = 0;
          done = 1;
        }
        else
        {
          retscmp = strncmp(ptr, "SIZE", strlen(ptr));
          if((retscmp == 0) && (done == 0))
          {
            *Wflag = 1;
            *Hflag = 1;
            done = 1;
          }
          else
          {
            retscmp = strncmp(ptr, "WIDTH", strlen(ptr));
            if((retscmp == 0) && (done == 0))
            {
              *Wflag = 1;
              *Hflag = 0;
              done = 1;
            }
            else
            {
              retscmp = strncmp(ptr, "HEIGHT", strlen(ptr));
              if((retscmp == 0) && (done == 0))
              {
                *Wflag = 0;
                *Hflag = 1;
                done = 1;
              }
            }
          }
        }
        break;
      case 1: // value1 - width
        if(*Wflag == 1)
        {
          *width = atoi(ptr);
        }
        if((*Wflag == 0) && (*Hflag == 1))
        {
          *height = atoi(ptr);
        }
        break;
      case 2: // value2 - height
        *height = atoi(ptr);
        break;
      default:
        ret = -1;
        break;
    }
    ptr = strtok(NULL, delimiter);
    i++;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     setINVflag()                          */
/*----------------------------------------------------------------------------*/
/* Purpose:      Returns 1 if the given invert-string equals to "INVERT"      */
/* Input(s):     (char *) - String with the invert-information                */
/* Return value: (int)    - Returns 1 if format equals "INVERT", if it equals */
/*                          "NOINVERT" than 2 will be returned else 0 will be */
/*                          returned                                          */
/******************************************************************************/
int setINVflag(char *invert)
{
  int ret = 0, retscmp = 0;
  
  retscmp = strncmp(invert, "INVERT", strlen(invert));
  if(retscmp == 0)
  {
    ret = 1;
  }
  
  retscmp = strncmp(invert, "NOINVERT", strlen(invert));
  if(retscmp == 0)
  {
    ret = 2;
  }
  
  return ret;
}

