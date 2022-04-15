#pragma once
#include "SudokuSolver.h"

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
	for (int i = 0; i < size; ++i)
		grid[i].resize(9);
}

SudokuSolver::~SudokuSolver()
{
}

bool SudokuSolver::possible(unsigned int y,unsigned int x, int num)
{
	for (int i = 0; i < size; ++i)
		if (grid[y][i] == num)
			return false;
	for (int i = 0; i < size; ++i)
		if (grid[i][x] == num)
			return false;
	int x0 = (x / 3) * 3;
	int y0 = (y / 3) * 3;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (grid[double(y0) + i][double(x0) + j] == num)
				return false;
	//std::cout << "possible" << std::endl;
	return true;
}

void SudokuSolver::solve()
{
	//std::cout << "sus" << std::endl;
	for (int y = 0; y < size; ++y)
		for (int x = 0; x < size; ++x)
			if (grid[y][x] == Sudoku::EMPTY)
			{
				for (int n = 1; n < 10; ++n)
					if (possible(y, x, n))
					{
						grid[y][x] = n;
						solve();
						grid[y][x] = Sudoku::EMPTY;
					}
				return;
			}
	message();
	out();
	return;
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