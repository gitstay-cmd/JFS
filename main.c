#include <stdio.h>
#include <jsh.h>
#include <ext.h>
#include <string.h>
#include <membox.h>

struct fs_t *fs;

#define MAX_OPEN_FILES 12

extern uint8_t *memory;
struct ext2_file *open_files[MAX_OPEN_FILES];
int place = 0;
int open_file_no = 0;

int ls(char **args){
    list_files(args);
}

int get_size(char **args){
    int count = 0;
    while(*args != NULL){
        count++;
        args++;
    }
    return count;
}

int _display_mem(char **args){

    for(int i = 0; i < 60; i++){
        for(int j = 0; j < 512; j++){
            if((memory[i*j + j]) == 0){
                printf("0 ");
            }else{
                printf("%c ", (char)memory[i*j + j]);
            }
    	}
    	printf("\n");
    }
}

int __fwrite(char **args){

	if(get_size(args) < 2){
    	printf("usage: fw <file_name> <word>\n");
    	return 0;
	}
	for(int i =0; i < MAX_OPEN_FILES; i++){

    	if(!strcmp(open_files[i]->name, args[0])){

        	if(!open_files[i]->open){
				printf("Please open file before attempting to write to it\n");
				return 0;
        	}

        	_fputs(open_files[i], args[1]);
        	return 0;
        }

	}

}

int __get_pos(char **args){
    if(get_size(args) < 1){
        return 0;
    }
	for(int i =0; i < MAX_OPEN_FILES; i++){

    	if(!strcmp(open_files[i]->name, args[0])){

        	if(!open_files[i]->open){
				printf("Please open file before attempting to read its data\n");
				return 0;
        	}

        	printf("%d\n", open_files[i]->pos);
        	return 0;
        }

	}
    
}

int __fclose(char **args){

	if(get_size(args) < 1){
    	printf("usage: fc <file_name>\n");
    	return 0;
	}

	for(int i =0; i < MAX_OPEN_FILES; i++){

    	if(!strcmp(open_files[i]->name, args[0])){
			fs->fclose(fs, open_files[i]);
			return 0;
		}

	}
    
}

int get_place(){
    for(int i =0; i < MAX_OPEN_FILES; i++){
        if(open_files[i] == NULL){
            return i;
        }
    }
    return -1;
}

int fropen(char **args){
    if(get_size(args) < 2){
        printf("usage: fo <file_name> <read/write>\n");
        return 0;
    }
    if(open_file_no == MAX_OPEN_FILES){
        printf("too many open files\n");
        return 0;
    }
    place = get_place();
    if(args[1][0] == 'r'){
    	open_files[place] = fs->fopen(fs, args[0], 'r');
    }else if(args[1][0] == 'w'){
        open_files[place] = fs->fopen(fs, args[0], 'w');
    }
    if(open_files[place] != NULL){
        open_file_no++;
    }
    return 0;
}

int __fread(char **args){

	if(get_size(args) < 1){
    	printf("usage: fr <file_name>");
	}

    for(int i = 0; i < 3; i++){

		if(!strcmp(open_files[i]->name, args[0])){
			char *buffer = _fgets(open_files[i]);
			if(buffer != NULL){
				printf("%s\n", buffer);
			}
			return 0;
		}

    }

}

int __fread_char(char **args){

	if(get_size(args) < 1){
    	printf("usage: fr <file_name>");
	}

	for(int i = 0; i < MAX_OPEN_FILES; i++){

		if(!strcmp(open_files[i]->name, args[0])){
			char buf = _fgetc(open_files[i]);
			printf("%c\n", buf);
			return 0;
		}

	}
}

int main(void){

	ext2_init(&fs);

	struct membox *mem = init();
	fs->parent = (struct block_t*)mem;
	mem->block.fs = fs;

	register_command("ls", ls);
	
	register_command("fr", __fread);

	register_command("fo", fropen);

	register_command("fw", __fwrite);

	register_command("fc", __fclose);

	register_command("dm", _display_mem);

	register_command("gp", __get_pos);

	register_command("frc", __fread_char);
	lsh_loop();

	return 0;

}
