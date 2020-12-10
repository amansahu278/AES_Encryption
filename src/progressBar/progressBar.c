#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PERCENTAGE(V,T) (100 - (((T-V)*100)/T))

void showProgress(size_t count, size_t max)
{
	const char prefix[] = "Progress: [";
	const char suffix[] = "]";
	const size_t prefix_length = sizeof(prefix) - 1;
	const size_t suffix_length = sizeof(suffix) - 1;
	char *buffer = calloc(max + prefix_length + suffix_length + 1, 1); // +1 for \0
	size_t i = 0;

	strcpy(buffer, prefix);
	for (; i < 100; ++i)
	{
		buffer[prefix_length + i] = i < PERCENTAGE(count, max) ? '#' : ' ';
	}

	strcpy(&buffer[prefix_length + i], suffix);
	printf("\b\r%c[2K\r%s", 27, buffer);
	printf("%ld%%", PERCENTAGE(count, max));
	fflush(stdout);
	free(buffer);
}

