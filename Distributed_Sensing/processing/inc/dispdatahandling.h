/******************************************************************************/
/* Filename:      dispdatahandling.h                                          */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Header-File for dispdatahandling.c                          */
/******************************************************************************/
#ifndef _displayhandling_h_
#define _displayhandling_h_

/******************************* PROTOTYPES ***********************************/
int printMQVal(int flags[], long senstype, int sensvalue);
int printMeanValCons(int flags[], float meanvalues[]);
int printValToTXTFile(FILE **fp, int flags[], long senstype, int sensvalue);
int printMeanValToSVGFile(FILE **fp, int flags[], float mhumi, float mpres, float mtemp);
int printValToSVGFile(FILE **fp, int flags[], int humiarray[], int presarray[], int temparray[]);

#endif

