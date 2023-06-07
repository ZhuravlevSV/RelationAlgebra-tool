TARGET = zhurasem

CC = g++ -std=c++17 -Wall -pedantic -Wno-long-long -O2 -Werror

PREF_SRC = ./src/
PREF_OBJ = ./src/obj/
PREF_DOC = ./doc/
PREF_DEP = ./src/dep/
PREF_DOC_SRC = ./doc/html/
PREF_DOC_DEST = $(PREF_DOC)

SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))
DEP = $(patsubst $(PREF_SRC)%.cpp, $(PREF_DEP)%.d, $(SRC))

SRC_RA = $(wildcard $(PREF_SRC)commands/ra/*.cpp)
OBJ_RA = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC_RA))
DEP_RA = $(patsubst $(PREF_SRC)%.cpp, $(PREF_DEP)%.d, $(SRC_RA))

SRC_SQL = $(wildcard $(PREF_SRC)commands/sql/*.cpp)
OBJ_SQL = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC_SQL))
DEP_SQL = $(patsubst $(PREF_SRC)%.cpp, $(PREF_DEP)%.d, $(SRC_SQL))

# $(TARGET) : $(OBJ)
#	$(CC) $(OBJ) -o $(TARGET)

$(TARGET) : $(OBJ) $(OBJ_RA) $(OBJ_SQL)
	$(CC) $(OBJ) $(OBJ_RA) $(OBJ_SQL) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	@if [ ! -d "$(PREF_OBJ)" ]; then mkdir -p $(PREF_OBJ); fi
	$(CC) -c $< -o $@

$(PREF_OBJ)commands/ra/%.o : $(PREF_SRC)commands/ra/%.cpp
	@if [ ! -d "$(PREF_OBJ)commands/ra" ]; then mkdir -p $(PREF_OBJ)commands/ra; fi
	$(CC) -c $< -o $@

$(PREF_OBJ)commands/sql/%.o : $(PREF_SRC)commands/sql/%.cpp
	@if [ ! -d "$(PREF_OBJ)commands/sql" ]; then mkdir -p $(PREF_OBJ)commands/sql; fi
	$(CC) -c $< -o $@

$(PREF_DEP)%.d : $(PREF_SRC)%.cpp
	@if [ ! -d "$(PREF_DEP)" ]; then mkdir -p $(PREF_DEP); fi
	$(CC) -MM -MT '$(PREF_OBJ)$*.o' $< -MF $@


-include $(DEP)

all : $(TARGET)

compile : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

run : $(TARGET)
	./$(TARGET)

doc : Doxyfile
	doxygen Doxyfile
	mv $(PREF_DOC_SRC)* $(PREF_DOC_DEST)
	rm -r $(PREF_DOC_SRC)

clean :
	rm $(TARGET) $(PREF_OBJ)*.o $(PREF_DEP)*.d

#clean :
#	rm $(TARGET) $(PREF_OBJ)*.o $(PREF_OBJ)commands/ra/*.o $(PREF_DEP)*.d $(PREF_DEP)commands/ra/*.d
