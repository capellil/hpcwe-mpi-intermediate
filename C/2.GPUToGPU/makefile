CC = mpicc
CFLAGS += -O3 -lcudart
SRC_DIR=src
BIN_DIR=bin

default: all

all: create_directory \
	 $(BIN_DIR)/gputogpu

create_directory:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/gputogpu: $(BIN_DIR)/gputogpu.o $(BIN_DIR)/increment.o
	$(CC) -o $@ $^ $(CFLAGS)

$(BIN_DIR)/gputogpu.o: $(SRC_DIR)/gputogpu.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_DIR)/increment.o: $(SRC_DIR)/increment.cu
	nvcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(BIN_DIR)/*;