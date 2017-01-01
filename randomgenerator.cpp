#include "randomgenerator.h"

RandomGenerator* RandomGenerator::_instance = nullptr;

RandomGenerator::RandomGenerator()
{
    g = std::mt19937(rd());
}

RandomGenerator::~RandomGenerator()
{
    delete _instance;
}

RandomGenerator *RandomGenerator::Instance()
{
    if(_instance == nullptr)
    {
        _instance = new RandomGenerator();
    }
    return _instance;
}

std::vector<int> RandomGenerator::getShuffleList(int size)
{

    std::vector<int> indices;
    if(size > 0)
    {
        for(int i =0; i < size; i++)
        {
            indices.push_back(i);
        }
        std::shuffle(indices.begin(),indices.end(),g);
    }

    return indices;
}
