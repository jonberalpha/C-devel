#include <stdio.h>
#include <stdlib.h>

#include <helper.h>

Person *init_struct()
{
  Person *p = (Person *)malloc(sizeof(Person));
  if (p == NULL)
  {
    exit(EXIT_FAILURE);
  }

  return p;
}

void free_struct(Person *p)
{
  if (p != NULL)
  {
    free(p);
    p = NULL;
  }
}
