/******************************************************************************/
/* Filename:      helper.h                                                    */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Header-File for helper.c                                    */
/******************************************************************************/
#ifndef _helper_h_
#define _helper_h_

/******************************* PROTOTYPES ***********************************/
int getRandomInt(int min, int max);
int addConvDuration(int flag, int time);
void checkRetdur(int retval);
void sendMSG(int msqid, MQdata *psensmsg, size_t mqdatasize);

#endif

