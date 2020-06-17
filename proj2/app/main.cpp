// main.cpp
//
// ICS 46 Spring 2020
// Project #2: Time Waits for No One
//
// This is the entry point for your simulation application.
//
// All of the code that makes up your simulation -- which is everything
// except for your DoublyLinkedList and Queue implementations -- should
// be in the "app" directory, though, naturally, it shouldn't all be in
// this file.  A design that keeps separate things separate is always
// part of the requirements.

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include "Queue.hpp"

//Customer
struct Customer {
    int timeEnteredLine;
};

//Returns an index of a line where maximum space is available, otherwise returns -1
int isEmptyLine(std::vector<Queue<Customer>>& lines, int maxLineLength) {
    int minCustomers = INT_MAX;
    int index = -1;
    for (int i = 0; i < lines.size(); ++i) {
        int customersInLine = lines.at(i).size();
        if (customersInLine != maxLineLength && customersInLine < minCustomers) {
            minCustomers = customersInLine;
            index = i;
        }
    }
    return index;
}

//Updates register and associated line for time in between customers entering lines
void updateTimeBetweenCustomers(std::vector<std::pair<bool, int>>& registerAvailability, std::vector<Queue<Customer>>& lineStorage,
                                std::vector<int>& registerWaitTimes, int& time, int& customerTime,
                                int& customersExitedLine, int& customersExitedRegister,
                                int& totalTimeWaited, const char* exitedLineString, const char* exitedRegisterString,
                                const char* enteredRegisterString) {
    //Priority queue lambda compare function (first is register index, second is time remaining)
    auto compare = [](std::pair<int, int> left, std::pair<int, int> right) {
        if (left.second < right.second) {
            return false;
        } else if (left.second > right.second) {
            return true;
        } else {
            return left.first >= right.first;
        }
    };


    //Priority queue for making sure older times are logged first
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(compare)> priorityQueue(compare);

    for (int i = 0; i < registerAvailability.size(); ++i) {
        //Check if anyone at register
        if (!registerAvailability.at(i).first) {
            continue;
        }

        //Calculate time left for customer at register
        int timeRemaining = (registerAvailability.at(i).second - customerTime) + registerWaitTimes.at(i);

        //Check if customer is about to leave register as new customer comes in
        if (timeRemaining == 0) {
            registerAvailability.at(i).second = 0;
            continue;
        }

        priorityQueue.push(std::make_pair(i, timeRemaining));

    }

    while (!priorityQueue.empty()) {

        int i = priorityQueue.top().first;
        int timeRemaining = priorityQueue.top().second;
        priorityQueue.pop();
        
        //Update registers and lines while still in negative time
        while (timeRemaining < 0) {

            //Log customer leaving register
            printf(exitedRegisterString, timeRemaining + customerTime, i + 1);
            customersExitedRegister++;

            //Remove customer from register
            registerAvailability.at(i).first = false;
            registerAvailability.at(i).second = 0;

            //Check if line is empty (if it is and timeRemaining is still negative then set to false)
            if (lineStorage.at(i).isEmpty() && timeRemaining < 0) {
                break;
            } else {

                //Store customer info and remove from line
                int timeEnteredRegister = timeRemaining + customerTime;
                int timeEnteredLine = lineStorage.at(i).front().timeEnteredLine;
                lineStorage.at(i).dequeue();

                //Log customer exiting the line
                printf(exitedLineString, timeEnteredRegister, i + 1, lineStorage.at(i).size(), timeEnteredRegister - timeEnteredLine);
                customersExitedLine++;

                //Add customer to register
                registerAvailability.at(i).first = true;
                registerAvailability.at(i).second = timeEnteredRegister;

                //Log customer entering register
                totalTimeWaited += timeEnteredRegister - timeEnteredLine;
                printf(enteredRegisterString, timeEnteredRegister, i + 1);
                timeRemaining = (registerAvailability.at(i).second - customerTime) + registerWaitTimes.at(i);
            }
        }
    }
}

