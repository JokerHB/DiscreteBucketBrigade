//
//  RandomGenerator.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2021/1/5.
//

#ifndef RandomGenerator_h
#define RandomGenerator_h

#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

class RandomGenerator
{
    unsigned seed;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
    double min;
    double max;

public:
    RandomGenerator(double mean, double stddev, double min, double max);

    double operator()();

    static int GenerateRandomInt(int left, int right);
};

#endif /* RandomGenerator_h */
