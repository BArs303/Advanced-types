CC = clang
CFLAGS = -std=c99

HEADERS_DIRS = headers
LIB_DIR = lib
SRC_DIR = source
OBJ_DIR = bin

TARGET_EXEC = main.elf

SOURCES = $(wildcard $(SRC_DIR)/*.c)

#SRC_NAMES = $(notdir $(SOURCES))

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
H_FLAGS = $(foreach DIR, $(HEADERS_DIRS), -I $(DIR))

#VAR = x.y.z
#OBJECTS = $(subst .,?, $(VAR)) will output x?y?z
#$(foreach var, list, text)
#subst doesn't support '%' in the pattern
#SSS1 = $(subst %.c,%,x.c.c bar.c)
#SSS = $(patsubst %.c,%.o, x.c.c bar.c)
#replace every *.h file to *.o
#REP = $(VAR:%.h=%.o)
#@ means don't print the command
#$@ means a target name
#$< means first dependency name
#$^ means a list of all dependency names

$(TARGET_EXEC):$(OBJECTS)
	$(CC) -o $@ $^

test:
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(H_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(H_FLAGS) -c -o $@ $<
