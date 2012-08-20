#!/bin/python
# -*- coding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
import sys
import time

class PuzzlePiece:
	def __init__(self, gridwidth, piece_shape):
		if gridwidth < 3:
			raise ValueError("Gridwidth %d is below minimum of 3" % gridwidth)
		if len(piece_shape) != (gridwidth * gridwidth):
			raise ValueError("Shape does not match gridwidth %d" % gridwidth)
		
		for point in piece_shape:
			if point != 0 and point != 1:
				raise ValueError("Only '0' or '1' allowed for shape specification.")
		self._gridwidth = gridwidth
		self._piece_shape = piece_shape
		
		#Check for 'floating' corners
		#Use orientation for easy checking
		for orientation in range(4):
			if self.get_point(orientation, 0, 0) == 1 \
			and self.get_point(orientation, 1, 0) == 0 \
			and self.get_point(orientation, 0, 1) == 0:
				raise ValueError("Floating corner found in corner %d" % orientation)
		
		#Check for holes in the center
		for row in range(1, gridwidth - 1):
			for column in range(1, gridwidth - 1):
				if self.get_point(0, row, column) != 1:
					raise ValueError("Center should be completely filled")
	
	
	#Used in validation
	def get_nr_of_edge_bits(self):
		edge_bits = 0
		for index in range(1, self._gridwidth - 1):
			if self.get_point(0, 0, index) == 1:
				edge_bits += 1
			if self.get_point(0, self._gridwidth - 1, index) == 1:
				edge_bits += 1
			if self.get_point(0, index, 0) == 1:
				edge_bits += 1
			if self.get_point(0, index, self._gridwidth - 1) == 1:
				edge_bits += 1
		return edge_bits

	def get_nr_of_corner_bits(self):
		corner_bits = self.get_point(0, 0, 0)
		corner_bits += self.get_point(0, 0, self._gridwidth - 1)
		corner_bits += self.get_point(0, self._gridwidth - 1, 0)
		corner_bits += self.get_point(0, self._gridwidth - 1, self._gridwidth - 1)
		return corner_bits
	
	
	def __str__(self):
		return self.get_field_str(0, False)

	def get_point_index(self, row_number, col_number):
		return (row_number * self._gridwidth) + col_number;

	def get_point(self, orientation, row_number, col_number):
		if row_number < 0 or row_number >= self._gridwidth:
			raise ValueError("Invalid row_number %d" % row_number)
		elif col_number < 0 or col_number >= self._gridwidth:
			raise ValueError("Invalid col_number %d" % col_number)
		elif orientation < 0 or orientation >= 4:
			raise Exception("Invalid orientation %d" % orientation)
		
		index = -1
		if orientation == 0:
			index = self.get_point_index(row_number, col_number)
		elif orientation == 1:
			index = self.get_point_index(self._gridwidth - col_number - 1, row_number)
		elif orientation == 2:
			index = self.get_point_index(self._gridwidth - row_number - 1, self._gridwidth - col_number - 1)
		elif orientation == 3:
			index = self.get_point_index(col_number, self._gridwidth - row_number - 1)
		return self._piece_shape[index]

	def get_flippable_point(self, orientation, flipped, row_number, col_number):
		if flipped:
			return self.get_point(orientation, row_number, self._gridwidth - col_number - 1)
		else:
			return self.get_point(orientation, row_number, col_number)
	
	def get_point_str(self, orientation, flipped, row_number, col_number):
		if self.get_flippable_point(orientation, flipped, row_number, col_number) == 1:
			return "X"
		else:
			return " "
	
	def get_row_str(self, orientation, flipped, row_number):
		output = ""
		for col_number in range(self._gridwidth):
			output += self.get_point_str(orientation, flipped, row_number, col_number)
		return output
	
	def get_field_str(self, orientation, flipped):
		output = ""
		for row_number in range(self._gridwidth):
			output += "%s\n" % (self.get_row_str(orientation, flipped, row_number))
		return output



