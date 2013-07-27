#ifndef __DAT2_FILE_H__
#define __DAT2_FILE_H__

/* Uncomment for additional debugging output */
/* #define MY_DEBUG 1 */

/* 
 * This file handles DAT2 (Fallout 2) Files.
 *
 * ChangeLog:
 *   2009-09-22 - Initial writing. [BB]
 */
/* This should be generic */
struct dat2_file_file
{
	size_t file_size;
	unsigned char *data;
};

struct dat2_file_header
{
	int directory_count;

	/* These values are apparently unknown */
	int unknown1;
	int unknown2;
	int unknown3;
};

struct dat1_file_list
{
	unsigned char length;
	char *name;
	unsigned int attributes;
	unsigned int offset;
	
	unsigned int uncompressed_size;
	unsigned int compressed_size;
};
	
struct dat1_directory_name_list
{
	unsigned char length;
	char *value;
};

struct dat1_directory_content
{
	int file_count;
	int unknown1;
	int unknown2;
	int unknown3;

	struct dat1_file_list **dat1_file_list;
};

struct dat1_file
{
	char *filename;

	struct dat1_file_header *dat1_file_header;
	
	struct dat1_directory_name_list **dat1_directory_name_list;
	struct dat1_directory_content   **dat1_directory_content;

	/* Raw File Data */
	char *data;
	size_t data_size;
	
	FILE *fp;
};	
	
char *dat1_decompress(const char *data, size_t compressed_size, size_t uncompressed_size);
struct dat1_file_file *dat1_file_file_from_dat1_file(struct dat1_file *df, const char *filename);
struct dat1_file *dat1_file_new_from_filename(const char *filename);

#endif /* __DAT2_FILE_H__ */
