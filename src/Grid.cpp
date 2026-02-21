#include "Grid.h"

Grid::Grid(int rows, int cols) : m_rows(rows), m_cols(cols)
{
	m_cells.reserve(rows * cols);

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < cols; ++c) {
			m_cells.push_back({ r,c,CellState::Empty });
		}
	}
}

int Grid::getRows() const
{
	return m_rows;
}

int Grid::getCols() const
{
	return m_cols;
}

int Grid::index(int row, int col) const
{
	return row * m_cols + col;
}

Cell& Grid::getCell(int row, int col)
{
	return m_cells[index(row, col)];
}

const Cell& Grid::getCell(int row, int col) const
{
	return m_cells[index(row, col)];
}

bool Grid::isInside(int row, int col) const
{
	return row >= 0 && row < m_rows && 
		   col >= 0 && col < m_cols;
}

const std::vector<Cell>& Grid::getCells() const
{
	return m_cells;
}

void Grid::setStart(int row, int col)
{
	int idx = index(row, col);
	if (m_startIndex.has_value())
		m_cells[*m_startIndex].state = CellState::Empty;

	m_startIndex = idx;
	m_cells[idx].state = CellState::Start;
}

void Grid::setGoal(int row, int col)
{
	int idx = index(row, col);
	if (m_goalIndex.has_value())
		m_cells[*m_goalIndex].state = CellState::Empty;

	m_goalIndex = idx;
	m_cells[idx].state = CellState::Goal;
}

void Grid::clearPathfindingData()
{
	for (Cell& cell : m_cells)
	{
		if (cell.state == CellState::Open || cell.state == CellState::Closed || cell.state == CellState::Path)
		{
			cell.state = CellState::Empty;
		}
	}
}

std::optional<Cell*> Grid::getStart()
{
	if (!m_startIndex.has_value())
	{
		return std::nullopt;
	}

	return &m_cells[*m_startIndex];
}

std::optional<Cell*> Grid::getGoal()
{
	if (!m_goalIndex.has_value())
	{
		return std::nullopt;
	}

	return &m_cells[*m_goalIndex];
}

std::vector<Cell*> Grid::getNeighbors(int row, int col)
{
	std::vector<Cell*> neighbors;

	const int directions[4][2] =
	{
		{-1, 0}, //up
		{1, 0},  //down
		{0, -1}, //right
		{0, 1},  //left
	};

	for (const auto& dir : directions) {
		int newRow = row + dir[0];
		int newCol = col + dir[1];

		if (!isInside(newRow, newCol))
			continue;

		Cell& neighbor = getCell(newRow, newCol);

		if (neighbor.state == CellState::Wall)
			continue;

		neighbors.push_back(&neighbor);
	}

	return neighbors;
}

std::vector<Cell*> Grid::getNeighbors(const Cell& cell)
{
	return getNeighbors(cell.row, cell.col);
}
