CXX = g++
CXXFLAGS = -Wall -c -g
#LDFLAGS = -Xlinker -zmuldefs -g
LDFLAGS = -g
#SOURCES = exception_hack.cpp PuzzlePiece.cpp Puzzle.cpp PuzzleLayout.cpp solve_puzzle.cpp
#OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = exception_hack.o PuzzlePiece.o Puzzle.o PuzzleLayout.o solve_puzzle.o
EXECUTABLES_DIR = bin
EXECUTABLE = bin/solve_puzzle
HEADERS = Puzzle.h  PuzzleLayout.h  PuzzlePiece.h  common.h exception_hack.h


all: $(EXECUTABLES_DIR) $(EXECUTABLE)

$(EXECUTABLES_DIR):
	mkdir -p $(EXECUTABLES_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
	rm -fr $(EXECUTABLES_DIR)
