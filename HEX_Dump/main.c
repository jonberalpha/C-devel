/******************************************************************************/
/* Task:          HEX Dump                                                    */
/* Filename:      main.c                                                      */
/* Author: 	      Berger Jonas                                                */
/* Version:       V01                                                         */
/* Creation Date: 14.11.2020                                                  */
/* Description:   Prints HEX Dump of given file                               */
/******************************************************************************/
/******************************** INCLUDES ************************************/
#include <stdio.h>  // File handling
#include <stdlib.h>
#include <string.h> // string operations

/******************************** DEFINES *************************************/
#define N 8

/******************************* PROTOTYPES ***********************************/
static void printUsageMsg(void);

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  FILE *file = NULL;
  char filename[42] = "";
  unsigned char buf[80] = "";
  char consbuf[80] = "";
  char *retstrcpy = NULL; // return strcpy()
  int ret = 0; // return value of fread()
  int retfcl = 0; // return value of fclose()
  unsigned int offset = 0; // offset for printed bytes
  int i = 0;

  // check count of arguments
  if(argc == 1)
  {
    printf("ERROR! Too few arguments!\n");
    printUsageMsg();
  }
  else 
  {
    if(argc > 2)
    {
      printf("ERROR! Too many arguments!\n");
      printUsageMsg();
    }
    else
    {
      retstrcpy = strncpy(filename, argv[1], sizeof(filename)); // copy arg in filename
      if(retstrcpy != filename) // check if strcpy() successful
      {
        printf("ERROR! strcpy() failed!\n");
        exit(EXIT_FAILURE); // terminate
      }
      
      // read file-content
      file = fopen(filename,"r"); // open file in read-mode
      if(file == NULL)
      {
        printf("ERROR! File can not be opened!\n");
        exit(EXIT_FAILURE);
      }
      else
      {
        while(feof(file) == 0) // check eof file
        {
          ret = (int)fread(buf, sizeof(unsigned char), N, file); // ret = read bytes
          
          if(ret == 0) // if EOF is reached break loop
          {
            break;
          }
          
          sprintf(consbuf, "%08X  ", offset); // print offset
          
          for(i = 0; i < ret; i++) // print hex values
          {
            sprintf(consbuf + strlen(consbuf), " %02X", buf[i]);
          }
          
          if(ret < 8) // if < 8 bytes, print spaces
          {
            for(i = 0; i < 8 - ret; i++)
            {
              sprintf(consbuf + strlen(consbuf), "   ");
            }
          }
          
          sprintf(consbuf + strlen(consbuf), "   ");   // print spaces between hex numbers and ASCIIs 
          
          for(i = 0; i < ret; i++) // print ASCII characters
          {
            if((buf[i] < (unsigned char)32) || (buf[i] > (unsigned char)126)) // handle unprintable characters
            {
              sprintf(consbuf + strlen(consbuf), "."); // replace them with points
            }
            else                                       // else
            {
              sprintf(consbuf + strlen(consbuf), "%c", buf[i]); // print character        
            }
          }
          
          sprintf(consbuf + strlen(consbuf), "\n");
          
          printf("%s", consbuf); // print whole hexdump-line (possible to redirect output to a file with fprintf())
          retstrcpy = strcpy(consbuf, "");
          if(retstrcpy != consbuf) // check if strcpy() successful
          {
            printf("ERROR! strcpy() failed!\n");
            exit(EXIT_FAILURE); // terminate
          }
          
          offset = offset + (unsigned int)ret; // new offset = cur offset + number of bytes (ret)
        }
        
        if(file != NULL)
        {
          retfcl = fclose(file); // close file
          if(retfcl != 0) // check return value of fclose()
          {
            printf("ERROR! fclose() failed!\n");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }
  
  return 0;
}

/******************************** SUBROUTINES *********************************/
/******************************************************************************/
/*            S U B R O U T I N E :     printUsageMsg()                       */
/*----------------------------------------------------------------------------*/
/* Purpose:      Checks if the given filename is valid                        */
/* Input(s):     none                                                         */
/* Return value: none                                                         */
/******************************************************************************/
static void printUsageMsg(void)
{
  printf("Usage-Example:\n");
  printf("Unix:    ./hexdump file.txt\n");
  printf("Windows: hexdump.exe file.txt\n");
  exit(EXIT_FAILURE); // terminate
}

