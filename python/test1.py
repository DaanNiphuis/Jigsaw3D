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

piece0 = puzzle.get_piece(0)
print(piece0.get_field_str(0, False))
print(piece0.get_field_str(0, True))
print(piece0.get_field_str(1, False))
print(piece0.get_field_str(1, True))


