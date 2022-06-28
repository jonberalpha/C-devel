/******************************************************************************/
/* Task:          Password Generator (Simple Task)                            */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V1.00                                                       */
/* Creation Date: 09.12.2020                                                  */
/* Description:   Generates a password from given template and length         */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include <string.h>
#include <ctype.h>

/******************************** DEFINES *************************************/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define MIN_CHAR           0x21
#define MAX_CHAR           0x7E

/******************************* STRUCTURES ***********************************/
typedef struct PWGENFlags
{
  int lowerchar;
  int upperchar;
  int digitchar;
  int ipchar;
} PWGENFlags;

/******************************* PROTOTYPES ***********************************/
static void printUsageMsg(void);
static int setFlags(char *temp, PWGENFlags *flagstorage);
static int checkFlags(char ch, PWGENFlags *flagstorage);

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  int randomchar = 0, i = 0, wh = 0, length = 0, retssc = 0, retsf = 0;
  int cat = -1, retpf = -1;
  char template[42] = "";
  char *retscp = NULL, *pw = NULL;
  PWGENFlags fs = {0, 0, 0, 0};

  srand(time(NULL)); // initialize rand()

  if(argc == 3) //when 2 input arguments
  {
    retssc = sscanf(argv[2], "%d", &length); // get length
    if(retssc != 1)
    {
      printf(ANSI_COLOR_RED "ERROR! Provided length-argument was not a number!\n" ANSI_COLOR_RESET);
      printUsageMsg();
    }

    pw = (char *)malloc((length + 1) * sizeof(char)); // declare pw (+1 cause of \0)
    if(pw == NULL)
    {
      printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    
    retscp = strncpy(template, argv[1], sizeof(template)); // get template
    if(retscp != template)
    {
      printf(ANSI_COLOR_RED "ERROR! strncpy() failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    
    retsf = setFlags(template, &fs);
    if(retsf < 0)
    {
      printf(ANSI_COLOR_RED "ERROR! No valid template string!\n" ANSI_COLOR_RESET);
      printUsageMsg();
    }
    
    for(i = 0; i < length; i++)
    {
      wh = 0;
      while(wh == 0)
      {
        randomchar = (rand()%((MAX_CHAR+1)-MIN_CHAR))+MIN_CHAR;
        cat = checkFlags(randomchar, &fs);
        if((cat == 1) || (cat == 2) || (cat == 3) || (cat == 4))
        {
          pw[i] = randomchar;
          wh = 1;
        }
      }
    }
    
    retpf = printf("%s\n", pw);
    if(retpf < 0)
    {
      printf(ANSI_COLOR_RED "ERROR! printf() failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    
    if(pw != NULL)
    {
      free(pw);
    }
    pw = NULL;
  }
  else
  {
    printf(ANSI_COLOR_RED "ERROR! Wrong input arguments!\n" ANSI_COLOR_RESET);
    printUsageMsg();
  }
  
  return 0;
}

/******************************** SUBROUTINES *********************************/
/******************************************************************************/
/*            S U B R O U T I N E :     printUsageMsg()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Prints a usage-message and exits the program                 */
/* Input(s):     none                                                         */
/* Return value: none                                                         */
/******************************************************************************/
static void printUsageMsg(void)
{
  printf("Usage-Example:\n");
  printf("Unix:    ./pwgen .aA 20\n");
  printf("Windows: pwgen.exe .aA 20\n");
  exit(EXIT_FAILURE); // terminate
}

/******************************************************************************/
/*            S U B R O U T I N E :     setFlags()                            */
/*----------------------------------------------------------------------------*/
/* Purpose:      Sets the flags according to given temp-string                */
/* Input(s):     (char *)       - String that contains pwgen-pattern          */
/*               (PWGENFlags *) - Contains the flags to set                   */
/* Return value: (int)          - Returns 0 on success and a negative         */
/*                                number on failure                           */
/******************************************************************************/
static int setFlags(char *temp, PWGENFlags *flagstorage)
{
  int ret = -1; 
  size_t i = 0;

  // initialize all flags to false
  flagstorage->lowerchar = 0;
  flagstorage->upperchar = 0;
  flagstorage->digitchar = 0;
  flagstorage->ipchar = 0;

  for(i = 0; i < strlen(temp); i++)
  {
    if(islower((int)temp[i]) != 0)
    {
      flagstorage->lowerchar = 1;
      ret = 0;
    }
    if(isupper((int)temp[i]) != 0)
    {
      flagstorage->upperchar = 1;
      ret = 0;
    }
    if(isdigit((int)temp[i]) != 0)
    {
      flagstorage->digitchar = 1;
      ret = 0;
    }
    if(ispunct((int)temp[i]) != 0)
    {
      flagstorage->ipchar = 1;
      ret = 0;
    }
  }
  
  if(ret != 0)
  {
    ret = -2;
  }

  return ret;
}

/******************************************************************************/
/*            S U B R O U T I N E :     checkFlags()                          */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks which flags fits the given string                     */
/* Input(s):     (char)         - Character to check                          */
/*               (PWGENFlags *) - Contains the reference flags                */
/* Return value: (int)          - Returns category 1 to 4 and a negative      */
/*                                number on failure                           */
/******************************************************************************/
static int checkFlags(char ch, PWGENFlags *flagstorage)
{
  int ret = -1;

  if((islower(ch) != 0) && (flagstorage->lowerchar == 1))
  {
    ret = 1;
  }
  if((isupper(ch) != 0) && (flagstorage->upperchar == 1))
  {
    ret = 2;
  }
  if((isdigit(ch) != 0) && (flagstorage->digitchar == 1))
  {
    ret = 3;
  }
  if((ispunct(ch) != 0) && (flagstorage->ipchar == 1))
  {
    ret = 4;
  }
  
  if(ret < 1)
  {
    ret = -2;
  }

  return ret;
}

