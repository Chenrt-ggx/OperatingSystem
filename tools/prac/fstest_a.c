#include "lib.h"

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void umain()
{
	char buf[512];
	int r, n, fdnum;

	// read link
	writef("^^^^^^^^^read link^^^^^^^^^\n");
	if ((r = open("/a.lnk", O_RDWR)) < 0)
		user_panic("open /a.lnk: %d", r);
	fdnum = r;
	writef("open is good\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /a.lnk: %d", r);
	if (strcmp(buf, diff_msg) != 0)
		user_panic("read returned wrong data");
	writef("read is good\n");

	if ((r = ftruncate(fdnum, 0)) < 0)
		user_panic("ftruncate: %d", r);
	seek(fdnum, 0);

	if ((r = write(fdnum, msg, strlen(msg) + 1)) < 0)
		user_panic("write /a.lnk: %d", r);
	if ((r = close(fdnum)) < 0)
		user_panic("close /a.lnk: %d", r);

	// read file
	writef("^^^^^^^^^read file^^^^^^^^^\n");
	if ((r = open("/newmotd", O_RDWR)) < 0)
		user_panic("open /newmotd: %d", r);
	fdnum = r;
	writef("open is good\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /newmotd: %d", r);
	if (strcmp(buf, msg) != 0)
		user_panic("read returned wrong data");
	writef("read is good\n");

	if ((r = ftruncate(fdnum, 0)) < 0)
		user_panic("ftruncate: %d", r);
	seek(fdnum, 0);
	if ((r = write(fdnum, diff_msg, strlen(diff_msg) + 1)) < 0)
		user_panic("write /newmotd: %d", r);
	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);

	// read link again
	writef("^^^^^^^^^read link again^^^^^^^^^\n");
	if ((r = open("/a.lnk", O_RDWR)) < 0)
		user_panic("open /a.lnk: %d", r);
	fdnum = r;
	writef("open is good\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /a.lnk: %d", r);
	if (strcmp(buf, diff_msg) != 0)
		user_panic("read returned wrong data");
	writef("read is good\n");

	if ((r = ftruncate(fdnum, 0)) < 0)
		user_panic("ftruncate: %d", r);
	seek(fdnum, 0);

	if ((r = write(fdnum, msg, strlen(msg) + 1)) < 0)
		user_panic("write /a.lnk: %d", r);
	if ((r = close(fdnum)) < 0)
		user_panic("close /a.lnk: %d", r);

	// remove link
	writef("^^^^^^^^^remove link^^^^^^^^^\n");
	if ((r = open("/a.lnk", O_RDONLY)) < 0)
		user_panic("open /a.lnk: %d", r);
	fdnum = r;
	writef("open again: OK\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /a.lnk: %d", r);
	if (strcmp(buf, msg) != 0)
		user_panic("read returned wrong data");
	writef("read again: OK\n");

	if ((r = close(fdnum)) < 0)
		user_panic("close /a.lnk: %d", r);
	writef("file rewrite is good\n");
	if ((r = remove("/a.lnk")) < 0)
		user_panic("remove /a.lnk: %d", r);
	if ((r = open("/a.lnk", O_RDONLY)) >= 0)
		user_panic("open after remove /a.lnk: %d", r);
	writef("link remove: OK\n");

	// read file again
	writef("^^^^^^^^^read file again^^^^^^^^^\n");
	if ((r = open("/newmotd", O_RDWR)) < 0)
		user_panic("open /newmotd: %d", r);
	fdnum = r;
	writef("open is good\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /newmotd: %d", r);
	if (strcmp(buf, msg) != 0)
		user_panic("read returned wrong data");
	writef("read is good\n");

	if ((r = ftruncate(fdnum, 0)) < 0)
		user_panic("ftruncate: %d", r);
	seek(fdnum, 0);
	if ((r = write(fdnum, diff_msg, strlen(diff_msg) + 1)) < 0)
		user_panic("write /newmotd: %d", r);
	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);

	// remove file
	writef("^^^^^^^^^remove file^^^^^^^^^\n");
	if ((r = open("/newmotd", O_RDONLY)) < 0)
		user_panic("open /newmotd: %d", r);
	fdnum = r;
	writef("open again: OK\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /newmotd: %d", r);
	if (strcmp(buf, diff_msg) != 0)
		user_panic("read returned wrong data");
	writef("read again: OK\n");

	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);
	writef("file rewrite is good\n");
	if ((r = remove("/newmotd")) < 0)
		user_panic("remove /newmotd: %d", r);
	if ((r = open("/newmotd", O_RDONLY)) >= 0)
		user_panic("open after remove /newmotd: %d", r);
	writef("file remove: OK\n");
	if ((r = open("/a.lnk", O_RDONLY)) >= 0)
		user_panic("open after remove /a.lnk: %d", r);
	writef("link remove: OK\n");

	while (1)
	{
		// writef("IDLE!");
	}
}
