#include <stdio.h>

char buffer[1 << 22];

inline void f1()
{
	FILE *in = fopen("fs.img", "rb");
	if (in == NULL)
		return;
	int i, tot = fread(buffer, 1, 1 << 22, in);
	printf("%d\n", tot);
	for (i = 0; i < tot; i++)
	{
		if (buffer[i] != 0)
			printf("%d %d\n", i, buffer[i]);
	}
	putchar('\n');
	return;
}

inline void f2()
{
	int x, y;
	while (~scanf("%d %d", &x, &y))
		buffer[x] = y;
	FILE *out = fopen("fs.img", "wb");
	if (out == NULL)
		return;
	fwrite(buffer, 1, 1 << 22, out);
	return;
}

int main()
{
	// f1();
	f2();
	return 0;
}
