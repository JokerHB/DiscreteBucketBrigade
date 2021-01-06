//
//  RandomGenerator.cpp
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2021/1/5.
//

#ifndef RandomGenerator_cpp
#define RandomGenerator_cpp

#include "../../Include/Tool/RandomGenerator.h"

RandomGenerator::RandomGenerator(double mean, double stddev, double min, double max)
{
    this->seed = (int)std::chrono::system_clock::now().time_since_epoch().count();
    this->generator = std::default_random_engine(seed);
    this->distribution = std::normal_distribution<double>(mean, stddev);
    this->min = min;
    this->max = max;
}

double RandomGenerator::operator()()
{
    while (true)
    {
        double number = this->distribution(generator);
        if (number >= this->min && number <= this->max)
            return number;
    }
}

int RandomGenerator::GenerateRandomInt(int left, int right)
{
    srand((unsigned)time(NULL));

    return (rand() % (right - left + 1)) + left;
}

#endif /* RandomGenerator_cpp */
