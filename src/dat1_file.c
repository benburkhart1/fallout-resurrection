/* DAT1 File Interface
 *
 * This file is unnecessarily complex.
 * I just did things for verbosity in places
 * and because copy/paste made it too easy to find
 * the "right" way of doing things.
 * 
 * FIXME: Check read() output
 * FIXME: Check endianness before flipping
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

#include "util.h"
#include "util-endian.h"
#include "dat1_file.h"
#include "dat_file.h"

#define DAT1_DICT_SIZE 4096
#define DAT1_MIN_MATCH 3
#define DAT1_MAX_MATCH 18

/* #define MY_DEBUG 1 * Debug */
void dat1_file_free(struct dat1_file *df)
{
	void                            *swapptr = NULL;
	struct dat1_directory_name_list *tmpddnl = NULL;

	free(df->filename);
	free(df->dat1_file_header);

	tmpddnl = *df->dat1_directory_name_list;

	return;
}	

/* 
 * This is the DAT1 LZSS decompression implementation.
 * I gleamed information from http://falloutmods.wikia.com/wiki/DAT_file_format
 * but the documentation was pretty bad.
 */
char *dat1_decompress(const char *data, size_t compressed_size, size_t uncompressed_size)
{
	short nb;
	short tmpshort;
	int retoff;
	unsigned short do_r; /* Dictionary Offset (Read) */
	unsigned short di_w; /* Dictionary Index (Write) */
	int ml;   /* Match Length */
	int cs;   /* Compression Status for the next 8 characters */
	unsigned char fl;
	unsigned char l;
	unsigned char rdo;
	char dict[DAT1_DICT_SIZE];
	char *ret = NULL;
	size_t cont;
	int i;
	int j;
	int k;

	/* See if it's decompressed already */
	if (compressed_size == 0)
	{
		/* Expecting to be able to free output of this function */
		if (uncompressed_size > 0)
			return tstrdup(data);
	}
	
	nb   = 0;
	do_r = 0;
	ml   = 0;
	cs   = 0;

	ret    = tmalloc0(uncompressed_size + 1);
	cont   = 0;
	retoff = 0;

	/* cont = 2455, compressed_size = 2441, nb = -14, retoff = 4449 */
	while (cont < compressed_size)
	{
		/* Read NB, this is the amount of bytes we're going to read, I think this is always
     * compressed_size - 2? Not sure about that though, seems retarded, but what the hell
     * do I know.
     */
		memcpy(&tmpshort, &data[cont], 2);
		nb = SWAP_UINT16(tmpshort);
		cont+=2;

		/* Obviously exit if we read 0 */
		if (nb == 0)
			return ret;

		if (nb > 0)
		{
			/* Clear dictionary (Fill with spaces \x20) */
			memset(dict, ' ', sizeof(dict));
			
			/* DI = DICT_SIZE - MAX_MATCH; */
			di_w = DAT1_DICT_SIZE - DAT1_MAX_MATCH;

			while (nb > 0 && cont < compressed_size)
			{
				/* Read 1 byte for FLAG */
				fl = data[cont];
				cont++;
				nb--;

				/* printf("Read flag as %d\n",fl); */
				if (nb <= 0 && cont < compressed_size)
					break;
				
				for (i=0;i<8;i++)
				{
					if (nb<=0 || cont > compressed_size)
						break;

					if ((fl % 2) == 0)
					{
						/* IsEven */
						/* Read DO from input. */
						rdo = data[cont];
						cont++;
						nb--;

						/* If N bytes have been read from input, go to @Start (in a correctly compressed file this should not ever happen). */
						if (nb <= 0 || cont > compressed_size)
							break;

						/* Read L from input. */
						l = data[cont];
						cont++;
						nb--;
						
						if (nb <= 0 && cont < compressed_size)
							break;

						/* Prepend the high-nibble (first 4 bits) from L to DO (DO = DO | ((L & 0xF0) << 4)) and remove it from L (L = L & 0x0F). */
						do_r = rdo | ((l & 0xF0) << 4);
						l    = l & 0x0F;

						/* (L + MIN_MATCH) times: */
						for (j = 0; j < (l + DAT1_MIN_MATCH); j++)
						{
							/* Read a byte from dictionary at offset DO (wrap to the start of dictionary if past the end), and write to the output. */
							ret[retoff] = dict[do_r];
							retoff++;
							
							/* Write the byte to the Dictionary also, at position DI. */
							dict[di_w] = dict[do_r];

							/* DI = DI + 1, or DI = 0 (if past the end of Dictionary). */
							di_w++;
	            if (di_w >= DAT1_DICT_SIZE)
  	            di_w = 0;

							do_r++;
							/* DO = DO + 1. */
							if (do_r >= DAT1_DICT_SIZE)
								do_r = 0;
						}
					}
					else
					{
						/* IsOdd */
						/* Read 1 byte from input, write it to output and to Dictionary (at position DI). */
						dict[di_w]    = data[cont];
						ret[retoff]   = dict[di_w];
						nb--;
						cont++;
						retoff++;
					
						/* If N bytes have been read from input, go to @Start. */
						if (nb <= 0 || cont >= compressed_size)
							break;
	
						/* DI = DI + 1, or DI = 0 (if past the end of Dictionary). */
						di_w++;
						
						if (di_w >= DAT1_DICT_SIZE)
							di_w = 0;
					}

					/* Divide by 2 rounding down */
					fl = fl >> 1;
				}
			}
		} 
		/* I guess this is another way of just saying, "Hey this is uncompressed data" */
		else if (nb < 0)
		{
			/* Take the absolute value of N (or multiply N by -1), and write that many bytes directly from input to output (without
			     putting anything in Dictionary). */
			memcpy(&ret[retoff], &data[cont], nb);
			cont += nb;
			retoff+=nb;
			
			/* Go to @Start */
		}
	}
	
	return ret;
}

