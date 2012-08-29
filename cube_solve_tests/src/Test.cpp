#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include <vector>

#include "PuzzlePiece.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
using namespace std;


vector<vector<uint> > arrayToPuzzleVector(const uint arrPuzzle[6][25]) {
	vector<vector<uint> > puzzleVector;
	for(int p = 0; p < 6; p++) {
		vector<uint> pieceVector;
		puzzleVector.push_back(pieceVector);
		puzzleVector.at(p).assign(arrPuzzle[p], arrPuzzle[p]+25);
	}
	return puzzleVector;
}

void testPuzzlePieceValidation() {
	uint arr_correct_field[] = {
		0,1,0,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		1,1,1,1,1,
		0,0,1,0,0
	};
	vector<uint> correct_field;
	correct_field.assign(arr_correct_field, arr_correct_field+25);

	//There is a hole in the middle
	uint arr_wrong_field_1[] = {
		0,1,0,1,1,
		1,0,1,1,1,
		0,1,1,1,0,
		1,1,1,1,1,
		0,0,1,0,0
	};
	vector<uint> wrong_field_1;
	wrong_field_1.assign(arr_wrong_field_1, arr_wrong_field_1+25);

	//There is a hole in the middle
	uint arr_wrong_field_2[] = {
		0,1,0,1,1,
		1,1,1,1,1,
		0,1,0,1,0,
		1,1,1,1,1,
		0,0,1,0,0
	};
	vector<uint> wrong_field_2;
	wrong_field_2.assign(arr_wrong_field_2, arr_wrong_field_2+25);

	//Floating corner
	uint arr_wrong_field_3[] = {
		0,1,0,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		0,1,1,1,1,
		1,0,1,0,0
	};
	vector<uint> wrong_field_3;
	wrong_field_3.assign(arr_wrong_field_3, arr_wrong_field_3+25);

	//Invalid field value '2'
	uint arr_wrong_field_4[] = {
		0,1,0,1,1,
		1,1,2,1,1,
		0,1,1,1,0,
		0,1,1,1,1,
		1,0,1,0,0
	};
	vector<uint> wrong_field_4;
	wrong_field_4.assign(arr_wrong_field_4, arr_wrong_field_4+25);

	//Invalid field value '-1'
	//Obsolete due to use of 'uint'.
	//uint arr_wrong_field_5[] = {
	//	0,1,0,1,1,
	//	1,1,-1,1,1,
	//	0,1,1,1,0,
	//	0,1,1,1,1,
	//	1,0,1,0,0
	//};
	//vector<uint> wrong_field_5;
	//wrong_field_5.assign(arr_wrong_field_5, arr_wrong_field_5+25);


	//This should not raise an Exception

	PuzzlePiece ptc1(5, correct_field);


	//Wrong gridwidth
	ASSERT_THROWSM("Expected exception", PuzzlePiece ptc2(6, correct_field), int);

	ASSERT_THROWSM("Expected exception", PuzzlePiece ptw1(5, wrong_field_1), int);

	ASSERT_THROWSM("Expected exception", PuzzlePiece ptw2(5, wrong_field_2), int);

	ASSERT_THROWSM("Expected exception", PuzzlePiece ptw3(5, wrong_field_3), int);

	ASSERT_THROWSM("Expected exception", PuzzlePiece ptw4(5, wrong_field_4), int);
}

void testPuzzle() {
	uint arr_puzzle[6][25] = {
			{
				0,1,0,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,1,
				0,0,1,0,0
			},{
				1,0,1,0,0,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				0,1,0,1,0
			},{
				1,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,0,1,1
			},{
				0,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,0,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,0,
				1,1,1,1,1,
				1,1,1,1,0,
				0,0,1,0,0
			}
	};
	vector<vector<uint> > puzzleVector = arrayToPuzzleVector(arr_puzzle);

	Puzzle puzzle(5, puzzleVector);
}

