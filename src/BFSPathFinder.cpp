#include "BFSPathFinder.h"

void BFSPathFinder::start(Grid& grid)
{
	while (!m_queue.empty()) m_queue.pop();
	m_visited.clear();
	m_parent.clear();

	auto startOpt = grid.getStart();
	auto goalOpt = grid.getGoal();

	if (!startOpt.has_value() || !goalOpt.has_value()) {
		m_state = AlgorithmState::Failed;
		return;
	}

	m_start = *startOpt;
	m_goal = *goalOpt;

	m_queue.push(m_start);
	m_visited.insert(m_start);

	m_state = AlgorithmState::Running;
}

void BFSPathFinder::step(Grid& grid)
{
	if (m_state != AlgorithmState::Running) {
		return;
	}

	if (m_queue.empty()) {
		m_state = AlgorithmState::Failed;
		return;
	}

	Cell* current = m_queue.front();
	m_queue.pop();

	//Mark as closed
	if (current->state != CellState::Start && current->state != CellState::Goal) {
		current->state = CellState::Closed;
	}

	//Check goal state
	if (current == m_goal) {
		reconstructPath();
		m_state = AlgorithmState::Finished;
		return;
	}

	//Expand neighbors counter
	for (Cell* neighbor : grid.getNeighbors(*current)) {
		if (m_visited.contains(neighbor)) {
			continue;
		}

		m_visited.insert(neighbor);
		m_parent[neighbor] = current;
		m_queue.push(neighbor);

		if (neighbor->state != CellState::Goal) {
			neighbor->state = CellState::Open;
		}
	}
}

AlgorithmState BFSPathFinder::getState() const
{
	return m_state;
}

void BFSPathFinder::reconstructPath()
{
	Cell* current = m_goal;
	while (current && current != m_start) {
		if (current->state != CellState::Goal) {
			current->state = CellState::Path;
		}
		current = m_parent[current];
	}
}
