#ifndef PUZZLE_H_
#define PUZZLE_H_

#include <string>
#include <vector>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece;

class Puzzle {
	const uint _gridwidth;
	std::vector<PuzzlePiece *> _pieces;

public:
	Puzzle(uint gridwidth, std::vector<std::vector<uint> > piece_shapes);
	virtual ~Puzzle(void);

	uint get_gridwidth(void) const;
	PuzzlePiece * get_piece(uint piece_number) const;

friend std::ostream &operator<<(std::ostream &out, const Puzzle &P);
};

#endif /* PUZZLE_H_ */
