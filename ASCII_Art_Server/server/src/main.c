/******************************************************************************/
/* Task:          ASCII-ART Server (Network Programming)                      */
/* Filename:      main.c                                                      */
/* Author:        Berger Jonas                                                */
/* Version:       V01                                                         */
/* Description:   Server Application: Creates ASCII-Art Image from received   */
/*                Request-Header and jpeg-image                               */
/******************************************************************************/

/******************************** INCLUDES ************************************/
#include <sys/socket.h>                                 // socket(), bind(), ...
#include <netinet/in.h>                                 // struct sockaddr_in
#include <strings.h>                                    // memset(), ...
#include <unistd.h>                                     // write(), close(), ...
#include <stdio.h>                                      // printf(), sprintf()
#include <arpa/inet.h>                                  // inet_ntoa(), ...
#include <string.h>                                     // strlen()
#include <sys/wait.h>                                   // waitpid()
#include <signal.h>                                     // signal()
#include <stdlib.h>                                     // exit()

#include <main.h>
#include <headerconv.h>

/********************** SIGNAL HANDLER INSTALLATION ***************************/
void SIGCHLD_handler(int);
void install_SIGCHLD_handler(void);

/***************************** MAIN FUNCTION **********************************/
int main(int argc, char *argv[])
{
  int listenfd = 0, connfd = 0, retcd = 0, j = 0, n = 0, retsp = 0;
  struct sockaddr_in servaddr, cliaddr;
  pid_t ForkPID;
  socklen_t len;
  int optval = 1, ret = 0, portnum = 0, i = 0, retscmp = 0;
  char *retscpy = NULL;
  char buffer[42] = {0}, argbuf[20] = {0}, headerformat[42] = {0};
  char headerdimension[42] = {0}, headerinvert[42] = {0};
  int width = 0, height = 0, HTMLflag = 0, INVflag = 0, WIDTHflag = 0, HEIGHTflag = 0;
  char recvbuf = ' ';
  
  char *binaryPathjp2a = "/usr/bin/jp2a";
  
  if(argc != 2)
  {
    fprintf(stderr, "ERROR: Too many or too few arguments!\n");
    exit(EXIT_FAILURE);
  }
  
  // portnum
  ret = sscanf(argv[1], "%d", &portnum);
  if(ret != 1)
  {
    fprintf(stderr, "ERROR: Argument not a number!\n");
    exit(EXIT_FAILURE);
  }
  
  if((portnum < 4952) || (portnum > 65535))
  {
    fprintf(stderr, "ERROR: Portnumber must be between 4952 and 65535\n");
    exit(EXIT_FAILURE);
  }
  
  // establish SIGCHLD signal handler that deals with zombies
  install_SIGCHLD_handler();
  
  // socket() open a file descriptor
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if(listenfd == -1)
  {
    perror ("socket error");
    exit(EXIT_FAILURE);
  }
  // setsockopt() free previously used sockets()
  ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
  if(ret == -1)
  {
    perror("setsockopt error");
    exit(EXIT_FAILURE);
  }
  
  // fill in the structure required to handle bind()
  memset(&servaddr, 0, sizeof(servaddr));
  
  servaddr.sin_family = AF_INET;  // use IPv4
  // htonl() and htons() convert values between host and
  // network byte order
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(portnum);
  
  // bind()
  ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if(ret == -1)
  {
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  // listen()
  ret = listen(listenfd, 5);
  if(ret == -1)
  {
    perror("listen error");
    exit(EXIT_FAILURE);
  }
  
  // print running status
  n = sprintf(buffer, "Server running ...\n");
  if(n <= 0)
  {
    perror("sprintf error");
    exit(EXIT_FAILURE);
  }
  ret = write(STDOUT_FILENO, buffer, sizeof(buffer));
  if(ret == -1)
  {
    perror("write error");
    exit(EXIT_FAILURE);
  }

  // endless loop
  while(1)
  {
    len = sizeof(cliaddr);
    // accept()
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
    if(connfd == -1)
    {
      perror("accept error");
      exit(EXIT_FAILURE);
    }
    
    // let the child handle the connection
    ForkPID = fork();
    if(ForkPID == 0)
    {
      // child process
      close(listenfd); // close listen port
      
      // read header
      // receive data from the client
      for(i = 0; i < 3; i++)
      {
        memset(buffer, 0, sizeof(buffer));
        j = 0;
        recvbuf = ' ';
        while(recvbuf != '\n')
        {
          if(recv(connfd, &recvbuf, sizeof(recvbuf), 0) == -1)
          {
            perror("receive failed");
            exit(EXIT_FAILURE);
          }
          //write(STDOUT_FILENO, &recvbuf, sizeof(char));
          if(recvbuf != '\n')
          {
            buffer[j] = recvbuf;
          }
          j++;
        }
        buffer[j++] = '\0'; // adding \0 at the end
        
        switch(i)
        {
          case 0: // recv format-header-part
            retscpy = strncpy(headerformat, buffer, sizeof(headerformat));
            retscmp = strncmp(retscpy, buffer, strlen(retscpy));
            if(retscmp != 0)
            {
              fprintf(stderr, "ERROR: Copy from recv-buffer failed!\n");
              exit(EXIT_FAILURE);
            }
            HTMLflag = setHTMLflag(headerformat);
            break;
          case 1: // recv dimension-header-part
            retscpy = strncpy(headerdimension, buffer, sizeof(headerdimension));
            retscmp = strncmp(retscpy, buffer, strlen(retscpy));
            if(retscmp != 0)
            {
              fprintf(stderr, "ERROR: Copy from recv-buffer failed!\n");
              exit(EXIT_FAILURE);
            }
            retcd = convDimension(headerdimension, &WIDTHflag, &HEIGHTflag, &width, &height);
            if(retcd < 0)
            {
              fprintf(stderr, "ERROR: Convert dimension failed!\n");
              exit(EXIT_FAILURE);
            }
            break;
          case 2: // recv invert-header-part
            retscpy = strncpy(headerinvert, buffer, sizeof(headerinvert));
            retscmp = strncmp(retscpy, buffer, strlen(retscpy));
            if(retscmp != 0)
            {
              fprintf(stderr, "ERROR: Copy from recv-buffer failed!\n");
              exit(EXIT_FAILURE);
            }
            INVflag = setINVflag(headerinvert);
            break;
          default:
            fprintf(stderr, "ERROR: An unexpected error occured!\n");
            exit(EXIT_FAILURE);
            break;
        }
      }
      
      dup2(connfd, 0); // redirect STDIN
      dup2(connfd, 1); // redirect STDOUT
      
      // execute a jp2a-application according to set header-flags
      if((WIDTHflag == 1) && (HEIGHTflag == 1))
      {
        retsp = sprintf(argbuf, "--size=%dx%d", width, height);
        if(retsp < 0)
        {
          fprintf(stderr, "ERROR: sprintf failed!");
          exit(EXIT_FAILURE);
        }
        if(HTMLflag == 1)
        {
          if(INVflag == 1)
          {
            ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, "--invert", (char *)NULL);
            if(ret == -1)
            {
              perror("execl error");
              exit(EXIT_FAILURE);
            }
          }
          else
          {
            ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, (char *)NULL);
            if(ret == -1)
            {
              perror("execl error");
              exit(EXIT_FAILURE);
            }
          }
        }
        else
        {
          if(INVflag == 1)
          {
            ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, "--invert", (char *)NULL);
            if(ret == -1)
            {
              perror("execl error");
              exit(EXIT_FAILURE);
            }
          }
          else
          {
            ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, (char *)NULL);
            if(ret == -1)
            {
              perror("execl error");
              exit(EXIT_FAILURE);
            }
          }
        }
      }
      else
      {
        if((WIDTHflag == 1) && (HEIGHTflag == 0))
        {
          retsp = sprintf(argbuf, "--width=%d", width);
          if(retsp < 0)
          {
            fprintf(stderr, "ERROR: sprintf failed!");
            exit(EXIT_FAILURE);
          }
          if(HTMLflag == 1)
          {
            if(INVflag == 1)
            {
              ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, "--invert", (char *)NULL);
              if(ret == -1)
              {
                perror("execl error");
                exit(EXIT_FAILURE);
              }
            }
            else
            {
              ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, (char *)NULL);
              if(ret == -1)
              {
                perror("execl error");
                exit(EXIT_FAILURE);
              }
            }
          }
          else
          {
            if(INVflag == 1)
            {
              ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, "--invert", (char *)NULL);
              if(ret == -1)
              {
                perror("execl error");
                exit(EXIT_FAILURE);
              }
            }
            else
            {
              ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, (char *)NULL);
              if(ret == -1)
              {
                perror("execl error");
                exit(EXIT_FAILURE);
              }
            }
          }
        }
        else
        {
          if((WIDTHflag == 0) && (HEIGHTflag == 1))
          {
            retsp = sprintf(argbuf, "--height=%d", height);
            if(retsp < 0)
            {
              fprintf(stderr, "ERROR: sprintf failed!");
              exit(EXIT_FAILURE);
            }
            if(HTMLflag == 1)
            {
              if(INVflag == 1)
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, "--invert", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
              else
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", "--html", argbuf, (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
            }
            else
            {
              if(INVflag == 1)
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, "--invert", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
              else
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", argbuf, (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
            }
          }
          else
          {
            if(HTMLflag == 1)
            {
              if(INVflag == 1)
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", "--html", "--invert", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
              else
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", "--html", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
            }
            else
            {
              if(INVflag == 1)
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", "--invert", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
              else
              {
                ret = execl(binaryPathjp2a, "jp2a", "-", (char *)NULL);
                if(ret == -1)
                {
                  perror("execl error");
                  exit(EXIT_FAILURE);
                }
              }
            }
          }
        }
      }
    }
    else
    {
      // parent process
      close(connfd);
    }
  }
}

// below is the signal handler to avoid zombie processes as
// well as the appropriate installer
void SIGCHLD_handler(int signo)
{
  pid_t pid;
  int stat;

  (void)signo;
  while((pid = waitpid(-1, &stat, WNOHANG)) > 0);
  return;
}

// installer for the SIGCHLD handler
void install_SIGCHLD_handler(void)
{
  struct sigaction act;
  // block all signals during exec of SIGCHLD_handler
  sigfillset(&act.sa_mask);
  act.sa_handler = &SIGCHLD_handler;
  // auto restart interrupted system calls
  act.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &act, NULL);
}

