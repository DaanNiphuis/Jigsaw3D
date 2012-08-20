#!/usr/bin/python
# -*- coding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
import unittest

import cube_puzzle_solver as cps


class TestSequenceFunctions(unittest.TestCase):

	def setUp(self):
		pass


	def test_PuzzlePiece_validation(self):
		correct_field = [
			0,1,0,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			1,1,1,1,1,
			0,0,1,0,0
		]
		
		#There is a hole in the middle
		wrong_field_1 = [
			0,1,0,1,1,
			1,0,1,1,1,
			0,1,1,1,0,
			1,1,1,1,1,
			0,0,1,0,0
		]
		#There is a hole in the middle
		wrong_field_2 = [
			0,1,0,1,1,
			1,1,1,1,1,
			0,1,0,1,0,
			1,1,1,1,1,
			0,0,1,0,0
		]
		#Floating corner
		wrong_field_3 = [
			0,1,0,1,1,
			1,1,1,1,1,
			0,1,1,1,0,
			0,1,1,1,1,
			1,0,1,0,0
		]
		#Invalid field value '2'
		wrong_field_4 = [
			0,1,0,1,1,
			1,1,2,1,1,
			0,1,1,1,0,
			0,1,1,1,1,
			1,0,1,0,0
		]
		#Invalid field value '-1'
		wrong_field_5 = [
			0,1,0,1,1,
			1,1,-1,1,1,
			0,1,1,1,0,
			0,1,1,1,1,
			1,0,1,0,0
		]
		
		#This should not raise an Exception
		cps.PuzzlePiece(5, correct_field)
		
		#Wrong gridwidth
		with self.assertRaises(ValueError):
			cps.PuzzlePiece(6, correct_field)
		
		with self.assertRaises(ValueError):
			cps.PuzzlePiece(5, wrong_field_1)
		
		with self.assertRaises(ValueError):
			cps.PuzzlePiece(5, wrong_field_2)

		with self.assertRaises(ValueError):
			cps.PuzzlePiece(5, wrong_field_3)

		with self.assertRaises(ValueError):
			cps.PuzzlePiece(5, wrong_field_4)

		with self.assertRaises(ValueError):
			cps.PuzzlePiece(5, wrong_field_5)

	def test_Puzzle(self):
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
	
	def test_Puzzle_validation(self):
		#Invalid number of pieces
		with self.assertRaises(ValueError):
			puzzle = cps.Puzzle(5, [
				[
					0,1,0,1,1,
					1,1,1,1,1,
					0,1,1,1,0,
					1,1,1,1,1,
					0,0,1,0,0
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
		
		#Unsolvable not enough corners
		with self.assertRaises(ValueError):
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
					0,1,0,1,0,
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
			
		#Unsolvable to much edges
		with self.assertRaises(ValueError):
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
					0,1,1,1,1,
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

	def test_PuzzleLayout(self):
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
		puzzle_layout = cps.PuzzleLayout(puzzle)
		#Can't place the same piece twice in different places
		puzzle_layout.place_piece(0, 0, False, "Front")
		with self.assertRaises(Exception):
			puzzle_layout.place_piece(0, 0, False, "Back")
	
	
	def test_PuzzleLayout_is_valid(self):
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
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(4, 1, False, 'Top')
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(2, 3, False, 'Left')
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(0, 1, False, 'Front')
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(5, 3, False, 'Right')
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(1, 0, False, 'Bottom')
		self.assertTrue(layout.is_valid())
		self.assertFalse(layout.is_solution())
		layout.place_piece(3, 1, False, 'Back')
		self.assertTrue(layout.is_valid())
		self.assertTrue(layout.is_solution())


if __name__ == '__main__':
	unittest.main()

