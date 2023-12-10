CC = mpicc
CFLAGS += -O3
SRC_DIR=src
BIN_DIR=bin

default: all

all: create_directory \
	 $(BIN_DIR)/gputogpu

create_directory:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/gputogpu: $(SRC_DIR)/gputogpu.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(BIN_DIR)/*;