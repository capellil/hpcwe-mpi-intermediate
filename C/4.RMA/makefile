CC = mpicc
CFLAGS += -O3
SRC_DIR=src
BIN_DIR=bin

default: all

all: create_directory \
	 $(BIN_DIR)/rma

create_directory:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/rma: $(SRC_DIR)/rma.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(BIN_DIR)/*;