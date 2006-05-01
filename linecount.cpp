/*++
Module Name: linecount.cpp
Abstract: This software counts the number of lines in text files.
Author: George V. Sudarkoff -- 3/12/2000 4:20:27 [version 1.0]
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("LineCount Y2K, Copyright (c) 2000 by C-Dixi Software LLC\n"
			"Usage:\tlinecount <mask.ext>\n");
		return (-1);
	}

	unsigned long counter = 0;
	unsigned int files = 0;
	unsigned long size = 0;

	for (int i = 1; i < argc; ++i)
	{
		int finish = 0;
		struct _finddata_t fileinfo;
		unsigned long ff = _findfirst (argv[i], &fileinfo);
		while (!finish) 
		{
			char fname[_MAX_PATH];
			strcpy(fname, fileinfo.name);	// take next file

			printf("Processing file: %s\n", fname);
			FILE* fd = fopen(fname, "rb");	// open it
			if (!fd) 
			{ 
				printf("Error: can't open file '%s'\n", argv[2]); 
				return (-2); 
			}

			files++;
			size += fileinfo.size;
			while (!feof(fd))				// and count lines
			{
				char ch;
				fread(&ch, 1, 1, fd);
				if (ch == '\n') counter++;
			}

			finish = _findnext(ff, &fileinfo);
		}
	}

	printf("\nSummary: %d lines | %d bytes | %d files\n", counter, size, files);
	return counter;
}
