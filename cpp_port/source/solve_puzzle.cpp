#include <iostream>
#include "PuzzlePiece.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include "PuzzleSolver.h"
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
		pieces.at(i).assign(shapes[i], shapes[i]+25);
	}
}

int main(void) {
	vector<vector<uint> > pieces;
	fill_puzzle_vector(pieces);

	Puzzle puzzle(5, pieces);
	cout << "Solving puzzle:" << endl;
	cout << puzzle << endl;
	PuzzleLayout * solution =  PuzzleSolver::solve_puzzle(puzzle);

	if(solution == NULL) {
		cout<<"No solution found";
	}
	else {
		cout << "Puzzle solution:" << endl;
		cout << (*solution) << endl;
	}
	return 0;
}
