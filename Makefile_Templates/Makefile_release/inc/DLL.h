#ifndef DLL_H_INCLUDED
#define DLL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/******* TYPE DEFINITIONS ******/
typedef struct DLL
{
  int data;
  struct DLL *prev;
  struct DLL *next;
}DLL;

/******* PUBLIC PROTOTYPES ****/
int addFirst(DLL **head, int data);
int addAfterNode(DLL *prev, int data);
int addLast(DLL **head, int data);
void printDLL(DLL *node);

#endif /* DLL_H_INCLUDED */
