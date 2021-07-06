#include "lib.h"

void halt(void);
char buffer[1 << 10];

static void user_myoutput(void *arg, const char *s, int l)
{
	int i;

	// special termination call
	/*
	if ((l == 1) && (s[0] == '\0'))
		return;
	for (i = 0; i < l; i++)
	{
		syscall_putchar(s[i]);
		if (s[i] == '\n')
			syscall_putchar('\n');
	}
	*/

	if ((l == 1) && (s[0] == '\0'))
		return;
	for (i = 0; i < l; i++)
		((char *)arg)[i] = s[i];
}

void writef(char *fmt, ...)
{
	va_list ap;
	user_bzero((void *)buffer, 1 << 10);
	va_start(ap, fmt);
	user_lp_Print(user_myoutput, buffer, fmt, ap);
	va_end(ap);
	syscall_puts(buffer);
}

void _user_panic(const char *file, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	writef("panic at %s:%d: ", file, line);
	user_lp_Print(user_myoutput, 0, (char *)fmt, ap);
	writef("\n");
	va_end(ap);
	for (;;);
}
