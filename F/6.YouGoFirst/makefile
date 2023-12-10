CC = mpicc
CFLAGS += -O3
SRC_DIR=src
BIN_DIR=bin

default: all

all: create_directory \
	 $(BIN_DIR)/columns

create_directory:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/columns: $(SRC_DIR)/columns.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(BIN_DIR)/*;