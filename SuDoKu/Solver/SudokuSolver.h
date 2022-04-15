#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#define ROWS 9
#define DIR "C:/sakeofShit/VS/myOpenCVPractice/SuDoKu/"

class SudokuSolver
{
	public:
		SudokuSolver();
		~SudokuSolver();
		void solve();
		void setGrid(std::vector<std::vector<int>>&);
	private:
		unsigned int id;
		unsigned int size;
		std::vector<std::vector<int>> grid;
	private:
		bool possible(unsigned int, unsigned int, int);
		void out();
		void message();
};