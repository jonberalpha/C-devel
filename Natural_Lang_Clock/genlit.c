/******************************************************************************/
/* Filename:      genlit.c                                                    */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 11.12.2020                                                  */
/* Description:   Contains genLit()                                           */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/
/*            S U B R O U T I N E :     genLit()                              */
/*----------------------------------------------------------------------------*/
/* Purpose:      Creates from given hours and minutes the time in literal     */
/* Input(s):     (int)    - Provided hours                                    */
/*               (int)    - Provided minutes                                  */
/*               (char *) - String in which the literated time                */
/*                          should be assigned                                */
/* Return value: (int) - Returns 0 on success and a negative number on failure*/
/******************************************************************************/
int genLit(int hours, int minutes, char *buffer)
{
  int ret = 0, retsp = -1;
  char *hour_table[] =
  {
    "midnight.", "one ", "two ", "three ", "four ", "five ", "six ", "seven ",
    "eight ", "nine ", "ten ", "eleven ", "noon.", "one ", "two ", "three ",
    "four ", "five ", "six ", "seven ", "eight ", "nine ", "ten ", "eleven ", 
    "midnight."
  };
  char *hour_suffix[]=
  {
   "in the morning.","in the afternoon."
  };
  int minfrom[] = {0,1,2,10,14,15,16,17,25,29,30,31,32,40,44,45,46,47,55,59};
  int minto[]   = {0,1,9,13,14,15,16,24,28,29,30,31,39,43,44,45,46,54,58,59};
  char *mintext[]  = 
  {
    "", "minute after ", "minutes after ",
    "minutes before quarter to ", "minute before quarter to ",
    "quarter past ", "minute after quarter past ", "minutes after quarter past ",
    "minutes to half past ", "minute to half past ", "half past ",
    "minute after half past ", "minutes after half past ",
    "minutes before quarter to ", "minute before quarter to ", "quarter to ",
    "minute after quarter to ", "minutes after quarter to ", "minutes before ",
    "minute before "
  };
  int mincalc[] = {0,0,0,15,15,0,15,15,30,30,0,30,30,45,45,0,45,45,60,60};
  int minvalue = 0, i = 0, minind = 0, hoursind = 0;
  char *mintext1[] = 
  {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
  };
  char *fullhour = "o'clock ";

  retsp = sprintf(buffer, "It is ");
  if(retsp < 0)
  {
    ret = -1;
  }

  // calculate indizes for minute text
  for(i=0; i <= 19; i++)
  {
    if((minutes >= minfrom[i]) && (minutes <= minto[i]))
    {
      minvalue = minutes - mincalc[i];
      if(minvalue < 0)
      {
        minvalue = minvalue * (-1);
      }
      minind = i;
      break;
    }
  }
  
  // Not full hour, not quarter past, not half past, not quarter to -> get minute text
  
  if((minvalue != 0)  && (minvalue != 15) && (minvalue != 30) && (minvalue != 45)) 
  {
    retsp = sprintf(buffer + strlen(buffer), "%s %s", mintext1[minvalue - 1], mintext[minind]);
    if(retsp < 0)
    {
      ret = -2;
    }
  }
  else
  {
    if((minvalue == 15) || (minvalue == 30) || (minvalue == 45))  // quarter past, half past, quarter to -> no minutes
    {
      retsp = sprintf(buffer + strlen(buffer), "%s", mintext[minind]);
      if(retsp < 0)
      {
        ret = -3;
      }
    }
  }
  
  hoursind = hours % 24;

  if(minutes >= 40)     // minutes >= 40 choose the next hour
  {
    retsp = sprintf(buffer + strlen(buffer), "%s", hour_table[hoursind + 1]);
    if(retsp < 0)
    {
      ret = -4;
    }
    hoursind = hoursind + 1;
  }
  else
  {
    retsp = sprintf(buffer + strlen(buffer), "%s", hour_table[hoursind]);
    if(retsp < 0)
    {
      ret = -5;
    }
  }

  if((hoursind % 12) != 0) // not noon and not midnight
  {
    if(minutes == 0)       // full hour
    {
      retsp = sprintf(buffer + strlen(buffer),"%s", fullhour);
      if(retsp < 0)
      {
        ret = -6;
      }
    }
    i = hours/12;        // calculate index: -> 0 = in the morning, 1 = in the afternoon
    retsp = sprintf(buffer + strlen(buffer),"%s", hour_suffix[i]);
    if(retsp < 0)
    {
      ret = -7;
    }
  }

  return ret;
}

