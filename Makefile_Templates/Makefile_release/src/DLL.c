#include <stdio.h>
#include <stdlib.h>

#include <DLL.h>

int addFirst(DLL **head, int data)
{
  int ret = 0;
  
  DLL *newnode = (DLL*)malloc(sizeof(DLL));
  if(newnode == NULL)
  {
    fprintf(stderr, "Cannot allocate memory for new node!\n");
    exit(EXIT_FAILURE);
  }
  
  newnode->data = data;
  newnode->next = *head;
  newnode->prev = NULL;
  
  if(*head != NULL)
  {
    (*head)->prev = newnode;
  }

  *head = newnode;
  
  return ret;
}

int addAfterNode(DLL *prev, int data)
{
  int ret = 0;
  
  if(prev == NULL) 
  {
    fprintf(stderr, "Given previous node cannot be NULL!\n");
    ret = -1;
  }
  else
  {
    DLL *newnode = (DLL*)malloc(sizeof(DLL));
    if(newnode == NULL)
    {
      fprintf(stderr, "Cannot allocate memory for new node!\n");
      exit(EXIT_FAILURE);
    }

    newnode->data = data;
    newnode->next = prev->next;
    prev->next = newnode;
    newnode->prev = prev;

    if(newnode->next != NULL)
    {
      newnode->next->prev = newnode;
    }
  }

  return ret;
}

int addLast(DLL **head, int data)
{
  int ret = 0;
  
  DLL *newnode = (DLL*)malloc(sizeof(DLL));
  if(newnode == NULL)
  {
    fprintf(stderr, "Cannot allocate memory for new node!\n");
    exit(EXIT_FAILURE);
  }
  
  DLL *last = *head;
  newnode->data = data;
  newnode->next = NULL;

  if(*head == NULL)
  {
    newnode->prev = NULL;
    *head = newnode;
    ret = -1;
  }
  else
  {
    while(last->next != NULL)
    {
      last = last->next;
    }

    last->next = newnode;
    newnode->prev = last;
  }

  return ret;
}

void printDLL(DLL *node)
{
    DLL *last = NULL;
    printf("\nTraversal in forward direction \n");
    while(node != NULL)
    {
      fprintf(stdout, "%d\n", node->data);
      last = node;
      node = node->next;
    }
 
    fprintf(stdout, "\nTraversal in reverse direction \n");
    while(last != NULL)
    {
      fprintf(stdout, "%d\n", last->data);
      last = last->prev;
    }
}

