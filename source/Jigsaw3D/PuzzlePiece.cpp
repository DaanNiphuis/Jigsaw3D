#include <string>
#include <vector>

#include "PuzzlePiece.h"
#include "Debug.h"
using namespace std;

PuzzlePiece::PuzzlePiece(uint gridwidth, const vector<uint> & piece_shape) : _gridwidth(gridwidth) {
	ASSERT(_gridwidth >= 3, "Gridwidth "<<_gridwidth<<" is below minimum of 3");

	uint expected_size = (_gridwidth * _gridwidth);
	ASSERT(piece_shape.size() == expected_size, "Shape does not match gridwidth "<<_gridwidth);

	for(uint i = 0; i < piece_shape.size(); i++) {
		uint point = piece_shape.at(i);
		ASSERT(point == 0 || point == 1, "Only '0' or '1' allowed for shape specification.");
	}
	//this->_piece_shape = piece_shape;	//TODO comment on this
	this->_piece_shape.insert(this->_piece_shape.end(), piece_shape.begin(), piece_shape.end());


	//Check for 'floating' corners
	//Use orientation for easy checking
	for(uint orientation = 0; orientation < 4; orientation++) {
		ASSERT(this->get_point(orientation, false, 0, 0) != 1
				|| this->get_point(orientation, false, 1, 0) != 0
				|| this->get_point(orientation, false, 0, 1) != 0,
				"Floating corner found in corner "<<orientation);
	}

	//Check for holes in the center
	for(uint row = 1; row < (_gridwidth - 1); row++) {
		for(uint column = 1; column < (_gridwidth -1); column++) {
			ASSERT(this->get_point(0, false, row, column) == 1, "Center should be completely filled");
		}
	}
}

PuzzlePiece::~PuzzlePiece() {
	//Empty
}


//Used in validation
uint PuzzlePiece::get_nr_of_edge_bits() const {
	uint edge_bits = 0;
	for(uint index = 1; index < (this->_gridwidth - 1); index++) {
		if(this->get_point(0, false, 0, index) == 1)
			edge_bits += 1;
		if(this->get_point(0, false, this->_gridwidth - 1, index) == 1)
			edge_bits += 1;
		if(this->get_point(0, false, index, 0) == 1)
			edge_bits += 1;
		if(this->get_point(0, false, index, this->_gridwidth - 1) == 1)
			edge_bits += 1;
	}
	return edge_bits;
}

uint PuzzlePiece::get_nr_of_corner_bits() const {
	uint corner_bits;
	corner_bits = this->get_point(0, false, 0, 0);
	corner_bits += this->get_point(0, false, 0, this->_gridwidth - 1);
	corner_bits += this->get_point(0, false, this->_gridwidth - 1, 0);
	corner_bits += this->get_point(0, false, this->_gridwidth - 1, this->_gridwidth - 1);
	return corner_bits;
}

uint PuzzlePiece::get_point_index(uint row_number, uint col_number) const {
	return (row_number * this->_gridwidth) + col_number;
}

uint PuzzlePiece::get_flippable_point(bool flipped, uint row_number, uint col_number) const {
	uint index;
	if(flipped)
		index = this->get_point_index(row_number, this->_gridwidth - col_number - 1);
	else
		index = this->get_point_index(row_number, col_number);
	return this->_piece_shape[index];
}

uint PuzzlePiece::get_point(uint orientation, bool flipped, uint row_number, uint col_number) const {
	ASSERT(row_number < this->_gridwidth, "Invalid row_number "<<row_number);
	ASSERT(col_number < this->_gridwidth, "Invalid col_number "<<col_number);
	ASSERT(0 <= orientation || orientation < 4, "Invalid orientation "<<orientation);

	switch(orientation) {
	case 0:
		return this->get_flippable_point(flipped, row_number, col_number);
	case 1:
		return this->get_flippable_point(flipped, this->_gridwidth - col_number - 1, row_number);
	case 2:
		return this->get_flippable_point(flipped, this->_gridwidth - row_number - 1, this->_gridwidth - col_number - 1);
	case 3:
		return this->get_flippable_point(flipped, col_number, this->_gridwidth - row_number - 1);
	}
	ASSERT(false, "Invalid orientation '"<<orientation<<"'")
	throw 1;
}

char PuzzlePiece::get_point_char(uint orientation, bool flipped, uint row_number, uint col_number) const {
	if(this->get_point(orientation, flipped, row_number, col_number) == 1) {
		return 'X';
	}
	return ' ';
}

string PuzzlePiece::get_row_str(uint orientation, bool flipped, uint row_number) const {
	string output = "";
	for(uint col_number = 0; col_number < this->_gridwidth; col_number++) {
		output += this->get_point_char(orientation, flipped, row_number, col_number);
	}
	return output;
}

string PuzzlePiece::get_field_str(uint orientation, bool flipped) const {
	std::ostringstream output;
	for(uint row_number = 0; row_number < this->_gridwidth; row_number++) {
		output << this->get_row_str(orientation, flipped, row_number) << endl;
	}
	return output.str();
}


ostream &operator<<(ostream &out, const PuzzlePiece &P) {
	return out<< P.get_field_str(0, false);
}
