#pragma once

#include <vector>
#include "kahypar/definitions.h"
namespace kahypar{
  class Individuum {
  public:
  Individuum(std::vector<PartitionID> &partition, double fitness):
    _partition(partition),
      _fitness(fitness) {}
    inline double getFitness()const {
      return _fitness;
    }
  Individuum(const Individuum &obj) :
    _partition(obj._partition),
      _fitness(obj._fitness) {}
    /* ~Individuum() {

       }*/
    /* Individuum(const Individuum &obj) { */
    /*   _partition = obj._partition; */
    /*   _fitness=obj._fitness; */
    /* } */
  private:   
    std::vector<PartitionID> _partition;
    double _fitness;
  };
  /*Individuum(std::vector<int> &partition, double fitness){
_partition = partition;
_fitness = fitness;
}*/

}
