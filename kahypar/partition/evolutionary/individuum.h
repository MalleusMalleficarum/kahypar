#pragma once

#include <vector>
#include "kahypar/definitions.h"
namespace kahypar{
  class Individuum {
  public:
  Individuum(std::vector<PartitionID> &partition, HyperedgeWeight fitness):
    _partition(partition),
      _fitness(fitness) {}
    inline HyperedgeWeight getFitness()const {
      return _fitness;
    }
    /* inline void setFitness(HyperedgeWeight fitness) { */
    /*   _fitness = fitness; */
    /* } */
    inline std::vector<PartitionID> getPartition() const{
      return _partition;
    }

  private:   
    std::vector<PartitionID> _partition;
    HyperedgeWeight _fitness;
  };


}
