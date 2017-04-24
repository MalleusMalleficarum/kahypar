#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
#include "population.h"

using kahypar::Partitioner;
namespace kahypar {
  class CombinatorBaseImplementation final : public ICombine{
  public:
    CombinatorBaseImplementation(Hypergraph &hypergraph, Configuration &config) :
    _hypergraph(hypergraph),
    _config(config) {}
	
  private:
    Individuum combineImpl(const Individuum &parent_1, const Individuum &parent_2) override final {
      Partitioner partitioner;
      EvoParameters evo(parent_1.getPartition(), parent_2.getPartition(), false, false, std::vector<double>());
      partitioner.partition(_hypergraph, _config, evo);
      return Population::createIndividuum(_hypergraph, _config);
    }


    Hypergraph &_hypergraph;
    Configuration &_config;
  };

}
