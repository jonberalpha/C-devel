/******************************************************************************/
/* Filename:      main.h                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 11.12.2020                                                  */
/* Description:   Header-File for main.c                                      */
/******************************************************************************/
#ifndef _main_h_
#define _main_h_

/******************************** DEFINES *************************************/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/******************************* STRUCTURES ***********************************/
typedef struct DigitalTime
{
  int hours;
  int minutes;
} DigitalTime;

#endif

