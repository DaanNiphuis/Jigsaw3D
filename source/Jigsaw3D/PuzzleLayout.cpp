#include <sstream>

#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "PuzzleLayout.h"
#include "Debug.h"
using namespace std;

PuzzleLayout::PuzzleLayout(const Puzzle & puzzle) : _puzzle(puzzle) {
	for(uint i = 0; i < Location::COUNT; i++) {
		this->_placed_pieces[i].used = false;
	}
}

PuzzleLayout::~PuzzleLayout(void) {
	//Empty
}

void PuzzleLayout::check_location_validity(Location::Enum location) const {
	ASSERT(0 <= location && location < Location::COUNT, "Unknown location "<<location);
}

void PuzzleLayout::check_orientation_validity(uint orientation) const {
	ASSERT(0 <= orientation && orientation < 4, "Invalid orientation "<<orientation);
}

void PuzzleLayout::place_piece(uint piece_index, uint orientation, bool flipped, Location::Enum location) {
	this->check_location_validity(location);
	this->check_orientation_validity(orientation);

	ASSERT(!this->_placed_pieces[location].used, "Location "<<location<<" is already uccupied!");

	//For each location...
	for(uint l = 0; l < Location::COUNT; l++) {
		ASSERT(!this->_placed_pieces[l].used || this->_placed_pieces[l].piece_index != piece_index,
				"Piece "<<piece_index<<" cannot be placed twice in the same layout!");
	}

	Placement * placement = &(this->_placed_pieces[location]);
	placement->used = true;
	placement->piece_index = piece_index;
	placement->orientation = orientation;
	placement->flipped = flipped;
}

void PuzzleLayout::remove_piece(Location::Enum location) {
	this->check_location_validity(location);
	this->_placed_pieces[location].used = false;
}

string PuzzleLayout::get_pieces_combined_str(const int row_placements[]) const {
	uint gridwidth = this->_puzzle.get_gridwidth();
	string filler_space = "";
	filler_space.insert(0, gridwidth, ' ');

	const Placement * placements[3];
	const PuzzlePiece * pieces[3];
	for(int i = 0; i < 3; i++) {
		if(row_placements[i] == -1) {
			placements[i] = NULL;
			pieces[i] = NULL;
		}
		else {
			placements[i] = &(this->_placed_pieces[row_placements[i]]);
			pieces[i] = this->_puzzle.get_piece(placements[i]->piece_index);
		}
	}

	string output = "";
	for(uint row_nr = 0; row_nr < gridwidth; row_nr++) {
		for(int index = 0; index < 3; index++) {
			const PuzzlePiece * piece = pieces[index];
			if(placements[index] == NULL || ! placements[index]->used) {
				output += filler_space;
				output += ' ';
			}
			else {
				output += piece->get_row_str(
					placements[index]->orientation,
					placements[index]->flipped,
					row_nr
				);
				output += ' ';
			}
		}
		output += "\n";
	}
	return output;
}

//Use orientation adjustments to compare the lower edge of piece1 with the upper edge of piece2.
bool PuzzleLayout::is_valid_edge(Location::Enum location1, uint orientation_adj1, Location::Enum location2, uint orientation_adj2) const {
	this->check_location_validity(location1);
	this->check_location_validity(location2);

	if(!this->_placed_pieces[location1].used || !this->_placed_pieces[location2].used) {
		return true;
	}

	uint gridwidth = this->_puzzle.get_gridwidth();
	const Placement * pp1 = &(this->_placed_pieces[location1]);
	const Placement * pp2 = &(this->_placed_pieces[location2]);
	const PuzzlePiece * piece1 = this->_puzzle.get_piece(pp1->piece_index);
	const PuzzlePiece * piece2 = this->_puzzle.get_piece(pp2->piece_index);
	uint orientation1 = (pp1->orientation + orientation_adj1) % 4;
	uint orientation2 = (pp2->orientation + orientation_adj2) % 4;

	//Check edge
	for(uint index = 1; index < (gridwidth - 1); index++) {
		uint edge_value = piece1->get_point(orientation1, pp1->flipped, gridwidth - 1, index);
		edge_value += piece2->get_point(orientation2, pp2->flipped, 0, index);
		if(edge_value != 1) {
			return false;
		}
	}

	//Check corners partial (can't do full corner check here, need 3 pieces for that)
	uint corner_left = piece1->get_point(orientation1, pp1->flipped, gridwidth - 1, 0);
	corner_left += piece2->get_point(orientation2, pp2->flipped, 0, 0);
	if(corner_left < 0 || corner_left > 1) {
		//print("Fail on left corner")
		return false;
	}

	uint corner_right = piece1->get_point(orientation1, pp1->flipped, gridwidth - 1, gridwidth - 1);
	corner_right += piece2->get_point(orientation2, pp2->flipped, 0, gridwidth - 1);
	if(corner_right < 0 || corner_right > 1) {
		//print("Fail on right corner")
		return false;
	}
	return true;
}

