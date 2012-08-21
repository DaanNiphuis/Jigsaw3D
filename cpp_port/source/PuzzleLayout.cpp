#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "PuzzleLayout.h"
using namespace std;

PuzzleLayout::PuzzleLayout(Puzzle * puzzle) {
	this->_puzzle = puzzle;
	for(uint i = 0; i < LOCATION_COUNT; i++) {
		this->_placed_pieces[i].used = false;
	}
}

void PuzzleLayout::check_location_validity(LOCATION location) const {
	if(0 > location || location >= LOCATION_COUNT) {
		string message = "Unknown location ";
		message += location;
		raise_Exception(message);
	}
}

void PuzzleLayout::check_orientation_validity(uint orientation) const {
	if(0 > orientation || orientation >= 4) {
		string message = "Invalid orientation ";
		message += orientation;
		raise_Exception(message);
	}
}

void PuzzleLayout::place_piece(uint piece_index, uint orientation, bool flipped, LOCATION location) {
	this->check_location_validity(location);
	this->check_orientation_validity(orientation);
	
	if(this->_placed_pieces[location].used) {
		string message = "Location ";
		message += location;
		message += " is already uccupied!";
		raise_Exception(message);
	}
	
	//For each location...
	for(uint l = 0; l < LOCATION_COUNT; l++) {
		if(l == location && this->_placed_pieces[l].used && this->_placed_pieces[l].piece_index == piece_index) {
			string message = "Piece ";
			message += piece_index;
			message += " cannot be placed twice in the same layout!";
			raise_Exception(message);
		}
	}
	
	Placement * placement = &(this->_placed_pieces[location]);
	placement->used = true;
	placement->orientation = orientation;
	placement->flipped = flipped;
}

void PuzzleLayout::remove_piece(LOCATION location) {
	this->check_location_validity(location);
	this->_placed_pieces[location].used = false;
}

