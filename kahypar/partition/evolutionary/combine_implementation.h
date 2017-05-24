#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
#include "population.h"

using kahypar::Partitioner;
namespace kahypar {

// TODO(robin): maintain less state:
// [ ] use a evo::combine namespace to encapsulate free functions
// [ ] make one function for combine
// [ ] make one function for cross combine

  class CombinatorBaseImplementation final : public ICombine{
  public:
    CombinatorBaseImplementation(Hypergraph &hypergraph, Configuration &config) :
    _hypergraph(hypergraph),
    _config(config) {}
	
  private:
    Individuum combineImpl(const Individuum &parent_1, const Individuum &parent_2, bool crossComb) override final {
      bool temp = crossComb;
      Partitioner partitioner;
      EvoParameters evo(parent_1.getPartition(), parent_2.getPartition(), false, false, std::vector<double>(), crossComb);
      _hypergraph.reset();
      partitioner.partition(_hypergraph, _config, evo);
      return Population::createIndividuum(_hypergraph, _config);
    }


    Hypergraph &_hypergraph;
    Configuration &_config;
  };

}
