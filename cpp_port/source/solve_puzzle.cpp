#include <iostream>
#include "PuzzlePiece.h"
#include "Puzzle.h"
using namespace std;

void fill_puzzle_vector(vector<vector<uint> >& pieces) {

	int shapes[][25] = {
		{
			0,1,0,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			1,1,1,1,1,
			0,0,1,0,0
		},
		{
			1,0,1,0,0,
			1,1,1,1,1,
			0,1,1,1,0,
			1,1,1,1,0,
			1,0,1,0,0
		},
		{
			0,0,1,0,0,
			0,1,1,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			0,1,0,1,0
		},
		{
			1,1,0,1,1,
			1,1,1,1,0,
			0,1,1,1,1,
			1,1,1,1,0,
			0,1,0,1,1
		},
		{
			0,1,0,1,1,
			1,1,1,1,0,
			0,1,1,1,1,
			1,1,1,1,0,
			0,0,0,1,1
		},
		{
			0,0,1,0,0,
			0,1,1,1,0,
			1,1,1,1,1,
			1,1,1,1,0,
			0,0,1,0,0
		}
	};
	for(int i = 0; i < 6; i++) {
		vector<uint> piece;
		pieces.push_back(piece);
		pieces.at(0).assign(shapes[i], shapes[i]+25);
	}
}

int main(void) {
	vector<vector<uint> > pieces;
	fill_puzzle_vector(pieces);

	Puzzle puzzle(5, pieces);
	cout << puzzle;
	PuzzlePiece * p = puzzle.get_piece(0);
	cout << endl;
	cout << (*p) <<endl;
	cout << endl;
	return 0;
}
