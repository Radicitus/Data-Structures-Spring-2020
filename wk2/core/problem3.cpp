
#include <vector>
#include <random>
#include <iostream>

std::vector<unsigned int> countRandomFrequencies(const std::vector<unsigned int> weights, unsigned int count) {
    std::random_device device;
    std::default_random_engine engine{device()};

    std::discrete_distribution<> distribution(weights.begin(), weights.end());

    std::vector<unsigned int> to_return(weights.size(), 0);
    for (int i = 0; i < count; ++i) {
        to_return[distribution(engine)]++;
    }

    return to_return;
}