/* This function gets the raw file data from a file within a DAT1 file. */
struct dat_file_file *dat_file_file_from_dat1_file(struct dat1_file *df, const char *filename)
{
	struct dat_file_file           *dff   = NULL;
	struct dat1_directory_content   *dc   = NULL;
	struct dat1_file_list           *dfl  = NULL;
	int i;
	int j;
	unsigned int data_offset = 0;
	char *ubuf;

	/* Sanity */
	if (df == NULL || filename == NULL)
		return NULL;

	for (i = 0; i < df->dat1_file_header->directory_count; i++)
	{
		/* Parent node */
		dc = df->dat1_directory_content[i];
			
		for (j = 0; j < dc->file_count; j++)
		{
			dfl = dc->dat1_file_list[j];

			/* printf("%s\n",dfl->name); */
			if (!tstrcasecmp(filename, dfl->name))
			{
				printf("Found %s in %s\n",dfl->name, df->dat1_directory_name_list[i]->value);
				dff = tmalloc(sizeof(struct dat_file_file));
				break;
			}
		}

		/* Found it, break out of this for also */
		if (j != dc->file_count)
			break;
	}

	/* Did Not find it */
	if (dff == NULL)
		return NULL;

	/* need to look up the offset contained in the dat1_file_list structure 
	 * which is contained in the dat1_directory_content structure 
	 */
	data_offset = dfl->offset;

	/* need to locate that location in the raw data chunk of the DAT1 file */
	/* need to determine if the file is compressed or not */
	/* If file is compressed, need to read compressed_size bytes into 1 buffer */
#ifdef MY_DEBUG	
	printf("Offset is %x\n",dfl->offset);
	printf("Compressed Size is %d\n",dfl->compressed_size);
	printf("Uncompressed Size is %d\n",dfl->uncompressed_size);
	printf("Attributes is %d\n",dfl->attributes);
#endif /* MY_DEBUG */
	
	/* buffer needs allocated for uncompressed_size amount of bytes for file data */
	if (dfl->compressed_size + dfl->uncompressed_size > 0)
	{
		ubuf = tmalloc0((dfl->compressed_size > 0) ? dfl->compressed_size : dfl->uncompressed_size + 1);

		if (df->data == NULL)
		{
			fseek(df->fp, (long)dfl->offset, SEEK_SET);
			i = fread(ubuf, (dfl->compressed_size > 0) ? dfl->compressed_size : dfl->uncompressed_size, 1, df->fp);
			dff->data      = dat1_decompress(ubuf, dfl->compressed_size, dfl->uncompressed_size);
			dff->file_size = dfl->uncompressed_size; 
		}
		else
		{
			/* Should be the only way */
#ifdef DATA_BLOCK_TO_RAM
#endif /* DATA_BLOCK_TO_RAM */
		}
	}

