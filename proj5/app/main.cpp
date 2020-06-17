// main.cpp
//
// ICS 46 Spring 2020
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
#include "InputReader.hpp"
#include "RoadMapReader.hpp"
#include "RoadMapWriter.hpp"
#include "TripReader.hpp"

#include <iostream>
#include <cmath>
#include <sstream>

std::stringstream time(double timeDouble) {
    std::stringstream toReturn;

    int timeHours = (timeDouble / 24);
    int timeMinutes = std::fmod(timeDouble, 60);
    double timeSeconds = std::round(10 * (std::fmod(timeDouble, 60) - timeMinutes) * 60) / 10;
    if (timeHours != 0) {toReturn << timeHours << " hours ";}
    if (timeMinutes != 0) {toReturn << timeMinutes << " minutes ";}
    if (timeSeconds != 0) {toReturn << timeSeconds << " seconds ";}

    return toReturn;
}

int main()
{
    //Input reader for readLine() and readIntLine()
    InputReader input = InputReader{std::cin};

    //Reader for roadmap from std::cin
    RoadMapReader roadMapReader = RoadMapReader();
    RoadMap roadMap = roadMapReader.readRoadMap(input);

    //RoadMap Writer for Testing
//    RoadMapWriter roadMapWriter = RoadMapWriter();
//    roadMapWriter.writeRoadMap(std::cout, roadMap);

    //Reader fro trips from std::cin
    TripReader tripReader = TripReader();
    std::vector<Trip> trips = tripReader.readTrips(input);

    std::map<int, int> shortestPaths;
    int previousStartVertex = -1;
    int previousMetric = -1; //0 is distance, 1 is time
    for (Trip t : trips) {
        if (t.metric == TripMetric::Distance) {
            std::cout << "Shortest distance from " << roadMap.vertexInfo(t.startVertex) << " to " << roadMap.vertexInfo(t.endVertex) << std::endl;
            std::cout << "Begin at " << roadMap.vertexInfo(t.startVertex) << std::endl;

            if (previousStartVertex != -1 && previousStartVertex == t.startVertex && previousMetric == 0) {
                previousStartVertex = t.startVertex;

                std::vector<std::pair<int, int>> shortestPath;
                int currentIndex = t.endVertex;
                while (currentIndex != t.startVertex) {
                    shortestPath.insert(shortestPath.begin(), std::pair(shortestPaths[currentIndex], currentIndex));
                    currentIndex = shortestPaths[currentIndex];
                }

                double totalMiles = 0;
                for (std::pair<int, int> pathStep : shortestPath) {
                    std::cout << "  Continue to " << roadMap.vertexInfo(pathStep.second) << " (" << roadMap.edgeInfo(pathStep.first, pathStep.second).miles << " miles)" <<std::endl;
                    totalMiles += roadMap.edgeInfo(pathStep.first, pathStep.second).miles;
                }

                std::cout << "Total distance: " << totalMiles << " miles\n" << std::endl;

            } else {
                shortestPaths = roadMap.findShortestPaths(t.startVertex, [](RoadSegment segment){return segment.miles;});
                previousStartVertex = t.startVertex;
                previousMetric = 0;

                std::vector<std::pair<int, int>> shortestPath;
                int currentIndex = t.endVertex;
                while (currentIndex != t.startVertex) {
                    shortestPath.insert(shortestPath.begin(), std::pair(shortestPaths[currentIndex], currentIndex));
                    currentIndex = shortestPaths[currentIndex];
                }

                double totalMiles = 0;
                for (std::pair<int, int> pathStep : shortestPath) {
                    std::cout << "  Continue to " << roadMap.vertexInfo(pathStep.second) << " (" << roadMap.edgeInfo(pathStep.first, pathStep.second).miles << " miles)" <<std::endl;
                    totalMiles += roadMap.edgeInfo(pathStep.first, pathStep.second).miles;
                }

                std::cout << "Total distance: " << totalMiles << " miles\n" << std::endl;
            }
        } else {
            std::cout << "Shortest driving time from " << roadMap.vertexInfo(t.startVertex) << " to " << roadMap.vertexInfo(t.endVertex) << std::endl;
            std::cout << "Begin at " << roadMap.vertexInfo(t.startVertex) << std::endl;

            if (previousStartVertex != -1 && previousStartVertex == t.startVertex && previousMetric == 1) {
                previousStartVertex = t.startVertex;

                std::vector<std::pair<int, int>> shortestPath;
                int currentIndex = t.endVertex;
                while (currentIndex != t.startVertex) {
                    shortestPath.insert(shortestPath.begin(), std::pair(shortestPaths[currentIndex], currentIndex));
                    currentIndex = shortestPaths[currentIndex];
                }

                double totalTime = 0;
                for (std::pair<int, int> pathStep : shortestPath) {
                    std::cout << "  Continue to " << roadMap.vertexInfo(pathStep.second) << " ( "
                              << roadMap.edgeInfo(pathStep.first, pathStep.second).miles << " miles @ "
                              << roadMap.edgeInfo(pathStep.first, pathStep.second).milesPerHour
                              << "mph = ";
                    double stepTime = 60 * roadMap.edgeInfo(pathStep.first, pathStep.second).miles / roadMap.edgeInfo(pathStep.first, pathStep.second).milesPerHour;
                    std::cout << time(stepTime).str() << ")" << std::endl;
                    totalTime += stepTime;
                }

                std::cout << "Total time: ";
                std::cout << time(totalTime).str() << "\n" << std::endl;
            } else {
                shortestPaths = roadMap.findShortestPaths(t.startVertex, [](RoadSegment segment){return 60 * segment.miles / segment.milesPerHour;});
                previousStartVertex = t.startVertex;
                previousMetric = 1;

                std::vector<std::pair<int, int>> shortestPath;
                int currentIndex = t.endVertex;
                while (currentIndex != t.startVertex) {
                    shortestPath.insert(shortestPath.begin(), std::pair(shortestPaths[currentIndex], currentIndex));
                    currentIndex = shortestPaths[currentIndex];
                }

                double totalTime = 0;
                for (std::pair<int, int> pathStep : shortestPath) {
                    std::cout << "  Continue to " << roadMap.vertexInfo(pathStep.second) << " ( "
                              << roadMap.edgeInfo(pathStep.first, pathStep.second).miles << " miles @ "
                              << roadMap.edgeInfo(pathStep.first, pathStep.second).milesPerHour
                              << "mph = ";
                    double stepTime = 60 * roadMap.edgeInfo(pathStep.first, pathStep.second).miles / roadMap.edgeInfo(pathStep.first, pathStep.second).milesPerHour;
                    std::cout << time(stepTime).str() << ")" << std::endl;
                    totalTime += stepTime;
                }

                std::cout << "Total time: ";
                std::cout << time(totalTime).str() << "\n" << std::endl;
            }
        }
    }

    return 0;
}

