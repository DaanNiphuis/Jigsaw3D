#include <sstream>

#include "PuzzlePiece.h"
#include "Puzzle.h"
#include "Debug.h"
using namespace std;

Puzzle::Puzzle(uint gridwidth, const vector<vector<uint> > & piece_shapes) : _gridwidth(gridwidth) {
	ASSERT(piece_shapes.size() == 6, "You can only create puzzles with 6 pieces");

	this->_pieces = vector<PuzzlePiece *>();
	for(uint i = 0; i < piece_shapes.size(); i++) {
		this->_pieces.push_back(new PuzzlePiece(this->_gridwidth, piece_shapes.at(i)));
	}
	//Validation
	uint expected_edge_bits = ((gridwidth - 2) * 12);
	uint actual_edge_bits = 0;
	for(uint i = 0; i < this->_pieces.size(); i++) {
		PuzzlePiece * piece = this->_pieces.at(i);
		actual_edge_bits += piece->get_nr_of_edge_bits();
	}
	ASSERT(expected_edge_bits == actual_edge_bits, "Expected edge bits "<<expected_edge_bits<<
			" did not match actual edge bits "<<actual_edge_bits<<". Puzzle is unsolvable!");

	uint expected_corner_bits = 8;
	uint actual_corner_bits = 0;
	for(uint i = 0; i < this->_pieces.size(); i++) {
		PuzzlePiece * piece = this->_pieces.at(i);
		actual_corner_bits += piece->get_nr_of_corner_bits();
	}
	ASSERT(expected_corner_bits == actual_corner_bits, "Expected corner bits "<<expected_corner_bits
			<<" did not match actual corner bits "<<actual_corner_bits<<". Puzzle is unsolvable!");
}

Puzzle::Puzzle(const Puzzle& puzzle) :
	_gridwidth(puzzle._gridwidth)
{
	for (std::vector<PuzzlePiece *>::const_iterator it = puzzle._pieces.begin(); it != puzzle._pieces.end(); ++it)
	{
		_pieces.push_back(new PuzzlePiece(*(*it)));
	}
}

Puzzle::~Puzzle() {
	while(!this->_pieces.empty()) {
		delete this->_pieces.back();
		this->_pieces.pop_back();
	}
}


uint Puzzle::get_gridwidth() const {
	return this->_gridwidth;
}

PuzzlePiece * Puzzle::get_piece(uint piece_number) const {
	return this->_pieces.at(piece_number);
}

ostream &operator<<(ostream &out, const Puzzle &P) {
	out << "Puzzle - width: " << P._gridwidth << endl;
	out << "Pieces:" << endl << endl;
	for(uint i = 0; i < P._pieces.size(); i++) {
		PuzzlePiece * piece = P._pieces.at(i);
		out << (*piece) << endl;
	}
	return out;
}
