#define DEFINE_EXTERNS

#include <iostream>
#include "PuzzlePiece.h"
#include "Puzzle.h"
using namespace std;

int main(void) {
	/*
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
	*/
	
	
	int shape0[] = {
		0,1,0,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		1,1,1,1,1,
		0,0,1,0,0
	};
	int shape1[] = {
		1,0,1,0,0,
		1,1,1,1,1,
		0,1,1,1,0,
		1,1,1,1,0,
		1,0,1,0,0
	};
	int shape2[] = {
		0,0,1,0,0,
		0,1,1,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		0,1,0,1,0
	};
	int shape3[] = {
		1,1,0,1,1,
		1,1,1,1,0,
		0,1,1,1,1,
		1,1,1,1,0,
		0,1,0,1,1
	};
	int shape4[] = {
		0,1,0,1,1,
		1,1,1,1,0,
		0,1,1,1,1,
		1,1,1,1,0,
		0,0,0,1,1
	};
	int shape5[] = {
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1,
		1,1,1,1,0,
		0,0,1,0,0
	};
	vector<vector<uint> > pieces;
	vector<uint> shape_vector0;
	vector<uint> shape_vector1;
	vector<uint> shape_vector2;
	vector<uint> shape_vector3;
	vector<uint> shape_vector4;
	vector<uint> shape_vector5;
	shape_vector0.assign(shape0, shape0+25);
	shape_vector1.assign(shape1, shape1+25);
	shape_vector2.assign(shape2, shape2+25);
	shape_vector3.assign(shape3, shape3+25);
	shape_vector4.assign(shape4, shape4+25);
	shape_vector5.assign(shape5, shape5+25);
	pieces.push_back(shape_vector0);
	pieces.push_back(shape_vector1);
	pieces.push_back(shape_vector2);
	pieces.push_back(shape_vector3);
	pieces.push_back(shape_vector4);
	pieces.push_back(shape_vector5);
	
	Puzzle puzzle(5, pieces);
	cout << puzzle;
	PuzzlePiece * p = puzzle.get_piece(0);
	cout << endl;
	cout << (*p) <<endl;
	cout << p->to_string() <<endl;
	cout << endl;
	return 0;
}
