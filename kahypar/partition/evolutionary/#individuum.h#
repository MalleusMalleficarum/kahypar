#pragma once

#include <vector>
#include "kahypar/definitions.h"
namespace kahypar{
  class Individuum {
  public:
  Individuum(std::vector<PartitionID> &partition, double fitness):
    _partition(partition),
      _fitness(fitness) {}
    inline double getFitness() {
      return _fitness;
    }
    /* ~Individuum() {

       }*/
  private:   
    const std::vector<PartitionID> _partition;
    const double _fitness;
  };
  /*Individuum(std::vector<int> &partition, double fitness){
_partition = partition;
_fitness = fitness;
}*/

}
