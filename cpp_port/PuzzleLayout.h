#ifndef PUZZLELAYOUT_H
#define PUZZLELAYOUT_H

#include "common.h"
#include "exception_hack.h"
#include "Puzzle.h"
#include "PuzzlePiece.h"
using namespace std;

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
	
	void check_location_validity(LOCATION location);
	void check_orientation_validity(uint orientation);
	
	string get_pieces_combined_str(int row_placements[]);		//TODO
	
	bool is_valid_edge(LOCATION location1, uint orientation_adj1, LOCATION location2, uint orientation_adj2);
	bool is_valid_corner(LOCATION location1, uint orient_adj1, LOCATION location2, uint orient_adj2, LOCATION location3, uint orient_adj3);
	
public:
	PuzzleLayout(Puzzle * puzzle);
	~PuzzleLayout(void);
	
	void place_piece(uint piece_index, uint orientation, bool flipped, LOCATION location);
	void remove_piece(LOCATION location);
	
	bool is_solution(void);
	bool is_valid(void);
	
	string to_string(void);
};

#endif