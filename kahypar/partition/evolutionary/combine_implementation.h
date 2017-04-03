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
	std::vector<HyperedgeID> cutEdges;
	for(HyperedgeID v : _hypergraph.edges()) {
	  
	  if(_hypergraph.connectivity(v) > 1) {
          //std::cout << _hypergraph.connectivity(v) << ' ' << v << ';';	
            for(unsigned i = 1; i < _hypergraph.connectivity(v); i++) {
	      cutEdges.push_back(v);
            }	
	  }
	}
	  return Individuum(result,cutEdges, metrics::km1(_hypergraph));
    }


    Hypergraph &_hypergraph;
    Configuration &_config;
  };

}
