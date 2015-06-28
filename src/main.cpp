#include <iostream>
#include <algorithm>

#include <core/RandomGenerator.h>
#include <core/ComputePoints.h>

#define size 10000

int main(int argc, char const *argv[])
{
  msc::Item* array = new msc::Item[size];

  msc::RandomGenerator random;
  for(int i = 0; i < size; ++i)
    array[i].sort_id = static_cast<int>(random.getSample() * 100);

  msc::computePoints(array, size);

  delete[] array;

  return 0;
}