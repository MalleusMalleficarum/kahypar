#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
namespace kahypar {
  class CombinatorBaseImplementation final : public ICombine{
  public:
    
  private:
    Individuum combineImpl(const Individuum &parent_1, const Individuum &parent_2) override final {
      Partitioner.partition(hypergraph, config, parent_1, parent_2);
    }


    &Hypergraph hypergraph;
    &Configuration config;
  };

}
