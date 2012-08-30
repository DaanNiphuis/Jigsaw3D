/*
 * PuzzleSolver.h
 *
 *  Created on: 24 aug. 2012
 *      Author: marcel
 */

#ifndef PUZZLESOLVER_H_
#define PUZZLESOLVER_H_

#include <string>
#include <vector>
#include "common.h"

class Puzzle;
class PuzzleLayout;

class PuzzleSolver {
	PuzzleSolver(void);	//Don't create this class, it's pointless.

	static PuzzleLayout * solve_puzzle_recursive(PuzzleLayout * puzzle_layout, std::vector<Location_t> & location_order, const std::vector<uint> & available_pieces);
	static PuzzleLayout * solve_puzzle_with_flipping_recursive(PuzzleLayout * puzzle_layout, std::vector<Location_t> & location_order, const std::vector<uint> & available_pieces);
	static uint count_solutions_recursive(PuzzleLayout & puzzle_layout, std::vector<Location_t> & location_order, const std::vector<uint> & available_pieces);

public:
	virtual ~PuzzleSolver(void);

	static PuzzleLayout * solve_puzzle(const Puzzle & puzzle);
	static PuzzleLayout * solve_puzzle_with_flipping(const Puzzle & puzzle);
	static uint count_solutions(const Puzzle & puzzle);
};

#endif /* PUZZLESOLVER_H_ */
