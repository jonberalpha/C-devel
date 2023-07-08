#include <stdio.h>
#include <stdlib.h>

#include <DLL.h>

int main()
{
  DLL *head = NULL;

  addLast(&head, 6);
  addFirst(&head, 7);
  addFirst(&head, 1);
  addLast(&head, 4);
  addAfterNode(head->next, 8);

  fprintf(stdout, "Created DLL is: \n");
  printDLL(head);

  return 0;
}
