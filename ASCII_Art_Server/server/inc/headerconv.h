/******************************************************************************/
/* Filename:      headerconv.h                                                */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Header-File for headerconv.c                                */
/******************************************************************************/
#ifndef _headerconv_h_
#define _headerconv_h_

/******************************* PROTOTYPES ***********************************/
int setHTMLflag(char *format);
int convDimension(char *dimension, int *Wflag, int *Hflag, int *width, int *heigth);
int setINVflag(char *invert);

#endif
