SOURCES = $(wildcard *)
OBJECTS = $(SOURCES:.c=.o)
obj: $(OBJECTS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(BIN_DIR:=$@)

