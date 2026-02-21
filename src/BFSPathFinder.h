#pragma once

#include "IPathFinder.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>

class BFSPathFinder : public IPathFinder
{
public:
	void start(Grid& grid) override;
	void step(Grid& grid) override;
	AlgorithmState getState() const override;

private:
	AlgorithmState m_state = AlgorithmState::Idle;

	std::queue<Cell*> m_queue;
	std::unordered_set<Cell*> m_visited;
	std::unordered_map<Cell*, Cell*> m_parent;

	Cell* m_start = nullptr;
	Cell* m_goal = nullptr;

	void reconstructPath();
};