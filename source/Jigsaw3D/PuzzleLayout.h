#ifndef PUZZLELAYOUT_H_
#define PUZZLELAYOUT_H_

#include <string>

#include "common.h"
#include "exception_hack.h"

class PuzzlePiece;
class Puzzle;

struct Placement {
	bool used;
	uint piece_index;
	uint orientation;
	bool flipped;
};

class PuzzleLayout {
	const Puzzle & _puzzle;
	Placement _placed_pieces[Location::COUNT];

	void check_location_validity(Location_t location) const;
	void check_orientation_validity(uint orientation) const;

	std::string get_pieces_combined_str(const int row_placements[]) const;	//TODO Change to reference

	bool is_valid_edge(Location_t location1, uint orientation_adj1, Location_t location2, uint orientation_adj2) const;
	bool is_valid_corner(Location_t location1, uint orient_adj1, Location_t location2, uint orient_adj2, Location_t location3, uint orient_adj3) const;

public:
	PuzzleLayout(const Puzzle & puzzle);
	virtual ~PuzzleLayout(void);

	void place_piece(uint piece_index, uint orientation, bool flipped, Location_t location);
	void remove_piece(Location_t location);

	bool is_solution(void) const;
	bool is_valid(void) const;

friend std::ostream &operator<<(std::ostream &out, const PuzzleLayout &P);
};

#endif /* PUZZLELAYOUT_H_ */
