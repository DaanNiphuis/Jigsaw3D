#ifndef PUZZLEPIECE_H_
#define PUZZLEPIECE_H_

#include <iostream>
#include <vector>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece {

	const uint _gridwidth;
	std::vector<uint> _piece_shape;

	uint get_point_index(uint row_number, uint col_number) const;
	uint get_flippable_point(bool flipped, uint row_number, uint col_number) const;
	char get_point_char(uint orientation, bool flipped, uint row_number, uint col_number) const;
	std::string get_field_str(uint orientation, bool flipped) const;

	PuzzlePiece& operator=(const PuzzlePiece&) {return *this;}

 public:
	PuzzlePiece(uint gridwidth, const std::vector<uint> & piece_shape);
	virtual ~PuzzlePiece(void);

	uint get_nr_of_edge_bits(void) const;
	uint get_nr_of_corner_bits(void) const;
	uint get_point(uint orientation, bool flipped, uint row_number, uint col_number) const;
	std::string get_row_str(uint orientation, bool flipped, uint row_number) const;

friend std::ostream &operator<<(std::ostream &out, const PuzzlePiece &P);
};

#endif /* PUZZLEPIECE_H_ */
