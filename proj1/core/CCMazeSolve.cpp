#include <ics46/factory/DynamicFactory.hpp>
#include "CCMazeSolve.hpp"
#include <iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, CCMazeSolve, "CamCam's Maze Solver (Required)");

void CCMazeSolve::solveMaze(const Maze& maze, MazeSolution& mazeSolution) {
    //Reset mazeSolution
    mazeSolution.restart();
    
    //Create and initialize checked vector
    int v_rows = maze.getHeight();
    int v_col = maze.getWidth();
    std::vector<std::vector<bool>> checked(v_rows, std::vector<bool>(v_col));
    for (int i = 0; i < v_rows; ++i){
        for (int j = 0; j < v_col; ++j){
            checked[i][j] = false;
        }
    }

    //Create source of random directions
    std::random_device device;
    std::default_random_engine engine{device()};

    //Call recursive helper function to solve maze
    int init_r = mazeSolution.getStartingCell().first;
    int init_c = mazeSolution.getStartingCell().second;
    CCMSRecursive(maze, mazeSolution, engine, checked, init_r, init_c);}

void CCMazeSolve::CCMSRecursive(const Maze& maze, MazeSolution& mazeSolution, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c) {
    while(true) {
        if (mazeSolution.isComplete()) {
            return;
        }
        
        if (!checkArea(maze, checked, r, c, false)) {
            checked[r][c] = true;
            mazeSolution.backUp();
            return;
        }

        //Recursive Logic:
        Direction direction = getValidDirection(maze, engine, checked, r, c);
        mazeSolution.extend(direction);
        checked[r][c] = true;

        if (direction == Direction::up) {
            CCMSRecursive(maze, mazeSolution, engine, checked, r - 1, c);
        }
        if (direction == Direction::right) {
            CCMSRecursive(maze, mazeSolution, engine, checked, r, c + 1);
        }
        if (direction == Direction::down) {
            CCMSRecursive(maze, mazeSolution, engine, checked, r + 1, c);
        }
        if (direction == Direction::left) {
            CCMSRecursive(maze, mazeSolution, engine, checked, r, c - 1);
        }
    }
}

Direction CCMazeSolve::getValidDirection(const Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c) {
    std::discrete_distribution<int> distribution{1, 5, 0, 1};

    int max_row = maze.getHeight() - 1;
    int max_col = maze.getWidth() - 1;

    while(true) {

        //Direction: 1 = up, 2 = right, 3 = down, 4 = left
        int direction = distribution(engine) + 1;

        if (r != max_row && !checked[r + 1][c] && !maze.wallExists(c, r, Direction::down)) {
            return Direction::down;
        }

        if (direction == 1) {
            if (r == 0) {
                continue;
            }
            if (checked[r - 1][c]) {
                continue;
            }
            if (maze.wallExists(c, r, Direction::up)) {
                continue;
            }
            return Direction::up;
        }

        if (direction == 2) {
            if (c == max_col) {
                continue;
            }
            if (checked[r][c + 1]) {
                continue;
            }
            if (maze.wallExists(c, r, Direction::right)) {
                continue;
            }
            return Direction::right;
        }

        if (direction == 4) {
            if (c == 0) {
                continue;
            }
            if (checked[r][c - 1]) {
                continue;
            }
            if (maze.wallExists(c, r, Direction::left)) {
                continue;
            }
            return Direction::left;
        }
    }
}

bool CCMazeSolve::checkArea(const Maze& maze, std::vector<std::vector<bool>>& checked, int r, int c, bool debug) { 
    //Base Case 1.1: Bottom Row
    if (r == maze.getHeight() - 1) {
        //Base Case 1.1.1: Bottom row left corner
        if (c == 0 && checked[r - 1][c] && checked[r][c + 1]) {
            return false;
        }

        //Base Case 1.1.2: Bottom row right corner
        if (c == maze.getWidth() - 1 && checked[r - 1][c] && checked[r][c - 1]) {
            return false;
        }

        //Base Case 1.1.3: Bottom row non-corner
        if ((c < maze.getWidth() - 1 && c > 0) && checked[r - 1][c] && checked[r][c + 1] && checked[r][c - 1]) {
            return false;
        }
        return true;
    }

    if (debug) {std::cout << "BC 1.1" << std::endl;}

    //Base Case 1.2: Top Row
    if (r == 0) {
        //Base Case 1.2.1: Top row left corner
        if (c == 0 && checked[r + 1][c] && checked[r][c + 1]) {
            return false;
        }

        //Base Case 1.2.2: Top row right corner
        if (c == maze.getWidth() - 1 && checked[r + 1][c] && checked[r][c - 1]) {
            return false;
        }

        //Base Case 1.2.3: Top row non-corner
        if ((c < maze.getWidth() - 1 && c > 0) && checked[r][c - 1] && checked[r][c + 1] && checked[r + 1][c]) {
            return false;
        }
        return true;
    }

    if (debug) {std::cout << "BC 1.2" << std::endl;}

    //Base Case 1.3: Right-most Col
    if (c == maze.getWidth() - 1) {
        //Base Case 1.3.1: Right side non-corner
        if ((r < maze.getHeight() - 1 && r > 0) && checked[r + 1][c] && checked[r - 1][c] && checked[r][c - 1]) {
            return false;
        }
        return true;
    }

    if (debug) {std::cout << "BC 1.3" << std::endl;}

    //Base Case 1.4: Left-most Col
    if (c == 0) {
        //Base Case 1.4.1: Left side non-corner
        if ((r < maze.getHeight() - 1 && r > 0) && checked[r - 1][c] && checked[r + 1][c] && checked[r][c + 1]) {
            return false;
        }
        return true;
    }

    if (debug) {std::cout << "BC 1.4" << std::endl;}

    //Base Case 2: Coords are the inner boxes
    if ((r < maze.getHeight() - 1 && r > 0 && c < maze.getWidth() - 1 && c > 0) && checked[r - 1][c] && checked[r + 1][c] && checked[r][c - 1] && checked[r][c + 1]) {
        return false;
    }
    return true;

    if (debug) {std::cout << "BC 2" << std::endl;}
}