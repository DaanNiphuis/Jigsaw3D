#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece;

class Puzzle {
	uint _gridwidth;
	std::vector<PuzzlePiece *> _pieces;
	
public:
	Puzzle(uint gridwidth, std::vector<std::vector<uint> > piece_shapes);
	~Puzzle(void);
	
	uint get_gridwidth(void);
	PuzzlePiece * get_piece(uint piece_number);
	std::string to_string(void);
};

#endif