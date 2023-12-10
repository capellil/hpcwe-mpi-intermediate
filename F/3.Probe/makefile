CC = mpifort
CFLAGS += -O3
SRC_DIR=src
BIN_DIR=bin

default: all

all: create_directory \
	 $(BIN_DIR)/probe

create_directory:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/probe: $(SRC_DIR)/probe.f08
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(BIN_DIR)/*;