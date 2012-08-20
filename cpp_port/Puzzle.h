#ifndef PUZZLE_H
#define PUZZLE_H

#include "common.h"
#include "exception_hack.h"
#include "PuzzlePiece.h"
using namespace std;

class Puzzle {
	uint _gridwidth;
	vector<PuzzlePiece *> _pieces;
	
public:
	Puzzle(uint gridwidth, vector<vector<uint> > piece_shapes);
	~Puzzle(void);
	
	uint get_gridwidth(void);
	PuzzlePiece * get_piece(uint piece_number);
	string to_string(void);
};

#endif