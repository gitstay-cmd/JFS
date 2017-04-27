#include <jsh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 32

struct command *commands[MAX_COMMANDS];

void register_command(const char *name, int (*command)(char **args)){

    for(int i =0; i < MAX_COMMANDS; i++){

		if(commands[i] == NULL){

			struct command *mand = malloc(sizeof(commands));

			mand->command = command;

			mand->name = malloc(sizeof(name));

			strcpy(mand->name, name);

			commands[i] = mand;

			return;

		}

    }

}

void list_commands(){

    for(int i =0; i < MAX_COMMANDS; i++){
        if(commands[i] != NULL){
			printf("%s\n", commands[i]->name);
    	}
    }

}

struct command *find_command(const char *name){

	if(name == NULL){
    	return (struct command*)0;
	}
	for(int i =0; i < MAX_COMMANDS; i++){

		if(commands[i] != NULL){
			if(!strcmp(commands[i]->name, name)){
				return commands[i];

			}
		}

	}

	return (struct command*)0;

}

int lsh_execute(char **args){

	struct command *comm;

	if((comm = find_command(args[0])) != 0){

    	comm->command(&args[1]);

	}
	
   	return 0; 
}

int lsh_loop(){

	char *line;
	char **args;
	int status;

	do {
    	printf(">> ");
    	line = lsh_read_line();
    	args = lsh_split_line(line);
    	status = lsh_execute(args);

    	free(line);
    	free(args);
	}while(!status);

}


char *lsh_read_line(void){
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char **lsh_split_line(char *line){
	
	int bufsize = LSH_TOK_BUFSIZE;
	int position = 0;

	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	token = strtok(line, LSH_TOK_DELIM);
	while(token != NULL){

		tokens[position] = token;
		position++;
		
		if(position >= bufsize){
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if(!tokens){
			
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);

	}
	tokens[position] = NULL;
	return tokens;

}