class Puzzle:
	def __init__(self, gridwidth, piece_shapes):
		self._gridwidth = gridwidth
		if len(piece_shapes) != 6:
			raise ValueError("You can only create puzzles with 6 pieces")
		
		self._pieces = []
		for shape in piece_shapes:
			self._pieces.append(PuzzlePiece(self._gridwidth, shape))

		#Validation
		expected_edge_bits = ((gridwidth - 2) * 12)
		actual_edge_bits = 0
		for piece in self._pieces:
			actual_edge_bits += piece.get_nr_of_edge_bits()
		if(expected_edge_bits != actual_edge_bits):
			raise ValueError("Expected edge bits %d did not match actual edge bits %d. Puzzle is unsolvable!" \
					% (expected_edge_bits, actual_edge_bits))
		
		expected_corner_bits = 8
		actual_corner_bits = 0
		for piece in self._pieces:
			actual_corner_bits += piece.get_nr_of_corner_bits()
		if(expected_corner_bits != actual_corner_bits):
			raise ValueError("Expected corner bits %d did not match actual corner bits %d. Puzzle is unsolvable!" \
					% (expected_corner_bits, actual_corner_bits))
	
	
	def __str__(self):
		description = "Puzzle - width: %d\nPieces:\n\n" % (self._gridwidth)
		for piece in self._pieces:
			description += ("%s\n" % (piece))
		return description

	def get_gridwidth(self):
		return self._gridwidth

	def get_piece(self, piece_number):
		return self._pieces[piece_number]