//Use orientation adjustments to compare the lower right corner of piece1 with the
//upper right corner of piece2 with the upper left corner of piece3.
bool PuzzleLayout::is_valid_corner(Location::Enum location1, uint orient_adj1, Location::Enum location2, uint orient_adj2,
		Location::Enum location3, uint orient_adj3) const {
	this->check_location_validity(location1);
	this->check_location_validity(location2);
	this->check_location_validity(location3);

	if(! this->_placed_pieces[location1].used
	|| ! this->_placed_pieces[location2].used
	|| ! this->_placed_pieces[location3].used) {
		return true;
	}

	uint gridwidth = this->_puzzle.get_gridwidth();
	const Placement * pp1 = &(this->_placed_pieces[location1]);
	const Placement * pp2 = &(this->_placed_pieces[location2]);
	const Placement * pp3 = &(this->_placed_pieces[location3]);
	const PuzzlePiece * piece1 = this->_puzzle.get_piece(pp1->piece_index);
	const PuzzlePiece * piece2 = this->_puzzle.get_piece(pp2->piece_index);
	const PuzzlePiece * piece3 = this->_puzzle.get_piece(pp3->piece_index);
	uint orientation1 = (pp1->orientation + orient_adj1) % 4;
	uint orientation2 = (pp2->orientation + orient_adj2) % 4;
	uint orientation3 = (pp3->orientation + orient_adj3) % 4;

	uint corner_bit1 = piece1->get_point(orientation1, pp1->flipped, gridwidth - 1, gridwidth - 1);
	uint corner_bit2 = piece2->get_point(orientation2, pp2->flipped, 0, gridwidth - 1);
	uint corner_bit3 = piece3->get_point(orientation3, pp3->flipped, 0, 0);
	return (corner_bit1 + corner_bit2 + corner_bit3) == 1;
}

bool PuzzleLayout::is_valid() const {
	//Check edges
	//Edges adjacent to 'Front'.
	if(! this->is_valid_edge(Location::Front, 2, Location::Top, 2))
		return false;
	if(! this->is_valid_edge(Location::Front, 1, Location::Right, 1))
		return false;
	if(! this->is_valid_edge(Location::Front, 0, Location::Bottom, 0))
		return false;
	if(! this->is_valid_edge(Location::Front, 3, Location::Left, 3))
		return false;
	//Edges not adjacent to 'Front' or 'Back
	if(! this->is_valid_edge(Location::Top, 1, Location::Right, 0))
		return false;
	if(! this->is_valid_edge(Location::Right, 0, Location::Bottom, 3))
		return false;
	if(! this->is_valid_edge(Location::Bottom, 3, Location::Left, 2))
		return false;
	if(! this->is_valid_edge(Location::Left, 2, Location::Top, 1))
		return false;
	//Edges adjacent to 'Back'
	if(! this->is_valid_edge(Location::Back, 0, Location::Top, 0))
		return false;
	if(! this->is_valid_edge(Location::Back, 1, Location::Right, 3))
		return false;
	if(! this->is_valid_edge(Location::Back, 2, Location::Bottom, 2))
		return false;
	if(! this->is_valid_edge(Location::Back, 3, Location::Left, 1))
		return false;

	//Check corners
	if(! this->is_valid_corner(Location::Top, 0, Location::Front, 0, Location::Right, 0))
		return false;
	if(! this->is_valid_corner(Location::Front, 0, Location::Bottom, 0, Location::Right, 1))
		return false;
	if(! this->is_valid_corner(Location::Left, 0, Location::Bottom, 1, Location::Front, 1))
		return false;
	if(! this->is_valid_corner(Location::Top, 3, Location::Left, 0, Location::Front, 0))
		return false;

	if(! this->is_valid_corner(Location::Bottom, 0, Location::Back, 0, Location::Right, 2))
		return false;
	if(! this->is_valid_corner(Location::Back, 0, Location::Top, 0, Location::Right, 3))
		return false;
	if(! this->is_valid_corner(Location::Left, 2, Location::Top, 1, Location::Back, 1))
		return false;
	if(! this->is_valid_corner(Location::Bottom, 3, Location::Left, 2, Location::Back, 0))
		return false;
	return true;
}


bool PuzzleLayout::is_solution() const {
	if(! this->is_valid()) {
		return false;
	}
	return (this->_placed_pieces[Location::Front].used && this->_placed_pieces[Location::Back].used
		&& this->_placed_pieces[Location::Left].used && this->_placed_pieces[Location::Right].used
		&& this->_placed_pieces[Location::Top].used && this->_placed_pieces[Location::Bottom].used);
}

ostream &operator<<(ostream &out, const PuzzleLayout &P) {
	int row_placements[3];
	out << endl;
	if (P._placed_pieces[Location::Top].used) {
		row_placements[0] = -1;
		row_placements[1] = Location::Top;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	out << endl;

	row_placements[0] = Location::Left;
	row_placements[1] = Location::Front;
	row_placements[2] = Location::Right;
	out << P.get_pieces_combined_str(row_placements);

	if(P._placed_pieces[Location::Bottom].used || P._placed_pieces[Location::Back].used) {
		out << endl;
		row_placements[0] = -1;
		row_placements[1] = Location::Bottom;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	if(P._placed_pieces[Location::Back].used) {
		out << endl;
		row_placements[0] = -1;
		row_placements[1] = Location::Back;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	return out;
}
