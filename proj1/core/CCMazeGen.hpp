#ifndef CCMAZEGEN_HPP
#define CCMAZEGEN_HPP

#include "MazeGenerator.hpp"
#include <random>
#include "Maze.hpp"

class CCMazeGen : public MazeGenerator {
    public:
        void generateMaze(Maze& maze) override;
    private:
        void CCMGRecursive(Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c);
        Direction getValidDirection(Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c);
        bool checkArea(Maze& maze, std::vector<std::vector<bool>>& checked, int r, int c, bool debug);
};


#endif
