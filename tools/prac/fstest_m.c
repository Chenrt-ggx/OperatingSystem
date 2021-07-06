#include "lib.h"

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void umain()
{
	char buf[512];
	int r, n, fdnum;

	if ((r = open("/newmotd", O_RDWR)) < 0)
		user_panic("open /newmotd: %d", r);
	fdnum = r;
	writef("open is good\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /newmotd: %d", r);
	if (strcmp(buf, diff_msg) != 0)
		user_panic("read returned wrong data");
	writef("read is good\n");

	if ((r = ftruncate(fdnum, 0)) < 0)
		user_panic("ftruncate: %d", r);
	seek(fdnum, 0);

	if ((r = write(fdnum, msg, strlen(msg) + 1)) < 0)
		user_panic("write /newmotd: %d", r);
	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);

	// get hash
	r = get_checksum("/newmotd");
	if (r < 0)
		user_panic("get hash failed");
	writef("^^^^^^^^^^^^^^^ hash is %d ^^^^^^^^^^^^^^^\n",
		   ((struct Filefd *)num2fd(r))->f_file.f_checksum);
	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);

	// get hash again
	r = get_checksum("/newmotd");
	if (r < 0)
		user_panic("get hash failed");
	writef("^^^^^^^^^^^^^^^ hash is %d ^^^^^^^^^^^^^^^\n",
		   ((struct Filefd *)num2fd(r))->f_file.f_checksum);
	if ((r = close(fdnum)) < 0)
		user_panic("close /newmotd: %d", r);

	// read again
	if ((r = open("/newmotd", O_RDONLY)) < 0)
		user_panic("open /newmotd: %d", r);
	fdnum = r;
	writef("open again: OK\n");

	if ((n = read(fdnum, buf, 511)) < 0)
		user_panic("read /newmotd: %d", r);
	if (strcmp(buf, msg) != 0)
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

	while (1)
	{
		// writef("IDLE!");
	}
}
