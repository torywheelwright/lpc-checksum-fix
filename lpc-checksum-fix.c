/**
 * Inserts the checksum in an LPC1769 binary image
 *
 * Written on 2015 Oct 27 by Tory Wheelwright
 */

#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	FILE* in = NULL;
	FILE* out = NULL;
	uint32_t current_word;
	uint32_t checksum = 0;
	unsigned int i;

	if(argc != 3)
	{
		puts("Invalid arguments. Usage: checksum in.bin out.bin");
		return 1;
	}
	if(!(in = fopen(argv[1], "r")))
	{
		printf("Failed to open file %s\n", argv[1]);
		return 1;
	}
	if((out = fopen(argv[2], "r")))
	{
		printf("Output file %s already exists\n", argv[2]);
		fclose(in);
		return 1;
	}
	if(!(out = fopen(argv[2], "w")))
	{
		printf("Failed to open output file %s for writing\n", argv[2]);
		fclose(in);
		return 1;
	}

	for(i = 0;
	    i < 7 && fread(&current_word, sizeof(uint32_t), 1, in);
	    checksum += current_word, i++)
	{
		if(!fwrite(&current_word, sizeof(uint32_t), 1, out))
		{
			printf("Failed writing output file %s after %d words\n",
			       argv[2],
			       i);
			fclose(in);
			fclose(out);
			return 1;
		}
	}
	if(i != 7)
	{
		printf("Failed reading input file %s after %d words\n", argv[1], i);
		fclose(in);
		fclose(out);
		return 1;
	}
	checksum = -checksum;
	printf("Checksum: %x\n", checksum);
	// Throw away word at checksum location and replace with checksum
	if(!fread(&current_word, sizeof(uint32_t), 1, in))
	{
		printf("Failed reading input file %s after %d words\n", argv[1], i);
		fclose(in);
		fclose(out);
		return 1;
	}
	if(!fwrite(&checksum, sizeof(uint32_t), 1, out))
	{
		printf("Filed writing output file %s after %d words\n", argv[2], i);
		fclose(in);
		fclose(out);
		return 1;
	}
	i++;
	while(fread(&current_word, sizeof(uint32_t), 1, in))
	{
		if(!fwrite(&current_word, sizeof(uint32_t), 1, out))
		{
			printf("Failed writing output file %s after %d words\n",
			       argv[2],
			       i);
			fclose(in);
			fclose(out);
			return 1;
		}
		i++;
	}
	printf("Successfully wrote %d words to %s\n", i, argv[2]);
	fclose(in);
	fclose(out);
	return 0;
}
