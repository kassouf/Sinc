#This is the make file for the fuzzy system
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = header



$(BIN_DIR)/fuzzy: $(BIN_DIR) $(BIN_DIR)/util.o $(BIN_DIR)/learn.o
	gcc $(SRC_DIR)/main.c $(BIN_DIR)/util.o $(BIN_DIR)/learn.o -o $(BIN_DIR)/fuzzy -lm -I$(INCLUDE_DIR)


$(BIN_DIR)/util.o: $(BIN_DIR) 
	gcc -c $(SRC_DIR)/util.c -o $(BIN_DIR)/util.o -I$(INCLUDE_DIR) 

clean:
	rm  ./bin/*

$(BIN_DIR)/learn.o: $(BIN_DIR)
	gcc -c $(SRC_DIR)/learn.c -o $(BIN_DIR)/learn.o -I$(INCLUDE_DIR)
$(BIN_DIR):
	mkdir $(BIN_DIR)

remake:
	make clean
	make $(BIN_DIR)/fuzzy