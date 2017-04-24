#pragma once
#include "kahypar/definitions.h"
namespace kahypar {
 struct EvoParameters {
 EvoParameters(const std::vector<PartitionID> parent1, const std::vector<PartitionID> parent2, const std::vector<double> frequency) : parent_1(parent1), parent_2(parent2), mutate(false), edgeFrequency(false), frequency(frequency) {}
  EvoParameters(const std::vector<PartitionID> parent1,const std::vector<PartitionID> parent2, bool mutate, bool edgeFrequency,const std::vector<double> frequency) : parent_1(parent1), parent_2(parent2), mutate(mutate), edgeFrequency(edgeFrequency), frequency(frequency) {}



   void print() {
     std::cout << "parent1length " << parent_1.size() << ";parent2length " << parent_2.size() << ";mutate " << mutate << ";edgeFreq " << edgeFrequency << ";freqlenght " << frequency.size();
   }
   
    const std::vector<PartitionID> parent_1;
    const std::vector<PartitionID> parent_2;
    bool mutate;
    bool edgeFrequency;
    const std::vector<double> frequency;
};
}
