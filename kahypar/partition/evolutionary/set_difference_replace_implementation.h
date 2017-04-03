#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
namespace kahypar {
  class SetDifferenceReplace final : public IReplace {
  public:
	//MutatorBaseImplementation(Hypergraph & hypergraph, Configuration &config) :
	//_config(config),
        //_hypergraph(hypergraph) {}
  private:

    ReplaceInformation replaceImpl(const Individuum &in,std::vector<Individuum> &population) override final {
      ReplaceInformation ret;
      ret.willReplace = false;
      unsigned max_similarity = std::numeric_limits<unsigned>::max();
      unsigned max_similarity_id = 0;
      for(unsigned i = 0; i < population.size(), i++) {
        if(population[i].getFitness() >= in.getFitness()) {
          ret.willReplace = true;
          std::vector<HyperedgeID> output_diff;
          std::vector<HyperedgeID> one(population[i].getCutEdges());
          std::vector<HyperedgeID> two(in.getCutEdges());
          std::set_symmetric_difference(one.begin(),
				    one.end(),
				    two.begin(),
				    two.end(),
				    std::back_inserter(output_diff));
          unsigned similarity = output_diff.size();
          std::cout << "Symmetric Difference: " << similarity << " from " << i << std::endl;
          if(similarity < max_similarity) {
            max_similarity = similarity;
            max_similarity_id = i;
          }
        }
      
      }
      ret.position = max_similarity_id;
      return ret;
  };


}
