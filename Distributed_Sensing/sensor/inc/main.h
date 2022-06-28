/******************************************************************************/
/* Filename:      main.h                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Header-File for main.c                                      */
/******************************************************************************/
#ifndef _main_h_
#define _main_h_

/******************************** DEFINES *************************************/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MIN_HUMI_REL       10
#define MAX_HUMI_REL       100

#define MIN_PRES_PA        96000
#define MAX_PRES_PA        140000

#define MIN_TEMP_K         233
#define MAX_TEMP_K         313

/****************************** ENUMERATION ***********************************/
enum SENSOR_VALUE_TYPES {HumidityVal = 1, PressureVal = 2, TemperatureVal = 3};

/******************************* STRUCTURES ***********************************/
typedef struct MQdata 
{
  long mtype;      // 1 ... Humidity, 2 ... Pressure, 3 ... Temperature
  int mvalue;      // actual sensor-value
}MQdata;


#endif
