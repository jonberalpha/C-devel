/******************************************************************************/
/* Filename:      helper.c                                                    */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Contains getRandomInt(), addConvDuration(), checkRetdur(),  */
/*                         sendMSG()                                          */
/******************************************************************************/

/******************************* INCLUDES *************************************/
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <main.h>
#include <helper.h>

/******************************************************************************/
/*            S U B R O U T I N E :     getRandomInt()                        */
/*----------------------------------------------------------------------------*/
/* Purpose:      Returns random integer number between a given Min-Max-Range  */
/* Input(s):     (int) - Min-integer number                                   */
/*               (int) - Max-integer number                                   */
/* Return value: (int) - Returns random generated integer number              */
/******************************************************************************/
int getRandomInt(int min, int max)
{
  srand(time(0)); // use current time as seed for random generator
  return (rand() % (max - min + 1) + min);
}

/******************************************************************************/
/*            S U B R O U T I N E :     addConvDuration()                     */
/*----------------------------------------------------------------------------*/
/* Purpose:      Process sleeps controlled by the given duration              */
/* Input(s):     (int)          - duration-flag, if set then the sleep        */ 
/*                                acutally evoke                              */
/*               (int)          - time in seconds                             */
/* Return value: (int) - Returns 0 if sleep evokes,                           */
/*                              -1 if no sleep evokes, or                     */
/*                               number of seconds left to sleep              */
/*                                 if sleep gets interrupted                  */
/******************************************************************************/
int addConvDuration(int flag, int time)
{
  int ret = 0;
  
  if(flag == 1) // add duration only if the flag is set
  {
    ret = (int)sleep(time);
  }
  else
  {
    ret = -1;
  }
  
  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     checkRetdur()                         */
/*----------------------------------------------------------------------------*/
/* Purpose:      Exits program if the input is a number higher than zero      */
/* Input(s):     (int)  - number to be checked                                */
/* Return value: (void)                                                       */
/******************************************************************************/
void checkRetdur(int retval)
{
  if(retval > 0)
  {
    printf(ANSI_COLOR_RED "ERROR! Conversion process got aborted!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
}

/******************************************************************************/
/*            S U B R O U T I N E :     sendMSG()                             */
/*----------------------------------------------------------------------------*/
/* Purpose:      Transmits a sensor value via message queue                   */
/* Input(s):     (int)     - ID of the message queue                          */
/*               (MQdata*) - Pointer to the message structure                 */
/*               (size_t)  - Size of the actual message data                  */
/* Return value: (void)                                                       */
/******************************************************************************/
void sendMSG(int msqid, MQdata *psensmsg, size_t mqdatasize)
{
  int retmsgs = 0;

  retmsgs = msgsnd(msqid, psensmsg, mqdatasize, 0);
  if(retmsgs == -1)
  {
    printf(ANSI_COLOR_RED "ERROR! Sending message failed!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  } 
  else
  {
    printf("Sensor-Value %d with Measure-Type %ld sent!\n", psensmsg->mvalue, psensmsg->mtype);
  }
}

