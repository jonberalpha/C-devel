#pragma once

// Structures
typedef struct Person
{
  int id;
  char name[40];
} Person;

// Prototypes
Person *init_struct();
void free_struct(Person *p);
