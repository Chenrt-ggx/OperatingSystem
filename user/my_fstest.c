#include "lib.h"

void umain()
{
	int n, r;

	writef("not fs read rtc\n");
	n = 48, r = syscall_read_dev(&n, 0x15000000, 4);
	writef("get %d, expected 0\n", r != 0);

	writef("not fs read console\n");
	n = 48, r = syscall_read_dev(&n, 0x10000000, 4);
	writef("get %d, expected 0\n", r != 0);

	writef("not fs read ide\n");
	n = 48, r = syscall_read_dev(&n, 0x13000000, 4);
	writef("get %d, expected 1\n", r != 0);

	writef("not fs write rtc\n");
	n = 48, r = syscall_write_dev(&n, 0x15000000, 4);
	writef("get %d, expected 0\n", r != 0);

	writef("not fs write console\n");
	n = 48, r = syscall_write_dev(&n, 0x10000000, 4);
	writef("get %d, expected 0\n", r != 0);

	writef("not fs write ide\n");
	n = 48, r = syscall_write_dev(&n, 0x13000000, 4);
	writef("get %d, expected 1\n", r != 0);

	while (1)
		writef("idle");
}
