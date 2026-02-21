#include <SFML/Graphics.hpp>
#include <iostream>

#include "Grid.h"
#include "BFSPathFinder.h"

int main()
{
    //Define the amount of rows and cols as well as the size of the cells in the grid
    constexpr int ROWS = 20;
	constexpr int COLS = 30;
	constexpr int CELL_SIZE = 25;

	constexpr int WINDOW_WIDTH = COLS * CELL_SIZE;
	constexpr int WINDOW_HEIGHT = ROWS * CELL_SIZE;

    //Renders the window
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), 
        "Algorithm!"
    );

    Cell* hoveredCell = nullptr;
    Grid grid(ROWS, COLS);
    BFSPathFinder bfs;

	sf::RectangleShape cellShape;
	cellShape.setSize({ 
        static_cast<float>(CELL_SIZE - 1), 
        static_cast<float>(CELL_SIZE - 1) 
        });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>())
            {
				//Start the algorithm with Space key
                if (keypressed->scancode == sf::Keyboard::Scan::Space) {
                    bfs.start(grid);
                }
                
				//Clear the pathfinding data with R key
                if (keypressed->scancode == sf::Keyboard::Scan::R)
                {
                    grid.clearPathfindingData();
                }
            }

            //Get mouse info
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                //Don't allow mouse input while running the algorithm
                if (bfs.getState() == AlgorithmState::Running) {
                    continue;
                }

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                //Finds the exact grid position
                int col = mousePos.x / CELL_SIZE;
                int row = mousePos.y / CELL_SIZE;


                if (grid.isInside(row, col)) {
                    hoveredCell = &grid.getCell(row, col);
                }
                else {
                    hoveredCell = nullptr;
                }

                Cell& cell = grid.getCell(row, col);

				bool sPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S); //Hold S to set the start point
				bool gPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::G); //Hold G to set the goal point

                //Creates the wall with Left mouse button
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (sPressed && cell.state != CellState::Wall) {
                        grid.setStart(row, col);
                    }
                    else if (gPressed && cell.state != CellState::Wall) {
                        grid.setGoal(row, col);
                    }
                    else if (cell.state == CellState::Empty) {
                        cell.state = CellState::Wall;
                    }
                 }
                 //Erase the wall with Right mouse button
                 else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    if (cell.state != CellState::Start && cell.state != CellState::Goal) {
                        cell.state = CellState::Empty;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        if (bfs.getState() == AlgorithmState::Running) {
            bfs.step(grid);
        }

        //Draw the grid on screen
        for (const Cell& cell : grid.getCells())
        {
			float x = static_cast<float>(cell.col * CELL_SIZE);
			float y = static_cast<float>(cell.row * CELL_SIZE);

			cellShape.setPosition(sf::Vector2(x, y));

            switch (cell.state)
            {
                case CellState::Empty:
                    cellShape.setFillColor(sf::Color::White);
					break;
                case CellState::Wall:
					cellShape.setFillColor(sf::Color::Black);
                    break;
                case CellState::Start:
                    cellShape.setFillColor(sf::Color::Green);
                    break;
                case CellState::Goal:
                    cellShape.setFillColor(sf::Color::Red);
                    break;
                case CellState::Open:
                    cellShape.setFillColor(sf::Color::Cyan);
                    break;
                case CellState::Closed:
                    cellShape.setFillColor(sf::Color::Magenta);
                    break;
                case CellState::Path:
                    cellShape.setFillColor(sf::Color::Yellow);
                    break;
                default:
                    break;
            }

			window.draw(cellShape);
        }
        
        window.display();
    }

	return 0;
}