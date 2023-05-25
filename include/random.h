#ifndef RANDOM_H
#define RANDOM_H

#include <vector3.h>

#include <chrono>
#include <random>

using namespace std::chrono;
using mpcs51045::Vector3;
using std::mt19937;
using std::normal_distribution;
using std::uniform_real_distribution;

mt19937 getRNG() {
    return mt19937(steady_clock::now().time_since_epoch().count());
}

// Normal distribution will generate uniformly random distribution of points(?)
normal_distribution<double> getNormalDistribution(double mean, double std) {
    return normal_distribution<double>(mean, std);
}

uniform_real_distribution<double> getUniformRealDistribution(double low, double high) {
    return uniform_real_distribution(low, high);
}

double getRandomProbValue() {
    mt19937 rng(getRNG());
    auto dist = getUniformRealDistribution(0.0, 1.0);
    return dist(rng);
}

// Generates a random direction vector
Vector3<double> randomDirection() {
    mt19937 rng(getRNG());
    auto distribution(getNormalDistribution(0.0, 1.0));

    double x = distribution(rng);
    double y = distribution(rng);
    double z = distribution(rng);
    Vector3<double> point{x, y, z};
    point.normalize();
    return point;
}

// Takes in a normal vector and a random direction vector; returns the random
// vector if it is <90 deg from the normal, else returns the inverted random
// vector
Vector3<double> handleInvert(Vector3<double> normal, Vector3<double> random) {
    if (Vector3<double>::dot(normal, random) < 0) {
        return random * -1.0;
    } else {
        return random;
    }
}

// Generates a random direction vector that is <=90 degrees from the normal
Vector3<double> randomReboundDirection(Vector3<double> normal) {
    return handleInvert(normal, randomDirection());
}

#endif