class PuzzleLayout:
	LOCATIONS = ["Front", "Back", "Left", "Right", "Top", "Bottom"]
	
	def __init__(self, puzzle):
		if not isinstance(puzzle, Puzzle):
			raise TypeError("Argument 'puzzle' must be argument of class 'Puzzle'")
		
		self._puzzle = puzzle
		self._placed_pieces = {}
		for location in self.LOCATIONS:
			self._placed_pieces[location] = None
	
	def place_piece(self, piece_index, orientation, flipped, location):
		if not location in self.LOCATIONS:
			raise ValueError("Unknown location %s" % location)
		
		if self._placed_pieces[location] != None:
			raise Exception("Location %s is already uccupied!" % location)
		
		for l in self.LOCATIONS:
			if l != location and self._placed_pieces[l] != None and self._placed_pieces[l]["piece"] == piece_index:
				raise Exception("Piece %d cannot be placed twice in the same layout!" % piece_index)
		
		self._placed_pieces[location] = {
			'piece':piece_index,
			'orientation':orientation,
			'flipped':flipped
		}
	
	def remove_piece(self, location):
		if not location in self._placed_pieces:
			raise ValueError("Unknown location %s" % location)
		
		self._placed_pieces[location] = None
	
	def get_pieces_combined_str(self, placements):
		gridwidth = self._puzzle.get_gridwidth()
		filler_space = ""
		for i in range(gridwidth):
			filler_space += " "
		
		pieces = []
		for placement in placements:
			if placement == None:
				pieces.append(None)
			else:
				pieces.append(self._puzzle.get_piece(placement["piece"]))

		output = ""
		for row_nr in range(gridwidth):
			for index in range(len(pieces)):
				piece = pieces[index]
				if piece == None:
					output += "%s " % filler_space
				else:
					output += "%s " % (piece.get_row_str(
						placements[index]["orientation"],
						placements[index]["flipped"],
						row_nr
					))
			output += "\n"
		
		return output
		
	def __str__(self):
		output = "\n"
		if self._placed_pieces["Top"] != None:
			output += self.get_pieces_combined_str([None, self._placed_pieces["Top"]])
		
		output += "\n"
		
		output += self.get_pieces_combined_str([
			self._placed_pieces["Left"],
			self._placed_pieces["Front"],
			self._placed_pieces["Right"]
		])
		
		if self._placed_pieces["Bottom"] != None or self._placed_pieces["Back"] != None:
			output += "\n"
			output += self.get_pieces_combined_str([
				None,
				self._placed_pieces["Bottom"]
		])
		
		if self._placed_pieces["Back"] != None:
			output += "\n"
			output += self.get_pieces_combined_str([
				None,
				self._placed_pieces["Back"]
		])

		return output
	
	#Use orientation adjustments to compare the lower edge of piece1 with the upper edge of piece2.
	def is_valid_edge(self, location1, orientation_adj1, location2, orientation_adj2):
		if self._placed_pieces[location1] == None or self._placed_pieces[location2] == None:
			return True
		
		gridwidth = self._puzzle.get_gridwidth()
		pp1 = self._placed_pieces[location1]
		pp2 = self._placed_pieces[location2]
		piece1 = self._puzzle.get_piece(pp1["piece"])
		piece2 = self._puzzle.get_piece(pp2["piece"])
		orientation1 = (pp1["orientation"] + orientation_adj1) % 4
		orientation2 = (pp2["orientation"] + orientation_adj2) % 4
		flipped1 = pp1["flipped"]
		flipped2 = pp2["flipped"]
		
		#TODO remove debug output
		#print()
		#print()
		#print(piece1.get_field_str(orientation1, pp1["flipped"]))
		#print(piece2.get_field_str(orientation2, pp2["flipped"]))
		#print()
		#print()
		
		#Check edge
		for index in range(1, gridwidth - 1):
			edge_value = piece1.get_flippable_point(orientation1, flipped1, gridwidth - 1, index)
			edge_value += piece2.get_flippable_point(orientation2, flipped2, 0, index)
			#edge_value = piece1.get_point(orientation1, gridwidth - 1, index)
			#edge_value += piece2.get_point(orientation2, 0, index)
			if edge_value != 1:
				return False
		
		#Check corners partial (can't do full corner check here, need 3 pieces for that)
		corner_left = piece1.get_flippable_point(orientation1, flipped1, gridwidth - 1, 0) 
		corner_left += piece2.get_flippable_point(orientation2, flipped2, 0, 0)
		#corner_left = piece1.get_point(orientation1, gridwidth - 1, 0) 
		#corner_left += piece2.get_point(orientation2, 0, 0)
		if corner_left < 0 or corner_left > 1:
			#print("Fail on left corner")
			return False
		
		corner_right = piece1.get_flippable_point(orientation1, flipped1, gridwidth - 1, gridwidth - 1) 
		corner_right += piece2.get_flippable_point(orientation2, flipped2, 0, gridwidth - 1)
		#corner_right = piece1.get_point(orientation1, gridwidth - 1, gridwidth - 1) 
		#corner_right += piece2.get_point(orientation2, 0, gridwidth - 1)
		if corner_right < 0 or corner_right > 1:
			#print("Fail on right corner")
			return False
		return True
	
	#Use orientation adjustments to compare the lower right corner of piece1 with the
	#upper right corner of piece2 with the upper left corner of piece3. 
	def is_valid_corner(self, location1, orient_adj1, location2, orient_adj2, location3, orient_adj3):
		if self._placed_pieces[location1] == None \
		or self._placed_pieces[location2] == None \
		or self._placed_pieces[location3] == None:
			return True
		
		gridwidth = self._puzzle.get_gridwidth()
		pp1 = self._placed_pieces[location1]
		pp2 = self._placed_pieces[location2]
		pp3 = self._placed_pieces[location3]
		piece1 = self._puzzle.get_piece(pp1["piece"])
		piece2 = self._puzzle.get_piece(pp2["piece"])
		piece3 = self._puzzle.get_piece(pp3["piece"])
		orientation1 = (pp1["orientation"] + orient_adj1) % 4
		orientation2 = (pp2["orientation"] + orient_adj2) % 4
		orientation3 = (pp3["orientation"] + orient_adj3) % 4
		flipped1 = pp1["flipped"]
		flipped2 = pp2["flipped"]
		flipped3 = pp3["flipped"]
		
		
		#TODO remove debug output
		#print()
		#print(piece1.get_field_str(orientation1, pp1["flipped"]))
		#debug_placements = [
		#		{
		#			"piece":pp2["piece"],
		#			"orientation":orientation2,
		#			"flipped":pp2["flipped"]
		#		},{
		#			"piece":pp3["piece"],
		#			"orientation":orientation3,
		#			"flipped":pp3["flipped"]
		#		}
		#	]
		#print(self.get_pieces_combined_str(debug_placements))
		#print()
		corner_bit1 = piece1.get_flippable_point(orientation1, flipped1, gridwidth - 1, gridwidth - 1)
		corner_bit2 = piece2.get_flippable_point(orientation2, flipped2, 0, gridwidth - 1)
		corner_bit3 = piece3.get_flippable_point(orientation3, flipped3, 0, 0)
		
		#corner_bit1 = piece1.get_point(orientation1, gridwidth - 1, gridwidth - 1)
		#corner_bit2 = piece2.get_point(orientation2, 0, gridwidth - 1)
		#corner_bit3 = piece3.get_point(orientation3, 0, 0)
		return corner_bit1 + corner_bit2 + corner_bit3 == 1

	#TODO This propably horribly broken with 'flipped'.
	def is_valid(self):
		#Check edges
		#Edges adjacent to 'Front'.
		if not self.is_valid_edge("Front", 2, "Top", 2):
			return False
		if not self.is_valid_edge("Front", 1, "Right", 1):
			return False
		if not self.is_valid_edge("Front", 0, "Bottom", 0):
			return False
		if not self.is_valid_edge("Front", 3, "Left", 3):
			return False
		#Edges not adjacent to 'Front' or 'Back
		if not self.is_valid_edge("Top", 1, "Right", 0):
			return False
		if not self.is_valid_edge("Right", 0, "Bottom", 3):
			return False
		if not self.is_valid_edge("Bottom", 3, "Left", 2):
			return False
		if not self.is_valid_edge("Left", 2, "Top", 1):
			return False
		#Edges adjacent to 'Back'
		if not self.is_valid_edge("Back", 0, "Top", 0):
			return False
		if not self.is_valid_edge("Back", 1, "Right", 3):
			return False
		if not self.is_valid_edge("Back", 2, "Bottom", 2):
			return False
		if not self.is_valid_edge("Back", 3, "Left", 1):
			return False
		
		#Check corners
		if not self.is_valid_corner("Top", 0, "Front", 0, "Right", 0):
			return False
		if not self.is_valid_corner("Front", 0, "Bottom", 0, "Right", 1):
			return False
		if not self.is_valid_corner("Left", 0, "Bottom", 1, "Front", 1):
			return False
		if not self.is_valid_corner("Top", 3, "Left", 0, "Front", 0):
			return False
		
		if not self.is_valid_corner("Bottom", 0, "Back", 0, "Right", 2):
			return False
		if not self.is_valid_corner("Back", 0, "Top", 0, "Right", 3):
			return False
		if not self.is_valid_corner("Left", 2, "Top", 1, "Back", 1):
			return False
		if not self.is_valid_corner("Bottom", 3, "Left", 2, "Back", 0):
			return False
		return True
	

	def is_solution(self):
		if not self.is_valid():
			return False
		
		if self._placed_pieces["Front"] == None or self._placed_pieces["Back"] == None \
		or self._placed_pieces["Left"] == None or self._placed_pieces["Right"] == None \
		or self._placed_pieces["Top"] == None or self._placed_pieces["Bottom"] == None:
			return False
		
		return True

def solve_puzzle(puzzle):
	if not isinstance(puzzle, Puzzle):
		raise TypeError("Argument 'puzzle' must be argument of class 'Puzzle'")
	
	#Order is backwards, so it is easier to use 'pop()' and 'append()' on the list.
	location_order = ["Back", "Right", "Bottom", "Left", "Top"]
	#The order here is of less importance for the performance.
	available_pieces = [1, 2, 3, 4, 5]
	
	puzzle_layout = PuzzleLayout(puzzle)
	#The first piece is placed statically.
	puzzle_layout.place_piece(0, 0, False, "Front")
	
	return _solve_puzzle_recursive(puzzle_layout, location_order, available_pieces)
	
	
def _solve_puzzle_recursive(puzzle_layout, location_order, available_pieces):
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
	

def solve_puzzle_with_flipping(puzzle):
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


def _solve_puzzle_with_flipping_recursive(puzzle_layout, location_order, available_pieces):
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


def count_solutions(puzzle):
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
	
	
def _count_solutions_recursive(puzzle_layout, location_order, available_pieces):
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
	