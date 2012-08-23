#ifndef PUZZLELAYOUT_H
#define PUZZLELAYOUT_H

#include <string>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece;
class Puzzle;

//Used as array index, do not change order.
enum LOCATION {
	Location_Front = 0,
	Location_Back = 1,
	Location_Left = 2,
	Location_Right = 3,
	Location_Top = 4,
	Location_Bottom = 5,

	LOCATION_COUNT = 6
};

struct Placement {
	bool used;
	uint piece_index;
	uint orientation;
	bool flipped;
};

class PuzzleLayout {
	Puzzle * _puzzle;
	Placement _placed_pieces[LOCATION_COUNT];

	void check_location_validity(LOCATION location) const;
	void check_orientation_validity(uint orientation) const;

	std::string get_pieces_combined_str(int row_placements[]) const;	//TODO Change to reference

	bool is_valid_edge(LOCATION location1, uint orientation_adj1, LOCATION location2, uint orientation_adj2) const;
	bool is_valid_corner(LOCATION location1, uint orient_adj1, LOCATION location2, uint orient_adj2, LOCATION location3, uint orient_adj3) const;

public:
	PuzzleLayout(Puzzle * puzzle);
	~PuzzleLayout(void);

	void place_piece(uint piece_index, uint orientation, bool flipped, LOCATION location);
	void remove_piece(LOCATION location);

	bool is_solution(void) const;
	bool is_valid(void) const;

friend std::ostream &operator<<(std::ostream &out, const PuzzleLayout &P);
};

#endif
