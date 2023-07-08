#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <helper.h>

int main()
{
  Person *p = init_struct();

  p->id = 8;
  strncpy(p->name, "Max Mustermann", sizeof(p->name));
  printf("Persondetails:\t%d\t%s\n", p->id, p->name);
  free_struct(p);

  return 0;
}
