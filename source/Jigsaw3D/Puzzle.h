#ifndef PUZZLE_H_
#define PUZZLE_H_

#include <string>
#include <vector>

#include "common.h"

class PuzzlePiece;

class Puzzle {
	const uint _gridwidth;
	std::vector<PuzzlePiece *> _pieces;

	Puzzle& operator=(const Puzzle&) {return *this;}
public:
	Puzzle(uint gridwidth, const std::vector<std::vector<uint> > & piece_shapes);
	virtual ~Puzzle(void);

	uint get_gridwidth(void) const;
	PuzzlePiece * get_piece(uint piece_number) const;

friend std::ostream &operator<<(std::ostream &out, const Puzzle &P);
};

#endif /* PUZZLE_H_ */
