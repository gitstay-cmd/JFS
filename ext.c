#include <stdint.h>
#include <stdlib.h>
#include <fs.h>
#include <string.h>
#include <ext.h>
#include <stdio.h>

int available_address[MAX_FILES] = {0, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096, 4608, 5120, 5632};


char *fs_name = "ext";

struct ext2_file *files[MAX_FILES];

struct ext2_file *is_present(const char *name){

    for(int i =0; i < MAX_FILES; i++){

        if(files[i] != NULL){

 			if(!strcmp(files[i]->name, name)){
     			return files[i];		
 			}

        }

    }
    return (struct ext2_file *)0;

}

uint32_t get_start_address(){
	int addr;
    for(int i =0; i < MAX_FILES; i++){

        if(available_address[i] != -1){
            addr = available_address[i];
            available_address[i] = -1;
            return addr;
        }

    }
    return -1;
}

struct ext2_file *_fopen(struct fs_t *fs, const char *name, char mode){
	struct ext2_file *fp;
	struct ext2_inode *in = malloc(sizeof(struct ext2_inode));
    if ((fp = is_present(name))){
        if(fp->open){
            return fp;
        }
        fp->in = in;
		fp->in->starting_address = fp->start_address;
        fp->fs = fs;
        fp->in->size = fp->size;
        fp->mode = mode;
        fp->pos = 0;
        fp->open = 1;
    	return fp;
    }else{
        for(int i = 0; i < MAX_FILES; i++){

            if(files[i] == NULL){
                
               	fp = (struct ext2_file*)malloc(sizeof(struct ext2_file));

               	files[i] = fp;
               	fp->in = in;

               	fp->start_address = get_start_address();
				fp->name = (char*)malloc(sizeof(name));
				fp->mode = mode;
				strcpy(fp->name, name);
               	if(fp->start_address == -1){
                   	return (struct ext2_file*)0;
               	}

				fp->in->starting_address = fp->start_address;
				fp->fs = fs;
				fp->in->size = fp->size = 0;
				fp->open = 1;
				fp->pos = 0;
				return fp;
            }
        }
        free(in);
        return (struct ext2_file *)0;
    }
}

/*struct dirent *make_dirent(char *path){
	char *tokenized = (char*)malloc(sizeof(path));
	strcpy(tokenized, path);
	char *token = strtok(path, "/");
}*/

int _fflush(struct ext2_file *fp){
    //to be implemented
    return 0;

}

int _fseek(struct ext2_file *fp, int offset, int whence){

    return 0;
}

int _fputc(struct ext2_file *fp, char c){

    if(fp->mode != 'w'){

        return 0;

    }
    char *cs = &c;

    _f_write(fp, (uint8_t*)&c, 1);

    return 0;
    
}

int _fputs(struct ext2_file *fp, char *c){
	if(fp->mode != 'w'){
    	return 0;
	}
	char *write = (char*)malloc(sizeof(c));
	strcpy(write, c);
	_f_write(fp, write, sizeof(write));
	return 0;
}

char _fgetc(struct ext2_file *fp){
	if(fp->mode != 'r'){
    	return 0;
	}
    char *c = malloc(sizeof(char));
    _f_fread(fp, (uint8_t*)c, 1); 
    char ret = *c;
    free(c);
    return ret;
}

char *_fgets(struct ext2_file *fp){
    if(fp->mode != 'r'){
        return 0;
    }
    char *str = calloc(81, sizeof(char));
    int i = 0; 
   	_f_fread(fp, (uint8_t*)str, sizeof(str));
    
    return str;
}

int _f_fread(struct ext2_file *fp, uint8_t *buf, size_t buf_size){
    return _fread(fp->fs, (void*)buf, buf_size, fp);
}

size_t _fread(struct fs_t *fs, void *ptr, size_t byte_size, struct ext2_file *stream){

    if((byte_size + stream->pos) > MAX_SIZE){
        byte_size = (MAX_SIZE - stream->pos);
    }
    
    fs->parent->read(fs->parent, (uint8_t*)ptr, byte_size , stream->pos, (stream->pos + stream->in->starting_address)/fs->block_size);    
   	stream->pos += byte_size;
   	return 1;
}

int fcreate(struct fs_t *fs, char *path){
    /*struct dirent *dir;*/
    return 1;
}
            
int _fclose(struct fs_t *fs, struct ext2_file *fp){
    free(fp->in);
    fp->open = 0;
    available_address[fp->start_address/5] = fp->start_address;
    return 0;
}

int _f_write(struct ext2_file *fp, uint8_t *buf, size_t buf_size){
    return _fwrite(fp->fs, (void*)buf, sizeof(buf), fp);
}

int _fsize(struct ext2_file *fp){
    return fp->in->size;
}

size_t _fwrite(struct fs_t *fs, void *ptr, size_t byte_size, struct ext2_file *stream){

	if((byte_size + stream->pos) > MAX_SIZE){
    	byte_size = (MAX_SIZE - stream->pos);
	}

	fs->parent->write(fs->parent, (uint8_t*)ptr, byte_size, stream->pos, (stream->pos + stream->in->starting_address)/fs->block_size);
	stream->in->size += byte_size;
	stream->pos += byte_size;
	return byte_size;
}

/*struct dirent *traverse_directory(const char *path, struct dirent *root){
	char *strpath = strdup(path);

    char *token = strtok(strpath, "/");
	
	while(token != NULL){

    	for(int i = 0; i < 3; i++){

        	if(root->children[i] == NULL){   
			}

        	else{

        		if(!strcmp(root->children[i]->name, token)){

            		root = root->children[i];

            		token = strtok(NULL, "/");

            		if(token == NULL){

                		return root;

            		}

        		}

        	}

    	}

	}

}*/

/*struct dirent *find_file(struct ext2_fs *fs, const char *path){
	if(path[0] == '/'){
    	return (struct dirent*)traverse_directory(&(path[1]),fs->root);
	}
	else{
    	return (struct dirent *)traverse_directory(path, fs->cur_dir); 
	}
}*/

int ext2_init(struct fs_t **fs){

	struct fs_t *ret = (struct fs_t *)malloc(sizeof(struct fs_t));

	memset(ret, 0, sizeof(struct fs_t));

	ret->fopen = _fopen;
	ret->fread = _fread;
	ret->fwrite = _fwrite;
	ret->fclose = _fclose;
	ret->fseek = _fseek;
	ret->fs_name = fs_name;
	ret->block_size = 512;
	*fs = (struct fs_t*)ret;

	return 1;
}

void list_files(char **args){

    for(int i = 0; i < MAX_FILES; i++){

		if(files[i] != NULL){ 
			
    		printf("FILE: %s", files[i]->name);
    		if(sizeof(args)/sizeof(char*) ==  1){
				if(files[i]->open){
    				printf("\tSize:%d", files[i]->in->size);
					printf("\tOPEN");
					printf("\t%c", files[i]->mode);
    			}	
    		}
    		printf("\n");
		}
		
    }
}
