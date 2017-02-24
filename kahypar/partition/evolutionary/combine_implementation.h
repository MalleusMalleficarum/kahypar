#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
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
      partitioner.partition(_hypergraph, _config, parent_1.getPartition(), parent_2.getPartition());
      std::vector<PartitionID> result;
      for (HypernodeID u : _hypergraph.nodes()) {	
	result.push_back(_hypergraph.partID(u));
	}
	  return Individuum(result, metrics::km1(_hypergraph));
    }


    Hypergraph &_hypergraph;
    Configuration &_config;
  };

}
