CC=clang
CFLAGS = -std=c99 -I $(HEADERS_DIR)
SOURCES=source
HEADERS=headers
BIN=bin
TARGET_EXEC=main.elf

ROOT=$(PWD)
HEADERS_DIR=$(ROOT)/$(HEADERS)/
BIN_DIR=$(ROOT)/$(BIN)/

export CC CFLAGS HEADERS_DIR BIN_DIR

$(TARGET_EXEC):
	make -C $(SOURCES) -f Makefile
	$(CC) $(BIN)/* $(CFLAGS) -o $@

clean:
	rm -f $(BIN_DIR)*.o $(TARGET_EXEC)

rebuild: clean $(TARGET_EXEC)

mem_check:
	valgrind --tool=memcheck --leak-check=full --log-file=VL_LOG ./$(TARGET_EXEC)

archive: $(TARGET_EXEC)
	rm $(BIN_DIR)main.o
	ar -rcs libadvanced_types.a $(BIN_DIR)*.o
