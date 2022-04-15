#include "SudokuSolver.h"

int main(int argc, const char ** argv)
{
	std::vector<std::vector<int>> sample{	{5,3,0,0,7,0,0,0,0},
											{6,0,0,1,9,5,0,0,0},
											{0,9,8,0,0,0,0,6,0},
											{8,0,0,0,6,0,0,0,3},
											{4,0,0,8,0,3,0,0,1},
											{7,0,0,0,2,0,0,0,6},
											{0,6,0,0,0,0,2,8,0},
											{0,0,0,4,1,9,0,0,5},
											{0,0,0,0,8,0,0,0,0}
										};
	//std::vector<std::vector<int>> input(9,std::vector<int>(9));
	std::vector<std::vector<int>> input;
	if (argc < 2)
	{
		input.swap(sample);
	}
	else
	{
		std::ifstream file;
		file.open(std::string(DIR) + std::string(argv[1]));
		int n;
		if (file.is_open())
		{
			for (size_t i = 0; i < ROWS; ++i)
			{
				std::vector<int> tdata;
				int cell;
				for (size_t j = 0; j < ROWS; ++j)
				{
					file >> cell;
					tdata.push_back(cell);
				}
				input.push_back(tdata);
			}
		}
		else
		{
			input.swap(sample);
			std::cerr << "\nUnable to open Data File. \"" << argv[1] << "\"\n";
			return EXIT_FAILURE;
		}
	}
	SudokuSolver S;
	S.setGrid(input);
	S.solve();
	return EXIT_SUCCESS;
}