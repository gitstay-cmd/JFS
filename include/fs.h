#ifndef fs_h_INCLUDED
#define fs_h_INCLUDED
#include "ext.h"
#include "block.h"

struct fs_t{
	struct block_t *parent;
	const char *fs_name;
	size_t block_size;

	struct ext2_file *(*fopen) (struct fs_t *, const char *path, char mode);
	size_t (*fread)(struct fs_t *, void *ptr, size_t byte_size, struct ext2_file *stream);
	size_t (*fwrite)(struct fs_t *, void *ptr, size_t byte_size, struct ext2_file *stream);
	int (*fclose) (struct fs_t *, struct ext2_file *fp);
	long (*fsize)(struct ext2_file *fp);
	int (*fseek)(struct ext2_file *fp, int offset, int whence);
	int (*fflush)(struct ext2_file *fp);
};


#endif // fs_h_INCLUDED

