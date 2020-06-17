#ifndef CCMAZEGEN_HPP
#define CCMAZEGEN_HPP

#include "MazeSolver.hpp"
#include "MazeSolution.hpp"
#include "Maze.hpp"
#include <random>

class CCMazeSolve : public MazeSolver {
    public:
        void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
    private:
        void CCMSRecursive(const Maze& maze, MazeSolution& mazeSolution, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c);
        Direction getValidDirection(const Maze& maze, std::default_random_engine& engine, std::vector<std::vector<bool>>& checked, int r, int c);
        bool checkArea(const Maze& maze, std::vector<std::vector<bool>>& checked, int r, int c, bool debug);
};

#endif