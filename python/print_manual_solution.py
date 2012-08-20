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


layout = cps.PuzzleLayout(puzzle)
layout.place_piece(4, 1, False, 'Top')
layout.place_piece(2, 3, False, 'Left')
layout.place_piece(0, 1, False, 'Front')
layout.place_piece(5, 3, False, 'Right')
layout.place_piece(1, 0, False, 'Bottom')
layout.place_piece(3, 1, False, 'Back')

print(layout)
