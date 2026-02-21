#pragma once

#include "Grid.h"
#include "Algorithm.h"

class IPathFinder
{
public:
	virtual ~IPathFinder() = default;
	virtual void start(Grid& grid) = 0;
	virtual void step(Grid& grid) = 0;
	virtual AlgorithmState getState() const = 0;
};