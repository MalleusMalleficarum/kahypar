#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
namespace kahypar {
  class MutatorBaseImplementation final : public IMutate{
  public:
	MutatorBaseImplementation(Hypergraph & hypergraph, Configuration &config) :
	_config(config),
        _hypergraph(hypergraph) {}
  private:
    Configuration &_config;
    Hypergraph &_hypergraph;
    Individuum mutateImpl(Individuum &target) override final {
      Partitioner partitioner;
      std::vector<PartitionID> vect = target.getPartition();
      partitioner.mutateHypergraphPartition(_hypergraph, _config, vect);
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
    };

  };


}
