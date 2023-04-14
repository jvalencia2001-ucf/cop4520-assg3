# Makefile for compiling app1.cpp and app2.cpp into prog1 and prog2

# Compiler
CC = g++

# Source files and output files
SRC1 = minotaurs-chain/minotaurs-chain.cpp
SRC2 = mars-rover/mars-rover.cpp
OUT1 = prog1
OUT2 = prog2

# Default target
all: $(OUT1) $(OUT2)

# Target for compiling app1.cpp into prog1
$(OUT1): $(SRC1)
	$(CC) $(SRC1) -o $(OUT1)

# Target for compiling app2.cpp into prog2
$(OUT2): $(SRC2)
	$(CC) $(SRC2) -o $(OUT2)

# Target for cleaning up generated files
clean:
	rm -f $(OUT1) $(OUT2)