#!/usr/bin/python
# -*- coding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
import cube_puzzle_solver as cps


puzzle = cps.Puzzle(5, [
	[
		0,1,0,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		1,1,1,1,1,
		0,0,1,0,0
	],[
		1,0,1,0,0,
		1,1,1,1,1,
		0,1,1,1,0,
		1,1,1,1,0,
		1,0,1,0,0
	],[
		0,0,1,0,0,
		0,1,1,1,1,
		1,1,1,1,1,
		0,1,1,1,0,
		0,1,0,1,0
	],[
		1,1,0,1,1,
		1,1,1,1,0,
		0,1,1,1,1,
		1,1,1,1,0,
		0,1,0,1,1
	],[
		0,1,0,1,1,
		1,1,1,1,0,
		0,1,1,1,1,
		1,1,1,1,0,
		0,0,0,1,1
	],[
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1,
		1,1,1,1,0,
		0,0,1,0,0
	]
])


solutions = cps.count_solutions(puzzle)
print("Number of found solutions: %d" % solutions)

