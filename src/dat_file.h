#ifndef __DAT_FILE_H__
#define __DAT_FILE_H__

/* This is the generic interface to both DAT1 and DAT2 files */
struct dat_file_file
{
	size_t file_size;
	char *data;
};


void dat_file_file_free(struct dat_file_file *dff);

#endif /* __DAT_FILE_H__ */