string PuzzleLayout::get_pieces_combined_str(int row_placements[]) const {
	uint gridwidth = this->_puzzle->get_gridwidth();
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
			placements[i] = &(this->_placed_pieces[row_placements[0]]);
			pieces[i] = this->_puzzle->get_piece(placements[i]->piece_index);
		}
	}

	string output = "";
	for(uint row_nr = 0; row_nr < gridwidth; row_nr++) {
		for(int index = 0; index < 3; index++) {
			const PuzzlePiece * piece = pieces[index];
			if(piece == NULL) {
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
bool PuzzleLayout::is_valid_edge(LOCATION location1, uint orientation_adj1, LOCATION location2, uint orientation_adj2) const {
	this->check_location_validity(location1);
	this->check_location_validity(location2);
	
	if(this->_placed_pieces[location1].used || this->_placed_pieces[location2].used) {
		return true;
	}
	
	uint gridwidth = this->_puzzle->get_gridwidth();
	const Placement * pp1 = &(this->_placed_pieces[location1]);
	const Placement * pp2 = &(this->_placed_pieces[location1]);
	const PuzzlePiece * piece1 = this->_puzzle->get_piece(pp1->piece_index);
	const PuzzlePiece * piece2 = this->_puzzle->get_piece(pp2->piece_index);
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
	if(corner_right < 0 or corner_right > 1) {
		//print("Fail on right corner")
		return false;
	}
	return true;
}

//Use orientation adjustments to compare the lower right corner of piece1 with the
//upper right corner of piece2 with the upper left corner of piece3. 
bool PuzzleLayout::is_valid_corner(LOCATION location1, uint orient_adj1, LOCATION location2, uint orient_adj2,
			LOCATION location3, uint orient_adj3) const {
	this->check_location_validity(location1);
	this->check_location_validity(location2);
	this->check_location_validity(location3);
	
	if(! this->_placed_pieces[location1].used
	|| ! this->_placed_pieces[location2].used
	|| ! this->_placed_pieces[location3].used) {
		return true;
	}
	
	uint gridwidth = this->_puzzle->get_gridwidth();
	const Placement * pp1 = &(this->_placed_pieces[location1]);
	const Placement * pp2 = &(this->_placed_pieces[location2]);
	const Placement * pp3 = &(this->_placed_pieces[location3]);
	const PuzzlePiece * piece1 = this->_puzzle->get_piece(pp1->piece_index);
	const PuzzlePiece * piece2 = this->_puzzle->get_piece(pp2->piece_index);
	const PuzzlePiece * piece3 = this->_puzzle->get_piece(pp3->piece_index);
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
	if(! this->is_valid_edge(Location_Front, 2, Location_Top, 2))
		return false;
	if(! this->is_valid_edge(Location_Front, 1, Location_Right, 1))
		return false;
	if(! this->is_valid_edge(Location_Front, 0, Location_Bottom, 0))
		return false;
	if(! this->is_valid_edge(Location_Front, 3, Location_Left, 3))
		return false;
	//Edges not adjacent to 'Front' or 'Back
	if(! this->is_valid_edge(Location_Top, 1, Location_Right, 0))
		return false;
	if(! this->is_valid_edge(Location_Right, 0, Location_Bottom, 3))
		return false;
	if(! this->is_valid_edge(Location_Bottom, 3, Location_Left, 2))
		return false;
	if(! this->is_valid_edge(Location_Left, 2, Location_Top, 1))
		return false;
	//Edges adjacent to 'Back'
	if(! this->is_valid_edge(Location_Back, 0, Location_Top, 0))
		return false;
	if(! this->is_valid_edge(Location_Back, 1, Location_Right, 3))
		return false;
	if(! this->is_valid_edge(Location_Back, 2, Location_Bottom, 2))
		return false;
	if(! this->is_valid_edge(Location_Back, 3, Location_Left, 1))
		return false;
	
	//Check corners
	if(! this->is_valid_corner(Location_Top, 0, Location_Front, 0, Location_Right, 0))
		return false;
	if(! this->is_valid_corner(Location_Front, 0, Location_Bottom, 0, Location_Right, 1))
		return false;
	if(! this->is_valid_corner(Location_Left, 0, Location_Bottom, 1, Location_Front, 1))
		return false;
	if(! this->is_valid_corner(Location_Top, 3, Location_Left, 0, Location_Front, 0))
		return false;
	
	if(! this->is_valid_corner(Location_Bottom, 0, Location_Back, 0, Location_Right, 2))
		return false;
	if(! this->is_valid_corner(Location_Back, 0, Location_Top, 0, Location_Right, 3))
		return false;
	if(! this->is_valid_corner(Location_Left, 2, Location_Top, 1, Location_Back, 1))
		return false;
	if(! this->is_valid_corner(Location_Bottom, 3, Location_Left, 2, Location_Back, 0))
		return false;
	return true;
}


bool PuzzleLayout::is_solution() const {
	if(! this->is_valid()) {
		return false;
	}
	return (this->_placed_pieces[Location_Front].used && this->_placed_pieces[Location_Back].used
		&& this->_placed_pieces[Location_Left].used && this->_placed_pieces[Location_Right].used
		&& this->_placed_pieces[Location_Top].used && this->_placed_pieces[Location_Bottom].used);
}

ostream &operator<<(ostream &out, const PuzzleLayout &P) {
	int row_placements[3];
	out << endl;
	if (! P._placed_pieces[Location_Top].used) {
		row_placements[0] = -1;
		row_placements[1] = Location_Top;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	out << endl;

	row_placements[0] = Location_Left;
	row_placements[1] = Location_Front;
	row_placements[2] = Location_Right;
	out << P.get_pieces_combined_str(row_placements);

	if(P._placed_pieces[Location_Bottom].used || P._placed_pieces[Location_Back].used) {
		out << endl;
		row_placements[0] = -1;
		row_placements[1] = Location_Bottom;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	if(P._placed_pieces[Location_Back].used) {
		out << endl;
		row_placements[0] = -1;
		row_placements[1] = Location_Back;
		row_placements[2] = -1;
		out << P.get_pieces_combined_str(row_placements);
	}
	return out;
}
