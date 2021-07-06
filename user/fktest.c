#include "lib.h"

void umain()
{
	int a = 0, id = 0;
	if ((id = fork()) == 0)
	{
		if ((id = fork()) == 0)
		{
			for (a += 3;;)
				writef("\t\tthis is child2 :a:%d\n", a);
		}
		for (a += 2;;)
			writef("\tthis is child :a:%d\n", a);
	}
	for (a++;;)
		writef("this is father: a:%d\n", a);
}
