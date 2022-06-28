/******************************************************************************/
/* Task:          Distributed Sensing (System Programming)                    */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Executes Sensor Task which fills a message queue with       */
/*                sensor values                                               */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#include <main.h>
#include <helper.h>

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  int opt = 0;
  int Hflag = 0, Pflag = 0, Tflag = 0, dflag = 0;
  int duration = 0, retdur = 0;
  int msqid = 0;
  
  size_t mqdatasize = 0;
  key_t key = ftok("/etc/hostname", 'a');
  
  // Declaration of the mq-structure pointer
  MQdata *psensmsg = NULL;
  
  // no arguments given
  if(argc == 1)
  {
    printf(ANSI_COLOR_RED "ERROR! No input arguments!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  while((opt = getopt(argc, argv, ":HPTd:")) != -1) 
  {
    switch(opt) 
    {
      case 'H':
        Hflag = 1;
        break;
      case 'P':
        Pflag = 1;
        break;
      case 'T':
        Tflag = 1;
        break;
      case 'd':
        dflag = 1;
        duration = atoi(optarg);
        if(duration <= 0)
        {
          printf(ANSI_COLOR_RED "ERROR! Duration must be above 0 sec!\n" ANSI_COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        break;
      case ':':
        printf(ANSI_COLOR_RED "ERROR! Option '%c' requires an argument!\n" ANSI_COLOR_RESET, optopt);
        exit(EXIT_FAILURE);
      case '?':
        printf(ANSI_COLOR_RED "ERROR! Unknown option character: %c\n" ANSI_COLOR_RESET, optopt);
        exit(EXIT_FAILURE);
      default:
        printf(ANSI_COLOR_RED "ERROR! Problem evaluating arguments!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
    }
  }
  
  // check if there are false input arguments
  if(optind < argc)
  {
    printf(ANSI_COLOR_RED "ERROR! Non invalid arguments!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // get message queue
  msqid = msgget(key, 0);
  if((msqid == -1) && (errno == ENOENT)) // if it is not created yet
  {
    printf(ANSI_COLOR_RED "ERROR! Processing Task has to be executed first!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  else
  {
    if(msqid == -1) // other failures
    {
      printf(ANSI_COLOR_RED "ERROR! Getting message queue failed!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
  }
  
  // allocate memory for the mq-structure
  psensmsg = (MQdata *)malloc(sizeof(MQdata));
  if(psensmsg == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // initialize allocated MQ-structure
  memset(psensmsg, 0, sizeof(MQdata));
  
  // calculate size of message content
  mqdatasize = sizeof(MQdata) - sizeof(long);
  
  printf("Sensor Task:\n");
  
  // generate sensor values and send it
  if(Hflag == 1)
  {
    retdur = addConvDuration(dflag, duration);
    checkRetdur(retdur);
    
    psensmsg->mtype = HumidityVal;
    psensmsg->mvalue = getRandomInt(MIN_HUMI_REL, MAX_HUMI_REL);
    sendMSG(msqid, psensmsg, mqdatasize);
  }
  if(Pflag == 1)
  {
    retdur = addConvDuration(dflag, duration);
    checkRetdur(retdur);
    
    psensmsg->mtype = PressureVal;
    psensmsg->mvalue = getRandomInt(MIN_PRES_PA, MAX_PRES_PA);
    sendMSG(msqid, psensmsg, mqdatasize);
  }
  if(Tflag == 1)
  {
    retdur = addConvDuration(dflag, duration);
    checkRetdur(retdur);
    
    psensmsg->mtype = TemperatureVal;
    psensmsg->mvalue = getRandomInt(MIN_TEMP_K, MAX_TEMP_K);
    sendMSG(msqid, psensmsg, mqdatasize);
  }
  
  // free allocated memory
  if(psensmsg != NULL)
  {
    free(psensmsg);
    psensmsg = NULL;
  }
  
  return 0;
}

