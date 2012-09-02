#ifndef PUZZLELAYOUT_H_
#define PUZZLELAYOUT_H_

#include <string>

#include "common.h"

class PuzzlePiece;
class Puzzle;
class Vector3;

struct Placement {
	bool used;
	uint piece_index;
	uint orientation;
	bool flipped;
};

class PuzzleLayout {
	const Puzzle & _puzzle;
	Placement _placed_pieces[Location::COUNT];

	void check_location_validity(Location::Enum location) const;
	void check_orientation_validity(uint orientation) const;

	std::string get_pieces_combined_str(const int row_placements[]) const;	//TODO Change to reference

	bool is_valid_edge(Location::Enum location1, uint orientation_adj1, Location::Enum location2, uint orientation_adj2) const;
	bool is_valid_corner(Location::Enum location1, uint orient_adj1, Location::Enum location2, uint orient_adj2, Location::Enum location3, uint orient_adj3) const;

	PuzzleLayout& operator=(const PuzzleLayout&) {return *this;}

public:
	PuzzleLayout(const Puzzle & puzzle);
	virtual ~PuzzleLayout(void);

	void place_piece(uint piece_index, uint orientation, bool flipped, Location::Enum location);
	void remove_piece(Location::Enum location);

	bool is_solution(void) const;
	bool is_valid(void) const;

	inline const Puzzle& getPuzzle() const {return _puzzle;}

	bool hasPuzzlePieceAtLocation(Location::Enum pieceLocation) const;
	bool hasPoint(Location::Enum pieceLocation, uint rowNumber, uint columnNumber) const;
	Vector3 getPointPosition(Location::Enum pieceLocation, uint row_number, uint column_number) const;

friend std::ostream &operator<<(std::ostream &out, const PuzzleLayout &P);
};

#endif /* PUZZLELAYOUT_H_ */
