/**
 * Inserts the proper checksum in a binary image for an NXP LPC ARM Cortex based
 * microcontroller.
 *
 * Written on 2015 Oct 27 by Tory Wheelwright
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

FILE* in = NULL;
FILE* out = NULL;

void exit_on_error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	if(in)
	{
		fclose(in);
	}
	if(out)
	{
		fclose(out);
	}
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	uint32_t current_word;
	uint32_t checksum = 0;
	unsigned int i;

	// Validate arguments and ensure files can be opened
	if(argc != 3)
	{
		exit_on_error("Invalid arguments. Usage: checksum in.bin out.bin\n");
	}
	if(strcmp(".bin", &argv[1][strlen(argv[1]) - 4]))
	{
		exit_on_error("Input file %s is incorrect format; expecting .bin\n",
		              argv[1]);
	}
	if(strcmp(".bin", &argv[2][strlen(argv[2]) - 4]))
	{
		exit_on_error("Output file %s is incorrect format; expecting .bin\n",
		              argv[2]);
	}
	if(!(in = fopen(argv[1], "r")))
	{
		exit_on_error("Failed to open file %s\n", argv[1]);
	}
	if((out = fopen(argv[2], "r")))
	{
		exit_on_error("Output file %s already exists\n", argv[2]);
	}
	if(!(out = fopen(argv[2], "w")))
	{
		exit_on_error("Failed to open output file %s for writing\n", argv[2]);
	}
	// Read first 6 words through and calculate checksum
	for(i = 0;
	    i < 7 && fread(&current_word, sizeof(uint32_t), 1, in);
	    checksum += current_word, i++)
	{
		if(!fwrite(&current_word, sizeof(uint32_t), 1, out))
		{
		    exit_on_error("Failed writing output file %s after %d words\n",
		                  argv[2],
		                  i);
		}
	}
	if(i != 7)
	{
		exit_on_error("Failed reading input file %s after %d words\n",
		              argv[1],
		              i);
	}
	// Checksum is 2's complement of sum of first 6 words
	checksum = -checksum;
	printf("Checksum: %x\n", checksum);
	// Throw away word at checksum location and replace with checksum
	if(!fread(&current_word, sizeof(uint32_t), 1, in))
	{
		exit_on_error("Failed reading input file %s after %d words\n",
		              argv[1],
		              i);
	}
	if(!fwrite(&checksum, sizeof(uint32_t), 1, out))
	{
		exit_on_error("Filed writing output file %s after %d words\n",
		              argv[2],
		              i);
	}
	i++;
	// Read remaining words through
	while(fread(&current_word, sizeof(uint32_t), 1, in))
	{
		if(!fwrite(&current_word, sizeof(uint32_t), 1, out))
		{
			exit_on_error("Failed writing output file %s after %d words\n",
			              argv[2],
			              i);
		}
		i++;
	}
	printf("Successfully wrote %d words to %s\n", i, argv[2]);
	fclose(in);
	fclose(out);
	return EXIT_SUCCESS;
}
