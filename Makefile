BIN_DIR = ./bin
SRC_DIR = ./src
INCLUDE_DIR = ./header



$(BIN_DIR)/fuzzy: $(BIN_DIR)/util.o
	gcc $(SRC_DIR)/main.c $(BIN_DIR)/util.o -o $(BIN_DIR)/fuzzy -lm -I$(INCLUDE_DIR)


$(BIN_DIR)/util.o: 
	gcc -c $(SRC_DIR)/util.c -o $(BIN_DIR)/util.o -I$(INCLUDE_DIR) 

clean:
	rm  ./bin/*
