#include "SudokuInterface.h"

int main(int argc, char **argv)
{
	std::string templatePath;
	std::vector<std::vector<int>> Grid;

	if (argc < 2)
		templatePath = TEMPLATE;
	else
		templatePath = (argv[1]);

	Classifier getDigitsFromImage(templatePath);
	if (!getDigitsFromImage.LoadData())
		Grid = getDigitsFromImage.getFinalVector();
	else
		return EXIT_FAILURE;
	SudokuSolver Solver;
	Solver.setGrid(Grid);
	Solver.solve();
	getDigitsFromImage.getGrid();
	return EXIT_SUCCESS;
}