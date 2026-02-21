#pragma once

#include <vector>
#include <optional>

enum class CellState {
	Empty,
	Wall,
	Start,
	Goal,
	Open,
	Closed,
	Path
};

//Structure to get each cells attribute
struct Cell {
	int row;
	int col;
	CellState state = CellState::Empty;
};

class Grid {
	public:
		//Create the full grid
		Grid(int rows, int cols);

		int getRows() const;
		int getCols() const;

		//Get the cell information
		Cell& getCell(int row, int col);
		const Cell& getCell(int row, int col) const;
		bool isInside(int row, int col) const;

		const std::vector<Cell>& getCells() const;

		//Define Start and Goals for the pathfinder
		void setStart(int row, int col);
		void setGoal(int row, int col);

		void clearPathfindingData();

		std::optional<Cell*> getStart();
		std::optional<Cell*> getGoal();

		std::vector<Cell*> getNeighbors(int row, int col);
		std::vector<Cell*> getNeighbors(const Cell& cell);

	private:
		int m_rows;
		int m_cols;
		std::vector<Cell> m_cells;

		int index(int row, int col) const;

		std::optional<int> m_startIndex;
		std::optional<int> m_goalIndex;
};
