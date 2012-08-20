#include "PuzzlePiece.h"
#include "Puzzle.h"
using namespace std;

Puzzle::Puzzle(uint gridwidth, vector<vector<uint> > piece_shapes) {
	this->_gridwidth = gridwidth;
	if(piece_shapes.size() != 6) {
		raise_Exception("You can only create puzzles with 6 pieces");
	}
	
	this->_pieces = vector<PuzzlePiece *>();
	for(uint i = 0; i < piece_shapes.size(); i++) {
		vector<uint> shape = piece_shapes.at(i);
		this->_pieces.push_back(new PuzzlePiece(this->_gridwidth, shape));
	}
	
	//Validation
	uint expected_edge_bits = ((gridwidth - 2) * 12); 
	uint actual_edge_bits = 0;
	for(uint i = 0; i < this->_pieces.size(); i++) {
		PuzzlePiece * piece = this->_pieces.at(i);
		actual_edge_bits += piece->get_nr_of_edge_bits();
	}
	if(expected_edge_bits != actual_edge_bits) {
		string message = "Expected edge bits ";
		message += expected_edge_bits;
		message += " did not match actual edge bits ";
		message += actual_edge_bits;
		message += ". Puzzle is unsolvable!";
		raise_Exception(message);
	}
	
	uint expected_corner_bits = 8;
	uint actual_corner_bits = 0;
	for(uint i = 0; i < this->_pieces.size(); i++) {
		PuzzlePiece * piece = this->_pieces.at(i);
		actual_corner_bits += piece->get_nr_of_corner_bits();
	}
	if(expected_corner_bits != actual_corner_bits) {
		string message = "Expected corner bits ";
		message += expected_corner_bits;
		message += " did not match actual corner bits ";
		message += actual_corner_bits;
		message += ". Puzzle is unsolvable!";
		raise_Exception(message);
	}
}

Puzzle::~Puzzle() {
	for(uint i = 0; i < this->_pieces.size(); i++) {
		delete this->_pieces[i];
	}
	this->_pieces.clear();
}


uint Puzzle::get_gridwidth() {
	return this->_gridwidth;
}

PuzzlePiece * Puzzle::get_piece(uint piece_number) {
	return this->_pieces.at(piece_number);
}

string Puzzle::to_string() const {
	string description = "Puzzle - width: ";
	description += this->_gridwidth;
	description += "\nPieces:\n\n";
	for(uint i = 0; i < this->_pieces.size(); i++) {
		PuzzlePiece * piece = this->_pieces.at(i);
		description += piece->to_string();
		description += '\n';
	}
	return description;
}

ostream &operator<<(ostream &out, const Puzzle &P) {
	out<< P.to_string();
	return out;
}