	return dff;
}

struct dat1_file *dat1_file_new_from_filename(const char *filename)
{
	FILE *fd;
	struct dat1_file *df;
	int tmpint;
	unsigned char mybyte;
	int i;
	int j;
	long data_start;
	long data_end;

	df = NULL;

	if ((fd = fopen(filename,"rb")) == NULL)
	{
		printf("Could not open %s\n",filename);
		return df;
	}

	df = tmalloc(sizeof(struct dat1_file));

	df->filename = tstrdup(filename);
	
	/* Start Header */
	df->dat1_file_header = tmalloc(sizeof(struct dat1_file_header));
		
	/* FIXME: Check endian before swapping, laziness */
	fread((char *)&tmpint, 4, 1, fd);
	df->dat1_file_header->directory_count = SWAP_UINT32(tmpint);
	
	fread((char *)&tmpint, 4, 1, fd);
	df->dat1_file_header->unknown1 = SWAP_UINT32(tmpint);

	fread((char *)&tmpint, 4, 1, fd);
	df->dat1_file_header->unknown2 = SWAP_UINT32(tmpint);

	fread((char *)&tmpint, 4, 1, fd);
	df->dat1_file_header->unknown3 = SWAP_UINT32(tmpint);
	/* End Header */

	/* Start Directory List */
	/* Let's allocate enough memory for the whole shebang + 1 NULL */
	df->dat1_directory_name_list = tmalloc0(sizeof(struct dat1_directory_name_list) * df->dat1_file_header->directory_count + 1);
	
	/* One for each directory */
	for (i=0;i < df->dat1_file_header->directory_count;i++)
	{
		/* Initialization */
		df->dat1_directory_name_list[i] = tmalloc(sizeof(struct dat1_directory_name_list));
		df->dat1_directory_name_list[i]->length = 0;
		df->dat1_directory_name_list[i]->value  = NULL;
		mybyte = 0;
		
		/* The length is in a byte */
		fread((char *)&mybyte, 1, 1, fd);
		df->dat1_directory_name_list[i]->length = mybyte;

		/* copy over a string length of the previous byte's value */
		df->dat1_directory_name_list[i]->value = tmalloc0(df->dat1_directory_name_list[i]->length + 1);
		fread(df->dat1_directory_name_list[i]->value, df->dat1_directory_name_list[i]->length, 1, fd);

#ifdef MY_DEBUG
		printf("%d (%s) {%d}\n",i,df->dat1_directory_name_list[i]->value, df->dat1_directory_name_list[i]->length);
#endif /* MY_DEBUG */
	}
	/* End Directory List */

	/* Start Directory Content */
	df->dat1_directory_content = tmalloc0(sizeof(struct dat1_directory_content) * df->dat1_file_header->directory_count);
	
