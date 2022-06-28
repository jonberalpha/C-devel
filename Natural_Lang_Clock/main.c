/******************************************************************************/
/* Task:          Natural Language Clock (Medium Task)                        */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 11.12.2020                                                  */
/* Description:   Converts the given digital time to a literal output         */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <main.h>
#include <convargs.h>
#include <genlit.h>

/*************************** PRIVATE PROTOTYPES *******************************/
static void printUsageMsg(void);

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  DigitalTime tm = {0, 0};
  int retca = -1, retgl = -1;
  char buffer[80] = "";

  if(argc == 3) // 2 input arguments
  {
    retca = convArg(&tm, argv[1], argv[2]);
    if(retca < 0)
    {
      fprintf(stderr, ANSI_COLOR_RED "ERROR! Provided inputs are no numbers!\n" ANSI_COLOR_RESET);
      printUsageMsg();
    }

    retgl = genLit(tm.hours, tm.minutes, buffer);
    if(retgl < 0)
    {
      fprintf(stderr, ANSI_COLOR_RED "ERROR! Generating literated time failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);
  }
  else
  {
    fprintf(stderr, ANSI_COLOR_RED "ERROR! Wrong input arguments!\n" ANSI_COLOR_RESET);
    printUsageMsg();
  }

  return 0;
}

/******************************************************************************/
/*            S U B R O U T I N E :     printUsageMsg()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints a usage-message and exits the program                 */
/* Input(s):     none                                                         */
/* Return value: none                                                         */
/******************************************************************************/
static void printUsageMsg(void)
{
  fprintf(stderr, "Usage-Example:\n");
  fprintf(stderr, "Unix:    ./clock 3 15\n");
  fprintf(stderr, "Windows: clock.exe 3 15\n");
  exit(EXIT_FAILURE); // terminate
}

