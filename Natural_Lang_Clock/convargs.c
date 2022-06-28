/******************************************************************************/
/* Filename:      convargs.c                                                  */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 11.12.2020                                                  */
/* Description:   Contains convArgs(), checkStr()                             */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <main.h>
#include <convargs.h>

/**************************** PRIVATE PROTOTYPES ******************************/
static int checkStr(const char *str);

/******************************************************************************/
/*            S U B R O U T I N E :     convArgs()                            */
/*----------------------------------------------------------------------------*/
/* Purpose:      Converts and checks given Stringnumbers                      */
/* Input(s):     (DigitalTime *) - Pointer of DigitalTime-structure which     */
/*                                 has to be set with the given time          */
/*               (char *)        - String that contains the hours             */
/*               (char *)        - String that contains the minutes           */
/* Return value: (int)           - Returns 0 on success and a negative number */
/*                                 on failure                                 */
/******************************************************************************/
int convArg(DigitalTime *tm, char *hr, char *mn)
{
  int ret = 0, retcs = -1;
  char *endp = NULL;
 
  tm->hours = (int)strtoul(hr, &endp, BASE_DEC);
  retcs = checkStr(endp);                          // check digits only
  if(retcs < 0)
  {
    ret = -1;
  }
  
  tm->minutes = (int)strtoul(mn, &endp, BASE_DEC);
  retcs = checkStr(endp);                          // check digits only
  if(retcs < 0)
  {
    ret = -2;
  }

  if(ret == 0)
  {
    if((tm->hours < 0) || (tm->hours > 23)) // check hours
    {
      ret = -3;
    }

    if((tm->minutes < 0) || (tm->minutes > 59)) // check minutes
    {
      ret = -4;
    }
  }

  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     checkStr()                            */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if a String contains numbers                          */
/* Input(s):     (char *) - String to check                                   */
/* Return value: (int)    - Returns 0 on success and a negative number        */
/*                          on failure                                        */
/******************************************************************************/
static int checkStr(const char *str)
{
  int ret = 0, retid = 0;
  size_t i = 0;
  
  for(i = 0; i < strlen(str); i++)
  {
    retid = isdigit(str[i]); // string should only contain numberss
    if(retid == 0)
    {
      ret = -1 - i;
    }
  }
  
  return ret;
}

