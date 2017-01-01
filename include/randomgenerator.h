#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>
#include <algorithm>
#include <iterator>
#include <vector>


class RandomGenerator
{
private:
    std::random_device rd;
    std::mt19937 g;
    static RandomGenerator *_instance;
private:
    RandomGenerator();
public:
    static RandomGenerator *Instance();
    std::vector<int> getShuffleList(int size);

    ~RandomGenerator();
};

#endif // RANDOMGENERATOR_H
