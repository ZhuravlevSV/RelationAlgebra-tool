TARGET = zhurasem

CC = g++ -std=c++17 -Wall -pedantic -Wno-long-long -O2 -Werror

PREF_SRC = ./src/
PREF_OBJ = ./src/obj/
PREF_DOC = ./doc/

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	@if [ ! -d "$(PREF_OBJ)" ]; then mkdir -p $(PREF_OBJ); fi
	$(CC) -c $< -o $@

all : $(TARGET)

compile : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

run : $(TARGET)
	./$(TARGET)

doc : Doxyfile
	doxygen Doxyfile
	mv ./doc/html/* $(PREF_DOC)
	rm -r ./doc/html

clean :
	rm $(TARGET) $(PREF_OBJ)*.o