int main()
{
    // =================================================================================================================
    //VARIABLES
    //InputVars
    int simLength;
    int numRegisters;
    int maxLineLength;
    bool isSingleLine = false;

    //SimVars
    int customersEnteredLine = 0;
    int customersLost = 0;

    int customersExitedLine = 0;
    int customersExitedRegister = 0;

    int customersRemainingLine = 0;
    int customersRemainingRegister = 0;

    int time = 0;
    int totalTimeWaited = 0;
    // =================================================================================================================
    //SIMULATION REPRESENTATIONS
    std::vector<Queue<Customer>> lineStorage; //Stores the queues that represent the lines
    std::vector<int> registerWaitTimes; //Stores the register wait times
    std::vector<std::pair<bool, int>> registerAvailablity; //Stores if someone at register and the times at which customers entered the registers
    // =================================================================================================================
    //STRING TEMPLATES
    const char *enteredLine = "%d entered line %d length %d\n"; //Time, Line #, Length
    const char *exitedLine = "%d exited line %d length %d wait time %d\n"; //Time, Line #, Length, Wait Time
    const char *enteredRegister = "%d entered register %d\n"; //Time, Register #
    const char *exitedRegister = "%d exited register %d\n"; //Time, Register #
    const char *lostCustomer = "%d lost\n"; //Time

    const char *stats = "STATS\n"
                        "Entered Line    : %d\n" //# of customers to enter lines
                        "Exited Line     : %d\n" //# of customers to exit lines
                        "Exited Register : %d\n" //# of customers to exit registers
                        "Avg Wait Time   : %.2f\n" //Avg wait time across all customers
                        "Left In Line    : %d\n" //# of customers still waiting in lines
                        "Left In Register: %d\n" //# of customers still at registers
                        "Lost            : %d\n" //# of customers lost
                        ;
    // =================================================================================================================
    //SIMULATION

    std::cin >> simLength >> numRegisters >> maxLineLength;
    simLength = simLength * 60; //Set sim length to seconds instead of minutes
    std::string tempSingleLine;
    std::cin >> tempSingleLine;
    if (tempSingleLine == "S") {isSingleLine = true;}

    //Add wait times to registerWaitTimes
    for (int i = 0; i < numRegisters; ++i) {
        int tempWaitTime;
        std::cin >> tempWaitTime;
        registerWaitTimes.push_back(tempWaitTime);
        registerAvailablity.emplace_back(false, 0);
    }

    //Create queues for lines
    if (isSingleLine) {
        lineStorage.emplace_back();
    } else {
        for (int i = 0; i < numRegisters; ++i) {
            lineStorage.emplace_back();
        }
    }

    //Simulation loop
    std::cout << "LOG\n0 start" << std::endl;

    int customerAmount;
    int customerTime;
    while (std::cin >> customerAmount) {
        std::cin >> customerTime;

        //Update all registers and time for the time in between last customers and new customers
        updateTimeBetweenCustomers(registerAvailablity, lineStorage, registerWaitTimes, time, customerTime,
                                customersExitedLine, customersExitedRegister, totalTimeWaited, exitedLine,
                                exitedRegister, enteredRegister);

        time = customerTime;

        //Enter customers into line
        for (int i = 0; i < customerAmount; ++i) {

            //Check if all lines are full
            int index = isEmptyLine(lineStorage, maxLineLength);
            if (index < 0) {
                customersLost++;
                printf(lostCustomer, time);
            } else {
                lineStorage.at(index).enqueue(Customer{time});
                customersEnteredLine++;
                printf(enteredLine, time, index + 1, lineStorage.at(index).size());
            }
        }

        //Enter customers into register
        for (int i = 0; i < lineStorage.size(); ++i) {

            //Check if someone is at the register
            if (!registerAvailablity.at(i).first && !lineStorage.at(i).isEmpty()) { //No one is at the register and no one in line

                //Add customer in line to register
                registerAvailablity.at(i).first = true;
                registerAvailablity.at(i).second = time;

                //Calculate timeWaited
                int timeWaited = time - lineStorage.at(i).front().timeEnteredLine;

                //Remove customer from line
                lineStorage.at(i).dequeue();

                //Log customer leaving line
                printf(exitedLine, time, i + 1, lineStorage.at(i).size(), timeWaited);
                customersExitedLine++;
                totalTimeWaited += timeWaited;

                //Log customer entering register
                printf(enteredRegister, time, i + 1);
            } else { //Someone is at the register

                //Check if customer at register was about to leave
                if (registerAvailablity.at(i).first && registerAvailablity.at(i).second == 0) {

                    //Log customer leaving register
                    printf(exitedRegister, time, i + 1);
                    customersExitedRegister++;

                    //Remove customer from register
                    registerAvailablity.at(i).first = false;
                    registerAvailablity.at(i).second = 0;

                    //Check if there are any customers in line
                    if (!lineStorage.at(i).isEmpty()) {

                        //Add customer in line to register
                        registerAvailablity.at(i).first = true;
                        registerAvailablity.at(i).second = time;

                        //Calculate timeWaited
                        int timeWaited = time - lineStorage.at(i).front().timeEnteredLine;

                        //Remove customer from line
                        lineStorage.at(i).dequeue();

                        //Log customer leaving line
                        printf(exitedLine, time, i + 1, lineStorage.at(i).size(), timeWaited);
                        customersExitedLine++;
                        totalTimeWaited += timeWaited;

                        //Log customer entering register
                        printf(enteredRegister, time, i + 1);
                    }
                }
            }
        }
    }

    //Cleanup (deal with remaining customers up until simLength is reached)
    //Update all registers and time for the time in between last customers and end of simulation
    updateTimeBetweenCustomers(registerAvailablity, lineStorage, registerWaitTimes, time, simLength,
                               customersExitedLine, customersExitedRegister, totalTimeWaited, exitedLine,
                               exitedRegister, enteredRegister);

    std::cout << simLength << " end\n" << std::endl;

    //STATS
    float avgWaitTime;

    avgWaitTime = (float)totalTimeWaited / (float)customersExitedLine;
    for (int i = 0; i < registerAvailablity.size(); ++i) {
        if (registerAvailablity.at(i).first) {
            customersRemainingRegister++;
        }
        customersRemainingLine += lineStorage.at(i).size();
    }

    printf(stats, customersEnteredLine, customersExitedLine, customersExitedRegister, avgWaitTime,
            customersRemainingLine, customersRemainingRegister, customersLost);


    return 0;
}

