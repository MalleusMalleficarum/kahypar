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
    void mutateImpl(Individuum &target) override final {
      Partitioner partitioner;
      std::vector<PartitionID> vect = target.getPartition();
      partitioner.mutateHypergraphPartition(_hypergraph, _config, vect);
    };

  };


}
