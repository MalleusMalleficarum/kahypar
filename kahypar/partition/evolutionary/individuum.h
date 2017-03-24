#pragma once

#include <vector>
#include "kahypar/definitions.h"
namespace kahypar{
  class Individuum {
  public:
  Individuum(std::vector<PartitionID> &partition, std::vector<HyperedgeID> &cutEdges, HyperedgeWeight fitness):
    _partition(partition),
      _cutEdges(cutEdges),
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
    inline std::vector<HyperedgeID> getCutEdges() {
      return _cutEdges;
    }

  private:   
    std::vector<PartitionID> _partition;
    std::vector<HyperedgeID> _cutEdges;
    HyperedgeWeight _fitness;
  };


}
