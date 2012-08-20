#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <vector>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece {
	uint _gridwidth;
	std::vector<uint> _piece_shape;
	
	uint get_point_index(uint row_number, uint col_number);
	uint get_flippable_point(bool flipped, uint row_number, uint col_number);
	std::string get_field_str(uint orientation, bool flipped);
	
 public:
	PuzzlePiece(uint gridwidth, std::vector<uint> & piece_shape);
	~PuzzlePiece(void);
	
	uint get_nr_of_edge_bits(void);
	uint get_nr_of_corner_bits(void);
	uint get_point(uint orientation, bool flipped, uint row_number, uint col_number);
	char get_point_char(uint orientation, bool flipped, uint row_number, uint col_number);
	std::string get_row_str(uint orientation, bool flipped, uint row_number);
	std::string to_string(void);
};

#endif