	/* One for each directory */
	for (i=0;i < df->dat1_file_header->directory_count;i++)
	{
		/* Initialization */
		df->dat1_directory_content[i] = tmalloc(sizeof(struct dat1_directory_content));
		df->dat1_directory_content[i]->file_count = 0;
		df->dat1_directory_content[i]->unknown1   = 0;
		df->dat1_directory_content[i]->unknown2   = 0;
		df->dat1_directory_content[i]->unknown3   = 0;

		/* FIXME: Check endian before swapping unnecessarily */
		fread((char *)&tmpint, 4, 1, fd);
		df->dat1_directory_content[i]->file_count = SWAP_UINT32(tmpint);

		fread((char *)&tmpint, 4, 1, fd);
		df->dat1_directory_content[i]->unknown1 = SWAP_UINT32(tmpint);

		fread((char *)&tmpint, 4, 1, fd);
		df->dat1_directory_content[i]->unknown2 = SWAP_UINT32(tmpint);

		fread((char *)&tmpint, 4, 1, fd);
		df->dat1_directory_content[i]->unknown3 = SWAP_UINT32(tmpint);

#ifdef MY_DEBUG
		printf("%s (%d) - 1 (%d:%x) 2 (%d:%x) 3 (%d:%x) [%x]\n", df->dat1_directory_name_list[i]->value, df->dat1_directory_content[i]->file_count, df->dat1_directory_content[i]->unknown1, df->dat1_directory_content[i]->unknown2, df->dat1_directory_content[i]->unknown3, df->dat1_directory_content[i]->unknown1, df->dat1_directory_content[i]->unknown2, df->dat1_directory_content[i]->unknown3, ftell(fd));
#endif /* MY_DEBUG */

		/* Now read the file list */
		df->dat1_directory_content[i]->dat1_file_list = tmalloc0(sizeof(struct dat1_file_list) * df->dat1_directory_content[i]->file_count);

		/* One for each directory */
		for (j=0;j < df->dat1_directory_content[i]->file_count;j++)
		{
			/* Initialization */
			df->dat1_directory_content[i]->dat1_file_list[j] = tmalloc(sizeof(struct dat1_file_list));
			df->dat1_directory_content[i]->dat1_file_list[j]->length            = 0;
			df->dat1_directory_content[i]->dat1_file_list[j]->name              = NULL;
			df->dat1_directory_content[i]->dat1_file_list[j]->attributes        = 0;
			df->dat1_directory_content[i]->dat1_file_list[j]->offset            = 0;
			df->dat1_directory_content[i]->dat1_file_list[j]->uncompressed_size = 0;
			df->dat1_directory_content[i]->dat1_file_list[j]->compressed_size   = 0;

			/* The length is in a byte */
			fread((char *)&mybyte, 1, 1, fd);
			df->dat1_directory_content[i]->dat1_file_list[j]->length = mybyte;

			/* copy over a string length of the previous byte's value */
			df->dat1_directory_content[i]->dat1_file_list[j]->name = tmalloc0(df->dat1_directory_content[i]->dat1_file_list[j]->length + 1);
			fread(df->dat1_directory_content[i]->dat1_file_list[j]->name, df->dat1_directory_content[i]->dat1_file_list[j]->length, 1, fd);

#ifdef MY_DEBUG
			printf("File Name: %s\n",df->dat1_directory_content[i]->dat1_file_list[j]->name);
#endif /* MY_DEBUG */

			fread((char *)&tmpint, 4, 1, fd);
			df->dat1_directory_content[i]->dat1_file_list[j]->attributes = SWAP_UINT32(tmpint);

			fread((char *)&tmpint, 4, 1, fd);
			df->dat1_directory_content[i]->dat1_file_list[j]->offset     = SWAP_UINT32(tmpint);

			fread((char *)&tmpint, 4, 1, fd);
			df->dat1_directory_content[i]->dat1_file_list[j]->uncompressed_size = SWAP_UINT32(tmpint);

			fread((char *)&tmpint, 4, 1, fd);
			df->dat1_directory_content[i]->dat1_file_list[j]->compressed_size = SWAP_UINT32(tmpint);
		}


	}
	/* End Directory Content */
	df->data = NULL;
	df->fp   = fd;

	/* Calculate size */
	data_start = ftell(fd);
	
	fseek(fd, 0, SEEK_END);
	
	data_end = ftell(fd);
	
	df->data_size = data_end - data_start;

#ifdef DATA_BLOCK_TO_RAM
	/* Read the rest of the file into a character array */
  fbuf = tmalloc0(1024);

  /* Read the entire file into memory */
  for(i=0;(count = fread(&fbuf[i],1,1024,fd)) == 1024;i+=1024)
  {
    if ((fbuf = realloc(fbuf,i+1024+1024)) == NULL)
    {
			printf("Ran out of memory allocating for data block of DAT1 file\n");
      exit(EXIT_FAILURE);
    }
  }

  /* Terminate it with a NULL */
  fbuf[i+count] = '\0';

	printf("Read total size of %d\n",i+count);
	df->data_size = i+count;

	df->data = fbuf;

	fclose(fd);
	df->fp = NULL;
#endif /* DATA_BLOCK_TO_RAM */


#ifdef MY_DEBUG
	printf("Directory Count is: %d\n", df->dat1_file_header->directory_count);
	printf("1 (%d:%x) 2 (%d:%x) 3 (%d:%x)\n", df->dat1_file_header->unknown1, df->dat1_file_header->unknown2, df->dat1_file_header->unknown3, df->dat1_file_header->unknown1, df->dat1_file_header->unknown2, df->dat1_file_header->unknown3);
#endif /* MY_DEBUG */

	return df;
}
