#include <stdio.h>

int puts(const char* string) {
	int res = 0;
	while(string[res] != 0)
	{
		if(putchar(string[res]) == EOF)
			break;
		res++;
	}
	return res;
}
