# Temporary makefile until we get one from Prof. Lewis

FLAGS = -Wall -Wextra -g

CA = 5
GROUP_NAME = let-s-go-mets
EXECUTABLE = ca5
OBJ_DIR = obj
BIN_DIR = bin
#OBJS = 	$(OBJ_DIR)/main.o

ca5: CA5

tar: clean
	cd .. && tar -cvzf CA$(CA)_$(GROUP_NAME).tar.gz cs240-spring21-ca$(CA)-$(GROUP_NAME) && cd -

scrub: clean
	rm ~* *.swp *\#

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing

CA5: $(OBJ_DIR)/main.o
	g++ $(FLAGS) $(OBJ_DIR)/main.o -o $(BIN_DIR)/$(EXECUTABLE)

$(OBJ_DIR)/main.o: main.cpp
	g++ $(FLAGS) -c main.cpp -o $(OBJ_DIR)/main.o

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(EXECUTABLE)