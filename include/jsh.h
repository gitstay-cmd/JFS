#ifndef jsh_h_INCLUDED
#define jsh_h_INCLUDED

struct command{
    char *name;
    int (*command)(char **args);
};

int lsh_loop();

char **lsh_split_line(char *line);

char *lsh_read_line(void);

void list_commands();

void register_command(const char *name, int (*command)(char **args));
#endif // jsh_h_INCLUDED

