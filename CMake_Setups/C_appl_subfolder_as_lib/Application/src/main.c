#include <main.h>   // all system includes
#include <helper.h> // additional includes

int main()
{
  Person *p = init_struct();

  p->id = 8;
  strncpy(p->name, "Max Mustermann", sizeof(p->name));
  printf("Persondetails:\t%d\t%s\n", p->id, p->name);
  free_struct(p);

  return 0;
}
