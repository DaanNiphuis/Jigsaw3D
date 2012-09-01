/*
 * PuzzleSolver.cpp
 *
 *  Created on: 24 aug. 2012
 *      Author: marcel
 */

#include <cstddef>

#include "PuzzleSolver.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include "Debug.h"
using namespace std;

PuzzleSolver::PuzzleSolver() {
	//Don't instantiate this class.

}

PuzzleSolver::~PuzzleSolver() {
	//Empty.
}


PuzzleLayout * PuzzleSolver::solve_puzzle(const Puzzle& puzzle) {
	//Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	vector<Location::Enum> location_order;
	location_order.push_back(Location::Back);
	location_order.push_back(Location::Right);
	location_order.push_back(Location::Bottom);
	location_order.push_back(Location::Left);
	location_order.push_back(Location::Top);

	//The order here is of less importance for the performance.
	vector<uint> available_pieces;
	available_pieces.push_back(1);
	available_pieces.push_back(2);
	available_pieces.push_back(3);
	available_pieces.push_back(4);
	available_pieces.push_back(5);

	PuzzleLayout * puzzle_layout = new PuzzleLayout(puzzle);

	//The first piece is placed statically.
	puzzle_layout->place_piece(0, 0, false, Location::Front);

	return solve_puzzle_recursive(puzzle_layout, location_order, available_pieces);
}


PuzzleLayout * PuzzleSolver::solve_puzzle_recursive(PuzzleLayout * puzzle_layout,
		vector<Location::Enum> & location_order, const vector<uint> & available_pieces) {

	ASSERT(!location_order.empty(), "Empty 'location_order', THIS SHOULD NEVER HAPPEN!");

	Location::Enum target_location = location_order.back();
	location_order.pop_back();

	//Do the important stuff
	for(uint i = 0; i < available_pieces.size(); i++) {
		uint target_piece = available_pieces.at(i);

		//A new list of pieces without the 'target_piece' (copy the vector).
		vector<uint> other_pieces = available_pieces;
		other_pieces.erase(other_pieces.begin()+i);

		//For each orientation
		for(uint orientation = 0; orientation < 4; orientation++) {
			puzzle_layout->place_piece(target_piece, orientation, false, target_location);
			if(puzzle_layout->is_valid()) {
				//All locations have been used, all pieces have been placed
				if(location_order.empty()) {
					if(puzzle_layout->is_solution()) {
						return puzzle_layout;
					}
					else {
						ASSERT(false, "All locations have been used and the PuzzleLayout is valid, "
								"but it is not a solution? THIS SHOULD NEVER HAPPEN!");
					}
				}
				//There are more pieces to place
				else {
					PuzzleLayout * return_value = solve_puzzle_recursive(puzzle_layout, location_order, other_pieces);
					if(return_value != NULL) {
						//Pass on the solution
						return return_value;
					}
				}
			}
			puzzle_layout->remove_piece(target_location);
		}
	}
	location_order.push_back(target_location);
	//No solution found
	return NULL;
}

PuzzleLayout * PuzzleSolver::solve_puzzle_with_flipping(const Puzzle& puzzle) {
	//Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	vector<Location::Enum> location_order;
	location_order.push_back(Location::Back);
	location_order.push_back(Location::Right);
	location_order.push_back(Location::Bottom);
	location_order.push_back(Location::Left);
	location_order.push_back(Location::Top);

	//The order here is of less importance for the performance.
	vector<uint> available_pieces;
	available_pieces.push_back(1);
	available_pieces.push_back(2);
	available_pieces.push_back(3);
	available_pieces.push_back(4);
	available_pieces.push_back(5);

	PuzzleLayout * puzzle_layout = new PuzzleLayout(puzzle);

	//The first piece is placed statically.
	puzzle_layout->place_piece(0, 0, false, Location::Front);

	return solve_puzzle_with_flipping_recursive(puzzle_layout, location_order, available_pieces);
}

