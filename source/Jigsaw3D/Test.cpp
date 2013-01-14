#include "Test.h"

#include "Debug.h"
#include "PuzzlePiece.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include "PuzzleSolver.h"

#include <vector>

std::vector<std::vector<unsigned int> > arrayToPuzzleVector(const unsigned int arrPuzzle[6][25]) {
	std::vector<std::vector<unsigned int> > puzzleVector;
	for(int p = 0; p < 6; p++) {
		std::vector<unsigned int> pieceVector;
		puzzleVector.push_back(pieceVector);
		puzzleVector.at(p).assign(arrPuzzle[p], arrPuzzle[p]+25);
	}
	return puzzleVector;
}

const PuzzleLayout* Test::runDifficultTest()
{
	unsigned int arr_pieces[6][25] = {
			{
				0,1,0,1,1,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,1,1,0,
				1,1,0,1,0
			},{
				0,1,1,0,1,
				0,1,1,1,1,
				1,1,1,1,1,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,1,0,1,0,
				0,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				1,1,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				0,0,0,1,0
			},{
				0,1,0,1,0,
				0,1,1,1,0,
				1,1,1,1,1,
				0,1,1,1,1,
				1,1,0,0,0
			},{
				0,0,1,0,0,
				1,1,1,1,0,
				0,1,1,1,1,
				0,1,1,1,1,
				0,1,0,1,1
			}
	};
	std::vector<std::vector<unsigned int> > pieces = arrayToPuzzleVector(arr_pieces);

	Puzzle puzzle(5, pieces);
	std::cout << "Solving (difficult) puzzle:" << std::endl;
	std::cout << puzzle << std::endl;

	PuzzleLayout * solution =  PuzzleSolver::solve_puzzle_with_flipping(puzzle);
	ASSERT(solution != NULL, "No solution found.");
	std::cout << "Puzzle solution:" << std::endl;
	std::cout << (*solution) << std::endl;

	return solution;
}

Test::Test()
{
}


Test::~Test()
{
}
