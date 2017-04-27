INCLUDE = include
TARGETS = jsh.c membox.c memblock.c ext.c main.c block.c
all:
	gcc $(TARGETS)  -I$(INCLUDE) -g -o jsh
