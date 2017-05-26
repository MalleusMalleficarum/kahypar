/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2016 Sebastian Schlag <sebastian.schlag@kit.edu>
 *
 * KaHyPar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KaHyPar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KaHyPar.  If not, see <http://www.gnu.org/licenses/>.
 *
******************************************************************************/
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
  ReplaceInformation replaceImpl(const Individuum& in, std::vector<Individuum>& population) override final {
    ReplaceInformation ret;
    ret.willReplace = false;
    unsigned max_similarity = std::numeric_limits<unsigned>::max();
    unsigned max_similarity_id = 0;
    for (unsigned i = 0; i < population.size(), i++) {
      if (population[i].getFitness() >= in.getFitness()) {
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
        if (similarity < max_similarity) {
          max_similarity = similarity;
          max_similarity_id = i;
        }
      }
    }
    ret.position = max_similarity_id;
    return ret;
  }
}
