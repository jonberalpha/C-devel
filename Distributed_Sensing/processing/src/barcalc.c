/******************************************************************************/
/* Filename:      barcalc.c                                                   */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Contains calcBarTemp(), calcBarPres() and calcBarHumi()     */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <main.h>
#include <barcalc.h>

/******************************************************************************/
/*            S U B R O U T I N E :     calcBarTemp()                         */
/*----------------------------------------------------------------------------*/
/* Purpose:      Calculates bar-length according to given temperature         */
/* Input(s):     (int) - Actual temperature-value                             */
/* Return value: (int) - Returns bar-length                                   */
/******************************************************************************/
int calcBarTemp(int temp)
{
  int y2length = 0;
  
  y2length = (int)(150 * (temp - MIN_TEMP_K) / (MAX_TEMP_K - MIN_TEMP_K) + 5);
  if(y2length >= 150)
  {
    y2length = 150;
  }
  if(y2length <= 5)
  {
    y2length = 5;
  }
  
  y2length = 200 - y2length;

  return y2length;
}

/******************************************************************************/
/*            S U B R O U T I N E :     calcBarPres()                         */
/*----------------------------------------------------------------------------*/
/* Purpose:      Calculates bar-length according to given pressure            */
/* Input(s):     (int) - Actual pressure-value                                */
/* Return value: (int) - Returns bar-length                                   */
/******************************************************************************/
int calcBarPres(int pres)
{
  int y2length = 0;
  
  y2length = (int)(150 * (pres - MIN_PRES_PA) / (MAX_PRES_PA - MIN_PRES_PA) + 5);
  if(y2length >= 150)
  {
    y2length = 150;
  }
  if(y2length <= 5)
  {
    y2length = 5;
  }
  
  y2length = 400 - y2length;

  return y2length;
}

/******************************************************************************/
/*            S U B R O U T I N E :     calcBarHumi()                         */
/*----------------------------------------------------------------------------*/
/* Purpose:      Calculates bar-length according to given humidity            */
/* Input(s):     (int) - Actual humidity-value                                */
/* Return value: (int) - Returns bar-length                                   */
/******************************************************************************/
int calcBarHumi(int humi)
{
  int y2length = 0;
  
  y2length = (int)(150 * (humi - MIN_HUMI_REL) / (MAX_HUMI_REL - MIN_HUMI_REL) + 5);
  if(y2length >= 150)
  {
    y2length = 150;
  }
  if(y2length <= 5)
  {
    y2length = 5;
  }
  
  y2length = 600 - y2length;

  return y2length;
}