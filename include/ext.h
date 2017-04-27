#ifndef ext_h_INCLUDED
#define ext_h_INCLUDED
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "fs.h"

#define MAX_FILES 12
#define MAX_BLOCKS 60
#define MAX_SIZE MAX_BLOCKS * 512
#define MAX_CHILDREN 3

struct ext2_inode{
    uint32_t starting_address;
    uint32_t cur_block;
    uint32_t size;
};

/*struct dirent{
    char *name;
    struct ext2_file *file;
    uint32_t start_address;
    uint32_t size;
    struct dirent *parent;
    struct dirent *children[MAX_CHILDREN];
};*/

/*struct ext2_fs{
    struct fs_t fs;
    uint32_t no_files;
    struct dirent *root;
    struct dirent *cur_dir;
};*/

struct ext2_file {
    char *name;
    char mode;
    bool open;
    uint32_t start_address;
    uint32_t size;
    uint32_t pos;
    struct fs_t *fs;
    struct ext2_inode *in;
};

/*struct dirent *traverse_directory(const char *path, struct dirent *root);
struct dirent *find_file(struct ext2_fs *fs, const char *path);*/
int _f_fread(struct ext2_file *fp, uint8_t *buf, size_t buf_size);

int ext2_init(struct fs_t **fs);

size_t _fread(struct fs_t *fs, void *ptr, size_t byte_size, struct ext2_file *stream);

char *_fgets(struct ext2_file *fp);

void list_files(char **args);

size_t _fwrite(struct fs_t *fs, void *ptr, size_t byte_size, struct ext2_file *stream);

int _fputs(struct ext2_file *fp, char *c);

int _fputc(struct ext2_file *fp, char c);

int _f_write(struct ext2_file *fp, uint8_t *buf, size_t buf_size);

char _fgetc(struct ext2_file *fp);
#endif // ext_h_INCLUDED

