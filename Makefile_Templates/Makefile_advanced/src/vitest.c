#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <vitest.h>

#define PI 3.14159

int main(int argc, char **argv)
{
	(void)argc;
	Person p = {PI, "Franz B"};

	double c = acos(0.59);

	fprintf(stdout, "%s\n", argv[1]);
	fprintf(stdout, "%f\n", p.id);
	fprintf(stdout, "%s\n", p.name);

	return 0;
}
