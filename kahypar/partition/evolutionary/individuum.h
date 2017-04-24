#pragma once

#include <vector>
#include "kahypar/definitions.h"
#include "kahypar/datastructure/sparse_map.h"
namespace kahypar{
  class Individuum {
  public:
  Individuum(std::vector<PartitionID> partition,std::vector<HyperedgeID> cutEdges, std::vector<HyperedgeID> strongEdges ,HyperedgeWeight fitness):
    _partition(partition),
      _cutEdges(cutEdges),
      _cutEdgesStrong(strongEdges),
      _fitness(fitness) {}
    inline HyperedgeWeight getFitness()const {
      return _fitness;
    }

    inline std::vector<PartitionID> getPartition() const{
      return _partition;
    }
    inline std::vector<HyperedgeID> getCutEdges() {
      return _cutEdges;
    }

    inline std::vector<HyperedgeID> getStrongCutEdges() {
      return _cutEdgesStrong;
    }
    inline void print() {
      std::cout << std::endl << std::endl << "Individuum Info : ----------------------" << std::endl << std::endl;
      std::cout << "Fitness: " << _fitness << std::endl;
      std::cout << "----------------------------------------" << std::endl;
      for (auto i = _partition.begin(); i != _partition.end(); ++i)
        std::cout << *i << ' ';

      std::cout << std::endl << "Cut Edges ----------------------------------------" << std::endl;
      for (auto i = _cutEdges.begin(); i != _cutEdges.end(); ++i)
        std::cout << *i << ' ';

      std::cout << std::endl << "Strong Cut Edges ----------------------------------------" << std::endl;
      for (auto i = _cutEdgesStrong.begin(); i != _cutEdgesStrong.end(); ++i)
        std::cout << *i << ' ';

      std::cout << std::endl << "----------------------------------------" << std::endl;
    }
    inline void printSparse() {
            std::cout << std::endl << std::endl << "Individuum Info : ----------------------" << std::endl << std::endl;
      std::cout << "Fitness: " << _fitness << std::endl;
std::cout << std::endl << "Cut Edges ----------------------------------------" << std::endl;
      for (auto i = _cutEdges.begin(); i != _cutEdges.end(); ++i)
        std::cout << *i << ' ';
std::cout << std::endl << "----------------------------------------" << std::endl;

    }

  private:   
    std::vector<PartitionID> _partition;
    std::vector<HyperedgeID> _cutEdges;
    std::vector<HyperedgeID> _cutEdgesStrong;
    HyperedgeWeight _fitness;
  };


}
