/******************************************************************************/
/* Task:          Distributed Sensing (System Programming)                    */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Executes Processing Task which converts sensor-values,      */
/*                creates a SVG-file with the mean-values, a SVG-file         */
/*                with the last 10 values and a TXT-file where all            */
/*                sensor-values logged                                        */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#include <main.h>
#include <dispdatahandling.h>

/**************************  PRIVATE PROTOTYPES *******************************/
static void cntrl_c_handler(int sig);

/************************* PRIVATE GLOBAL VARIABLES ***************************/
static int msqid = 0;
static MQdata *prcvmsg = NULL; // Declaration of the mq-structure pointer
static FILE *fptxt = NULL, *fpmsvg = NULL, *fpssvg = NULL;

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  int opt = 0, Ncounthumi = 0, Ncountpres = 0, Ncounttemp = 0, i = 0;
  int flagstorage[3] = {0, 0, 0}; // °C, °F, atm
  int retmsgr = 0, retpf = 0, retis = 0, retpv = 0, retfpmvc  = 0, retmis = 0;
  long int herg = 0, perg = 0, terg = 0;
  float meanhumi = 0.0, meanpres = 0.0, meantemp = 0.0, meanvalues[3] = {0, 0, 0};
  int indexhumi = 1, indexpres = 1, indextemp = 1;
  size_t mqdatasize = 0;
  
  int humiarray[NUM_OF_MEASUREMENTS], presarray[NUM_OF_MEASUREMENTS], temparray[NUM_OF_MEASUREMENTS];
  // initialize sensor-arrays
  for(i = 0; i < NUM_OF_MEASUREMENTS; i++)
  {
    humiarray[i] = 0;
    presarray[i] = 0;
    temparray[i] = 0;
  }
  
  signal(SIGINT, cntrl_c_handler); // install signal handler
  
  key_t key = ftok("/etc/hostname", 'a');
  
  while((opt = getopt(argc, argv, "cfa")) != -1)
  {
    switch(opt) 
    {
      case 'c':
        if(flagstorage[1] == 1)
        {
          printf(ANSI_COLOR_RED "ERROR! Do not use '-f' and"
          " '-c' simultaneously !\n" ANSI_COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        else
        {
          flagstorage[0] = 1; 
        }
        break;
      case 'f':
        if(flagstorage[0] == 1)
        {
          printf(ANSI_COLOR_RED "ERROR! Do not use '-c' and"
          " '-f' simultaneously !\n" ANSI_COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        else
        {
          flagstorage[1] = 1;
        }
        break;
      case 'a':
        flagstorage[2] = 1;
        break;
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

  // create message queue
  msqid = msgget(key, IPC_CREAT | 0666);
  if(msqid < 0)
  {
    printf(ANSI_COLOR_RED "ERROR! Message Queue cannot be created!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // allocate memory for the mq-structure
  prcvmsg = (MQdata *)malloc(sizeof(MQdata));
  if(prcvmsg == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot allocate memory!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // calculate size of message content
  mqdatasize = sizeof(MQdata) - sizeof(long);
  
  // open txt-file in write-mode
  fptxt = fopen("./sensdata.txt","w");
  if(fptxt == NULL)
  {
    printf(ANSI_COLOR_RED "ERROR! File cannot be written!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  printf("Processing Task:\n");
  while(1)
  {
    retmsgr = msgrcv(msqid, prcvmsg, mqdatasize, 0, 0);
    if(retmsgr == -1) 
    {
      printf(ANSI_COLOR_RED "ERROR! Cannot receive messages!\n" ANSI_COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    else
    {
      // print value from the message queue to console
      retpv = printMQVal(flagstorage, prcvmsg->mtype, prcvmsg->mvalue);
      if(retpv < 0)
      {
        printf(ANSI_COLOR_RED "ERROR! Printing Sensor-Values to console failed!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      
      // print sensor-values to file
      retpf = printValToTXTFile(&fptxt, flagstorage, prcvmsg->mtype, prcvmsg->mvalue);
      if(retpf < 0)
      {
        printf(ANSI_COLOR_RED "ERROR! Cannot write value to file!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      
      // calculate the mean-values and convert if the corresponding flag is set
      if(prcvmsg->mtype == HumidityVal)
      {
        herg = herg + prcvmsg->mvalue;
        meanhumi = (float)herg / (float)indexhumi;
        indexhumi++;
      }
      if(prcvmsg->mtype == PressureVal)
      {
        perg = perg + prcvmsg->mvalue;
        meanpres = (float)perg / (float)indexpres;
        indexpres++;
        if(flagstorage[2] == 1)
        {
          meanpres = meanpres / 101325; // conversion PA to atm
        }
      }
      if(prcvmsg->mtype == TemperatureVal)
      {
        terg = terg + prcvmsg->mvalue;
        meantemp = (float)terg / (float)indextemp;
        indextemp++;
        if(flagstorage[0] == 1)
        {
          meantemp = meantemp - 273.15; // conversion K to °C
        }
        if(flagstorage[1] == 1)
        {
          meantemp = (meantemp - 273.15) * (9/5) + 32; // conversion K to °F
        }
      }
      
      // print mean values to console
      meanvalues[0] = meanhumi;
      meanvalues[1] = meanpres;
      meanvalues[2] = meantemp;
      retfpmvc = printMeanValCons(flagstorage, meanvalues);
      if(retfpmvc < 0)
      {
        printf(ANSI_COLOR_RED "ERROR! Printing Mean-Sensor-Values to console failed!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      
      // open svg-file for mean-sensor-values in write-mode
      fpmsvg = fopen("./meansensdata.svg","w");
      if(fpmsvg == NULL)
      {
        printf(ANSI_COLOR_RED "ERROR! File cannot be written!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      
      // print mean-sensor-values to SVG-File
      retmis = printMeanValToSVGFile(&fpmsvg, flagstorage, meanhumi, meanpres, meantemp);
      if(retmis < 0)
      {
        printf(ANSI_COLOR_RED "ERROR! Cannot write svg-file!\n" ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      
      // close svg-file
      if(fpmsvg != NULL)
      {
        fclose(fpmsvg);
        fpmsvg = NULL;
      }
      
      // print last N measurements of each measuretype to svg-file
      if((Ncounthumi == NUM_OF_MEASUREMENTS) || (Ncountpres == NUM_OF_MEASUREMENTS) || (Ncounttemp == NUM_OF_MEASUREMENTS))
      {
        // open svg-file for mean-sensor-values in write-mode
        fpssvg = fopen("./sensdata.svg","w");
        if(fpssvg == NULL)
        {
          printf(ANSI_COLOR_RED "ERROR! File cannot be written!\n" ANSI_COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        
        retis = printValToSVGFile(&fpssvg, flagstorage, humiarray, presarray, temparray);
        if(retis < 0)
        {
          printf(ANSI_COLOR_RED "ERROR! Cannot write values to svg-file!\n" ANSI_COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        
        // close svg-file
        if(fpssvg != NULL)
        {
          fclose(fpssvg);
          fpssvg = NULL;
        }
        
        // reset run-variable according to whom reached NUM_OF_MEASUREMENTS
        if(Ncounthumi == NUM_OF_MEASUREMENTS)
        {
          Ncounthumi = 0;
        }
        if(Ncountpres == NUM_OF_MEASUREMENTS)
        {
          Ncountpres = 0;
        }
        if(Ncounttemp == NUM_OF_MEASUREMENTS)
        {
          Ncounttemp = 0;
        }
      }
      else
      {
        if(prcvmsg->mtype == HumidityVal)
        {
          humiarray[Ncounthumi] = prcvmsg->mvalue;
          Ncounthumi++;
        }
        if(prcvmsg->mtype == PressureVal)
        {
          presarray[Ncountpres] = prcvmsg->mvalue;
          Ncountpres++;
        }
        if(prcvmsg->mtype == TemperatureVal)
        {
          temparray[Ncounttemp] = prcvmsg->mvalue;
          Ncounttemp++;
        }
      }
    }
  }
}

/************************ SIGNAL HANDLER FUNCTION *****************************/
void cntrl_c_handler(int sig)
{
  (void)sig; // sig variable voided because not used
  int retmsgc = 0;
  const char *buf = "\n\nCntrl+c detected: Exiting program ...\n ";
  
  write(STDOUT_FILENO, buf, strlen(buf));
 
  // destroy message queue
  retmsgc = msgctl(msqid, IPC_RMID, NULL);
  if(retmsgc == -1)
  {
    printf(ANSI_COLOR_RED "ERROR! Cannot remove message queue!\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  
  // close txt-file
  if(fptxt != NULL)
  {
    fclose(fptxt);
    fptxt = NULL;
  }
  
  // free allocated memory
  if(prcvmsg != NULL)
  {
    free(prcvmsg);
    prcvmsg = NULL;
  }
 
  exit(EXIT_SUCCESS);
}

