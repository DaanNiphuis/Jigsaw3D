/*
 * PuzzleSolver.cpp
 *
 *  Created on: 24 aug. 2012
 *      Author: marcel
 */

#include "PuzzleSolver.h"
#include "Puzzle.h"
#include "PuzzleLayout.h"
#include <cstddef>
#include <iostream>
using namespace std;

PuzzleSolver::PuzzleSolver() {
	//Don't instantiate this class.

}

PuzzleSolver::~PuzzleSolver() {
	//Empty.
}


PuzzleLayout * PuzzleSolver::solve_puzzle(const Puzzle& puzzle) {
	//Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	vector<Location_t> location_order;
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

	return PuzzleSolver::solve_puzzle_recursive(puzzle_layout, location_order, available_pieces);
}


PuzzleLayout * PuzzleSolver::solve_puzzle_recursive(PuzzleLayout * puzzle_layout,
		vector<Location_t> & location_order, const vector<uint> & available_pieces) {
	Location_t target_location = location_order.back();
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
						raise_Exception("All locations have been used and the PuzzleLayout is valid, "
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

	/*
	if not isinstance(puzzle_layout, PuzzleLayout):
		raise TypeError("Argument 'puzzle_layout' must be argument of class 'PuzzleLayout'")

	if not location_order:
		raise ValueError("Empty 'location_order', THIS SHOULD NEVER HAPPEN!")

	target_location = location_order.pop()

	#Cool slow mode :)
	#print()
	#print()
	#print(puzzle_layout)
	#print()
	#print()
	#time.sleep(3)

	#Do the important stuff
	for target_piece_index in range(len(available_pieces)):
		target_piece = available_pieces[target_piece_index]
		#A new list of pieces without the 'target_piece'.
		other_pieces = available_pieces[:target_piece_index] + available_pieces[(target_piece_index + 1):]

		#For each orientation
		for orientation in range(4):
			puzzle_layout.place_piece(target_piece, orientation, False, target_location)
			if puzzle_layout.is_valid():
				#All locations have been used, all pieces have been placed
				if not location_order:
					if puzzle_layout.is_solution():
						return puzzle_layout
					else:
						raise Exception("All locations have been used and the PuzzleLayout is valid, " \
								+ "but it is not a solution? THIS SHOULD NEVER HAPPEN!")
				#There are more pieces to place
				else:
					return_value = _solve_puzzle_recursive(puzzle_layout, location_order, other_pieces)
					if return_value != False:
						#Pass on the solution
						return return_value
			puzzle_layout.remove_piece(target_location)

	location_order.append(target_location)
	#No solution found
	return False
	*/
}

PuzzleLayout * PuzzleSolver::solve_puzzle_with_flipping(const Puzzle& puzzle) {
	/*
	if not isinstance(puzzle, Puzzle):
		raise TypeError("Argument 'puzzle' must be argument of class 'Puzzle'")

	#Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	location_order = ["Back", "Right", "Bottom", "Left", "Top"]

	#The order here is of less importance for the performance.
	available_pieces = [1, 2, 3, 4, 5]

	puzzle_layout = PuzzleLayout(puzzle)
	#The first piece is placed statically.
	puzzle_layout.place_piece(0, 0, False, "Front")

	return _solve_puzzle_with_flipping_recursive(puzzle_layout, location_order, available_pieces)
	*/
	return NULL;
}

PuzzleLayout * PuzzleSolver::solve_puzzle_with_flipping_recursive(PuzzleLayout * puzzle_layout,
		vector<Location_t> * location_order, vector<uint> available_pieces) {
	/*
	if not isinstance(puzzle_layout, PuzzleLayout):
		raise TypeError("Argument 'puzzle_layout' must be argument of class 'PuzzleLayout'")

	if not location_order:
		raise ValueError("Empty 'location_order', THIS SHOULD NEVER HAPPEN!")

	target_location = location_order.pop()

	#Cool slow mode :)
	#print()
	#print()
	#print(puzzle_layout)
	#print()
	#print()
	#time.sleep(1)

	#Do the important stuff
	for target_piece_index in range(len(available_pieces)):
		target_piece = available_pieces[target_piece_index]
		#A new list of pieces without the 'target_piece'.
		other_pieces = available_pieces[:target_piece_index] + available_pieces[(target_piece_index + 1):]

		flip_states = [False, True]
		#For each orientation
		for orientation in range(4):
			for flip_state in flip_states:
				puzzle_layout.place_piece(target_piece, orientation, flip_state, target_location)



				print()
				print()
				print(puzzle_layout)
				print()
				print()
				time.sleep(0.1)

				if puzzle_layout.is_valid():
					#All locations have been used, all pieces have been placed
					if not location_order:
						if puzzle_layout.is_solution():
							return puzzle_layout
						else:
							raise Exception("All locations have been used and the PuzzleLayout is valid, " \
									+ "but it is not a solution? THIS SHOULD NEVER HAPPEN!")
					#There are more pieces to place
					else:
						return_value = _solve_puzzle_with_flipping_recursive(puzzle_layout, location_order, other_pieces)
						if return_value != False:
							#Pass on the solution
							return return_value
				puzzle_layout.remove_piece(target_location)

	location_order.append(target_location)
	#No solution found
	return False
	*/
	return NULL;
}

uint PuzzleSolver::count_solutions(const Puzzle& puzzle) {
	/*
	if not isinstance(puzzle, Puzzle):
		raise TypeError("Argument 'puzzle' must be argument of class 'Puzzle'")

	#Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	location_order = ["Back", "Right", "Bottom", "Left", "Top"]
	#The order here is of less importance for the performance.
	available_pieces = [1, 2, 3, 4, 5]

	puzzle_layout = PuzzleLayout(puzzle)
	#The first piece is placed statically.
	puzzle_layout.place_piece(0, 0, False, "Front")

	return _count_solutions_recursive(puzzle_layout, location_order, available_pieces)
	*/
	return 0;
}

uint PuzzleSolver::count_solutions_recursive(PuzzleLayout * puzzle_layout,
		vector<Location_t> * location_order, vector<uint> available_pieces) {
	/*
	if not isinstance(puzzle_layout, PuzzleLayout):
		raise TypeError("Argument 'puzzle_layout' must be argument of class 'PuzzleLayout'")

	if not location_order:
		raise ValueError("Empty 'location_order', THIS SHOULD NEVER HAPPEN!")

	nr_of_found_solutions = 0
	target_location = location_order.pop()

	#Do the important stuff
	for target_piece_index in range(len(available_pieces)):

		target_piece = available_pieces[target_piece_index]
		#A new list of pieces without the 'target_piece'.
		other_pieces = available_pieces[:target_piece_index] + available_pieces[(target_piece_index + 1):]

		#For each orientation
		for orientation in range(4):
			puzzle_layout.place_piece(target_piece, orientation, False, target_location)
			if puzzle_layout.is_valid():
				#All locations have been used, all pieces have been placed
				if not location_order:
					if puzzle_layout.is_solution():
						nr_of_found_solutions += 1
					else:
						raise Exception("All locations have been used and the PuzzleLayout is valid, " \
								+ "but it is not a solution? THIS SHOULD NEVER HAPPEN!")
				#There are more pieces to place
				else:
					return_value = _count_solutions_recursive(puzzle_layout, location_order, other_pieces)
					nr_of_found_solutions += return_value
			puzzle_layout.remove_piece(target_location)

	location_order.append(target_location)
	#No solution found
	return nr_of_found_solutions
	*/
	return 0;
}
