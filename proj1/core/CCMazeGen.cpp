#include <ics46/factory/DynamicFactory.hpp>
#include "CCMazeGen.hpp"
#include <iostream>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, CCMazeGen, "CamCam's Maze Gen (Required)");

void CCMazeGen::generateMaze(Maze& maze){
    //Reset all walls
    maze.addAllWalls();

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

    //Call recursive helper function to generate maze
    CCMGRecursive(maze, engine, checked, 0, 0);

}

void CCMazeGen::CCMGRecursive(Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c) {
    while(true) {
        if (!checkArea(maze, checked, r, c, false)) {
            checked[r][c] = true;
            return;
        }

        //Recursive Logic:
        Direction direction = getValidDirection(maze, engine, checked, r, c);
        maze.removeWall(c, r, direction);
        checked[r][c] = true;

        if (direction == Direction::up) {
            CCMGRecursive(maze, engine, checked, r - 1, c);
        }
        if (direction == Direction::right) {
            CCMGRecursive(maze, engine, checked, r, c + 1);
        }
        if (direction == Direction::down) {
            CCMGRecursive(maze, engine, checked, r + 1, c);
        }
        if (direction == Direction::left) {
            CCMGRecursive(maze, engine, checked, r, c - 1);
        }
    }
}

Direction CCMazeGen::getValidDirection(Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c) {
    std::uniform_int_distribution<int> distribution{1, 4};

    int max_row = maze.getHeight() - 1;
    int max_col = maze.getWidth() - 1;

    while(true) {

        //Direction: 1 = up, 2 = right, 3 = down, 4 = left
        int direction = distribution(engine);

        if (direction == 1) {
            if (r == 0) {
                continue;
            }
            if (checked[r - 1][c]) {
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
            return Direction::right;
        }

        if (direction == 3) {
            if (r == max_row) {
                continue;
            }
            if (checked[r + 1][c]) {
                continue;
            }
            return Direction::down;
        }

        if (direction == 4) {
            if (c == 0) {
                continue;
            }
            if (checked[r][c - 1]) {
                continue;
            }
            return Direction::left;
        }
    }
}

bool CCMazeGen::checkArea(Maze& maze, std::vector<std::vector<bool>>& checked, int r, int c, bool debug) { 
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