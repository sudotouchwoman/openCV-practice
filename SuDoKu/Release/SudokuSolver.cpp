#pragma once
#include "SudokuInterface.h"

namespace Sudoku
{
	int SIZE = 9;
	int EMPTY = 0;
};

SudokuSolver::SudokuSolver()
{
	size = Sudoku::SIZE;
	id = Sudoku::EMPTY;
	grid.resize(9);
	for (unsigned i = 0; i < size; ++i)
		grid[i].resize(9);
}

SudokuSolver::~SudokuSolver()
{
}

bool SudokuSolver::possible(unsigned int y,unsigned int x, int num)
{
	for (unsigned i = 0; i < size; ++i)					// check horizontal line
		if (grid[y][i] == num)
			return false;
	for (unsigned i = 0; i < size; ++i)					// check vertical line
		if (grid[i][x] == num)
			return false;
	unsigned x0 = (x / 3) * 3;
	unsigned y0 = (y / 3) * 3;
	for (unsigned i = 0; i < 3; ++i)					// check the corresponding
		for (unsigned j = 0; j < 3; ++j)				// 3x3 square
			if (grid[y0 + i][x0 + j] == num)
				return false;
	return true;
}

void SudokuSolver::solve()
{
	for (unsigned y = 0; y < size; ++y)					// loop through all the cells
		for (unsigned x = 0; x < size; ++x)
			if (grid[y][x] == Sudoku::EMPTY)			// if cell is empty try to fill it
			{
				for (int n = 1; n < 10; ++n)			// look for the match
					if (possible(y, x, n))				// if possible to fill, calls itself
					{									// -> gets one step deeper
						grid[y][x] = n;
						solve();
						grid[y][x] = Sudoku::EMPTY;		// if came to dead end, erase the cell
					}									// & try again with different digit maybe
				return;
			}
	message();			// found a solution
	out();				// print it out
	return;				// get one step back
}

void SudokuSolver::setGrid(std::vector<std::vector<int>> & input)
{
	if (input.size() != size)
	{
		return;
	}
	for (size_t i = 0; i < grid.size(); ++i)
		for (size_t j = 0; j < grid.size(); ++j)
			grid[i][j] = input[i][j];
	std::cout << "\nGRID INITIATED:\n";
	out();
	return;
}

void SudokuSolver::out()
{
	for (size_t i = 0; i < grid.size(); ++i)
	{
		for (size_t j = 0; j < grid.size(); ++j)
			std::cout << ' ' << grid[i][j];
		std::cout << '\n';
	}
	return;
}

void SudokuSolver::message()
{
	std::cout << "\nSOLVED: Solution #" << ++id << std::endl;
	return;
}