PuzzleLayout * PuzzleSolver::solve_puzzle_with_flipping_recursive(PuzzleLayout * puzzle_layout,
		vector<Location::Enum> & location_order, const vector<uint> & available_pieces) {

	ASSERT(!location_order.empty(), "Empty 'location_order', THIS SHOULD NEVER HAPPEN!");

	Location::Enum target_location = location_order.back();
	location_order.pop_back();

	//Do the important stuff
	bool flip_states[2] = {false, true};
	for(uint i = 0; i < available_pieces.size(); i++) {
		uint target_piece = available_pieces.at(i);

		//A new list of pieces without the 'target_piece' (copy the vector).
		vector<uint> other_pieces = available_pieces;
		other_pieces.erase(other_pieces.begin()+i);

		//For each orientation
		for(uint orientation = 0; orientation < 4; orientation++) {
			//For each flipstate
			for(uint f_i = 0; f_i < 2; f_i++) {
				bool flip_state = flip_states[f_i];
				puzzle_layout->place_piece(target_piece, orientation, flip_state, target_location);
				if(puzzle_layout->is_valid()) {
					//All locations have been used, all pieces have been placed
					if(location_order.empty()) {
						if(puzzle_layout->is_solution()) {
							return puzzle_layout;
						}
						else {
							ASSERT(false, "All locations have been used and the PuzzleLayout is valid, "
									"but it is not a solution? THIS SHOULD NEVER HAPPEN!");
						}
					}
					//There are more pieces to place
					else {
						PuzzleLayout * return_value = solve_puzzle_with_flipping_recursive(puzzle_layout, location_order, other_pieces);
						if(return_value != NULL) {
							//Pass on the solution
							return return_value;
						}
					}
				}
				puzzle_layout->remove_piece(target_location);
			}
		}
	}
	location_order.push_back(target_location);
	//No solution found
	return NULL;
}

uint PuzzleSolver::count_solutions(const Puzzle& puzzle) {
	//Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	vector<Location::Enum> location_order;
	location_order.push_back(Location::Back);
	location_order.push_back(Location::Right);
	location_order.push_back(Location::Bottom);
	location_order.push_back(Location::Left);
	location_order.push_back(Location::Top);

	//The order here is of less importance for the performance.
	vector<uint> available_pieces;
	available_pieces.push_back(1);
	available_pieces.push_back(2);
	available_pieces.push_back(3);
	available_pieces.push_back(4);
	available_pieces.push_back(5);

	PuzzleLayout puzzle_layout(puzzle);

	//The first piece is placed statically.
	puzzle_layout.place_piece(0, 0, false, Location::Front);

	return count_solutions_recursive(puzzle_layout, location_order, available_pieces);
}

uint PuzzleSolver::count_solutions_recursive(PuzzleLayout & puzzle_layout,
		vector<Location::Enum> & location_order, const vector<uint> & available_pieces) {

	ASSERT(!location_order.empty(), "Empty 'location_order', THIS SHOULD NEVER HAPPEN!");

	uint nr_of_found_solutions = 0;
	Location::Enum target_location = location_order.back();
	location_order.pop_back();

	//Do the important stuff
	bool flip_states[2] = {false, true};
	for(uint i = 0; i < available_pieces.size(); i++) {
		uint target_piece = available_pieces.at(i);

		//A new list of pieces without the 'target_piece' (copy the vector).
		vector<uint> other_pieces = available_pieces;
		other_pieces.erase(other_pieces.begin()+i);

		//For each orientation
		for(uint orientation = 0; orientation < 4; orientation++) {
			//For each flipstate
			for(uint f_i = 0; f_i < 2; f_i++) {
				bool flip_state = flip_states[f_i];
				puzzle_layout.place_piece(target_piece, orientation, flip_state, target_location);
				if(puzzle_layout.is_valid()) {
					//All locations have been used, all pieces have been placed
					if(location_order.empty()) {
						if(puzzle_layout.is_solution()) {
							nr_of_found_solutions += 1;
						}
						else {
							ASSERT(false, "All locations have been used and the PuzzleLayout is valid, "
									"but it is not a solution? THIS SHOULD NEVER HAPPEN!");
						}
					}
					//There are more pieces to place
					else {
						nr_of_found_solutions += count_solutions_recursive(puzzle_layout, location_order, other_pieces);
					}
				}
				puzzle_layout.remove_piece(target_location);
			}
		}
	}
	location_order.push_back(target_location);
	return nr_of_found_solutions;
}
