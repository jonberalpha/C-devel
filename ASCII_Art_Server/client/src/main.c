/******************************************************************************/
/* Task:          ASCII-ART Server (Network Programming)                      */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Client Application: Sends Request-Header and jpeg-image     */
/*                to server and prints received ASCII-ART version of the image*/
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include <main.h>

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  int opt = 0, n = 0, j = 0;
  int HTMLflag = 0, WIDTHflag = 0, HEIGHTflag = 0, INVflag = 0;
  int width = 0, height = 0;
  int sockfd = 0, retscmp = 0, portnum = 0, wrcount = 0, retpf = 0;
  struct sockaddr_in serv_addr;
  char *retscpy = NULL;
  char buffer[42] = {0}, filename[80] = {0};
  char *socketaddr = NULL;
  char c = ' ', sendbuf = ' ';
  
  FILE *imgptr = NULL;
  
  // no arguments given
  if(argc == 1)
  {
    fprintf(stderr, "ERROR: No input arguments!\n");
    exit(EXIT_FAILURE);
  }
  
  while((opt = getopt(argc, argv, ":s:p:Hw:h:i")) != -1) 
  {
    switch(opt) 
    {
      case 's':
        socketaddr = optarg;
        break;
      case 'p':
        portnum = atoi(optarg);
        if((portnum < 4952) || (portnum > 65535))
        {
          fprintf(stderr, "ERROR: Portnumber must be between 4952 and 65535 or is missing!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'H':
        HTMLflag = 1;
        break;
      case 'w':
        WIDTHflag = 1;
        width = atoi(optarg);
        if(width <= 0)
        {
          fprintf(stderr, "ERROR: Width must be above 0!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'h':
        HEIGHTflag = 1;
        height = atoi(optarg);
        if(height <= 0)
        {
          fprintf(stderr, "ERROR: Height must be above 0!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'i':
        INVflag = 1;
        break;
      case ':':
        fprintf(stderr, "ERROR: Option '%c' requires an argument!\n", optopt);
        exit(EXIT_FAILURE);
      case '?':
        fprintf(stderr, "ERROR: Unknown option character: %c\n", optopt);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "ERROR: Problem evaluating arguments!\n");
        exit(EXIT_FAILURE);
    }
  }
  
  // check if there are false or missing input arguments
  if(optind == argc-1)
  {
    retscpy = strncpy(filename, argv[argc-1], sizeof(filename));
    retscmp = strncmp(retscpy, argv[argc-1], strlen(retscpy));
    if(retscmp != 0)
    {
      fprintf(stderr, "ERROR: Copy filename failed!\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    fprintf(stderr, "ERROR: Invalid or missing arguments!\n");
    exit(EXIT_FAILURE);
  }
  
  if(portnum == 0)
  {
    fprintf(stderr, "ERROR: Portnumber is missing!\n");
    exit(EXIT_FAILURE);
  }
  
  if(socketaddr == NULL)
  {
    fprintf(stderr, "ERROR: Socket-address is missing!\n");
    exit(EXIT_FAILURE);
  }
  
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    fprintf(stderr, "ERROR: Socket creation failed!\n");
    exit(EXIT_FAILURE);
  }
   
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portnum);
  
  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, socketaddr, &serv_addr.sin_addr) <= 0) 
  {
    fprintf(stderr, "ERROR: Invalid address or address not supported!\n");
    exit(EXIT_FAILURE);
  }
   
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    fprintf(stderr, "ERROR: Connection Failed!\n");
    exit(EXIT_FAILURE);
  }
  
  // send header
  // send format
  if(HTMLflag == 1)
  {
    n = sprintf(buffer, "HTML\n");
    if(n <= 0)
    {
      fprintf(stderr, "ERROR: sprintf failed!");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    n = sprintf(buffer, "TEXT\n");
    if(n <= 0)
    {
      fprintf(stderr, "ERROR: sprintf failed!");
      exit(EXIT_FAILURE);
    }
  }
  j = 0;
  sendbuf = buffer[0];
  while(sendbuf != '\n')
  {
    if(send(sockfd, &sendbuf, sizeof(char), 0) == -1)
    {
      perror("socket-send failed");
      exit(EXIT_FAILURE);
    }
    sendbuf = buffer[++j];
  }
  if(send(sockfd, "\n", sizeof(char), 0) == -1)
  {
    perror("socket-send failed");
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, sizeof(buffer));
  
  // send dimension
  if((WIDTHflag == 1) && (HEIGHTflag == 1))
  {
    n = sprintf(buffer, "SIZE:%d:%d\n", width, height);
    if(n <= 0)
    {
      fprintf(stderr, "ERROR: sprintf failed!");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    if((WIDTHflag == 1) && (HEIGHTflag == 0))
    {
      n = sprintf(buffer, "WIDTH:%d\n", width);
      if(n <= 0)
      {
        fprintf(stderr, "ERROR: sprintf failed!");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      if((WIDTHflag == 0) && (HEIGHTflag == 1))
      {
        n = sprintf(buffer, "HEIGHT:%d\n", height);
        if(n <= 0)
        {
          fprintf(stderr, "ERROR: sprintf failed!");
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        if((WIDTHflag == 0) && (HEIGHTflag == 0))
        {
          n = sprintf(buffer, "DEFAULT\n");
          if(n <= 0)
          {
            fprintf(stderr, "ERROR: sprintf failed!");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }
  j = 0;
  sendbuf = buffer[0];
  while(sendbuf != '\n')
  {
    if(send(sockfd, &sendbuf, sizeof(char), 0) == -1)
    {
      perror("socket-send failed");
      exit(EXIT_FAILURE);
    }
    sendbuf = buffer[++j];
  }
  if(send(sockfd, "\n", sizeof(char), 0) == -1)
  {
    perror("socket-send failed");
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, sizeof(buffer));
  
  // send invert
  if(INVflag == 1)
  {
    n = sprintf(buffer, "INVERT\n");
    if(n <= 0)
    {
      fprintf(stderr, "ERROR: sprintf failed!");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    n = sprintf(buffer, "NOINVERT\n");
    if(n <= 0)
    {
      fprintf(stderr, "ERROR: sprintf failed!");
      exit(EXIT_FAILURE);
    }
  }
  j = 0;
  sendbuf = buffer[0];
  while(sendbuf != '\n')
  {
    if(send(sockfd, &sendbuf, sizeof(char), 0) == -1)
    {
      perror("socket-send failed");
      exit(EXIT_FAILURE);
    }
    sendbuf = buffer[++j];
  }
  if(send(sockfd, "\n", sizeof(char), 0) == -1)
  {
    perror("socket-send failed");
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, sizeof(buffer));
  
  // Open image
  imgptr = fopen(filename, "r");
  if(imgptr == NULL)
  {
    fprintf(stderr, "ERROR: Cannot open file!\n");
    exit(EXIT_FAILURE);
  }
  // Read contents from image and send it to server
  while(fread(&c, sizeof(char), sizeof(char), imgptr) == 1)
  {
    wrcount = send(sockfd, &c, sizeof(char), 0); // Pass a single character
    if(wrcount != 1)
    {
      fprintf(stderr, "ERROR: Sending image-data failed!\n");
      exit(EXIT_FAILURE);
    }
  }
  
  if(imgptr != NULL)
  {
    fclose(imgptr);
    imgptr = NULL;
  }
  
  // send EOF - close write channel
  if(shutdown(sockfd, SHUT_WR) == -1)
  {
    perror("shutdown error");
    exit(EXIT_FAILURE);
  }
  
  while(recv(sockfd, &c, sizeof(char), 0) == 1)
  {
    retpf = write(STDOUT_FILENO, &c, sizeof(char)); // print out bytewise image in ascii-format
    if(retpf != 1)
    {
      fprintf(stderr, "ERROR: write failed!");
      exit(EXIT_FAILURE);
    }
  }
  
  close(sockfd); // close socket
  
  return 0;
}