void testPuzzleValidation() {
	uint arr_invalid_corners[6][25] = {
			{
				0,1,0,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,1,
				0,0,1,0,0
			},{
				1,0,1,0,0,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				0,1,0,1,0
			},{
				1,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,0,1,1
			},{
				0,1,0,1,0,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,0,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,0,
				1,1,1,1,1,
				1,1,1,1,0,
				0,0,1,0,0
			}
		};
	vector<vector<uint> > invalidCornersVector = arrayToPuzzleVector(arr_invalid_corners);
	ASSERT_THROWSM("Expected exception", Puzzle pvtc1(5, invalidCornersVector), int);


	//Unsolvable not enough corners
	uint arr_not_enough_corners[6][25] = {
			{
				0,1,0,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,1,
				0,0,1,0,0
			},{
				1,0,1,0,0,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				0,1,0,1,0
			},{
				1,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,0,1,1
			},{
				0,1,0,1,0,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,0,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,0,
				1,1,1,1,1,
				1,1,1,1,0,
				0,0,1,0,0
			}
		};
	vector<vector<uint> > notEnoughCornersVector = arrayToPuzzleVector(arr_not_enough_corners);
	ASSERT_THROWSM("Expected exception", Puzzle pvtc2(5, notEnoughCornersVector), int);

	//Unsolvable to much edges
	uint arr_unsolvable_to_much_edges[6][25] = {
			{
				0,1,0,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,1,
				0,0,1,0,0
			},{
				1,0,1,0,0,
				1,1,1,1,1,
				0,1,1,1,1,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				0,1,0,1,0
			},{
				1,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,0,1,1
			},{
				0,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,0,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,0,
				1,1,1,1,1,
				1,1,1,1,0,
				0,0,1,0,0
			}
		};
	vector<vector<uint> > unsolvable_to_much_edges_vector = arrayToPuzzleVector(arr_unsolvable_to_much_edges);
	ASSERT_THROWSM("Expected exception", Puzzle pvtc3(5, unsolvable_to_much_edges_vector), int);
}

void testPuzzleLayout() {
	uint arr_puzzle[6][25] = {
		{
			0,1,0,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			1,1,1,1,1,
			0,0,1,0,0
		},{
			1,0,1,0,0,
			1,1,1,1,1,
			0,1,1,1,0,
			1,1,1,1,0,
			1,0,1,0,0
		},{
			0,0,1,0,0,
			0,1,1,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			0,1,0,1,0
		},{
			1,1,0,1,1,
			1,1,1,1,0,
			0,1,1,1,1,
			1,1,1,1,0,
			0,1,0,1,1
		},{
			0,1,0,1,1,
			1,1,1,1,0,
			0,1,1,1,1,
			1,1,1,1,0,
			0,0,0,1,1
		},{
			0,0,1,0,0,
			0,1,1,1,0,
			1,1,1,1,1,
			1,1,1,1,0,
			0,0,1,0,0
		}
	};
	vector<vector<uint> > puzzle_vector = arrayToPuzzleVector(arr_puzzle);
	Puzzle puzzle(5, puzzle_vector);
	PuzzleLayout puzzleLayout(&puzzle);
	//Can't place the same piece twice in different places
	puzzleLayout.place_piece(0, 0, false, Location::Front);

	ASSERT_THROWSM("Expected exception", puzzleLayout.place_piece(0, 0, false, Location::Back), int);
}

void testPuzzleLayout_is_valid() {
	uint arr_puzzle[6][25] {
			{
				0,1,0,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,1,
				0,0,1,0,0
			},{
				1,0,1,0,0,
				1,1,1,1,1,
				0,1,1,1,0,
				1,1,1,1,0,
				1,0,1,0,0
			},{
				0,0,1,0,0,
				0,1,1,1,1,
				1,1,1,1,1,
				0,1,1,1,0,
				0,1,0,1,0
			},{
				1,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,1,0,1,1
			},{
				0,1,0,1,1,
				1,1,1,1,0,
				0,1,1,1,1,
				1,1,1,1,0,
				0,0,0,1,1
			},{
				0,0,1,0,0,
				0,1,1,1,0,
				1,1,1,1,1,
				1,1,1,1,0,
				0,0,1,0,0
			}
		};
	vector<vector<uint> > puzzleVector = arrayToPuzzleVector(arr_puzzle);
	Puzzle puzzle(5, puzzleVector);

	PuzzleLayout layout(&puzzle);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(4, 1, false, Location::Top);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(2, 3, false, Location::Left);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(0, 1, false, Location::Front);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(5, 3, false, Location::Right);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(1, 0, false, Location::Bottom);
	ASSERT(layout.is_valid());
	ASSERT(!layout.is_solution());
	layout.place_piece(3, 1, false, Location::Back);
	ASSERT(layout.is_valid());
	ASSERT(layout.is_solution());
}

void runSuite(){
	cute::suite s;
	s.push_back(CUTE(testPuzzlePieceValidation));
	s.push_back(CUTE(testPuzzle));
	s.push_back(CUTE(testPuzzleValidation));
	s.push_back(CUTE(testPuzzleLayout));
	s.push_back(CUTE(testPuzzleLayout_is_valid));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "The Suite");
}

int main(){
	runSuite();
	return 0;
}



