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
#include "population.h"

using kahypar::Partitioner;
namespace kahypar {
// TODO(robin): maintain less state:
// [ ] use a evo::combine namespace to encapsulate free functions
// [ ] make one function for combine
// [ ] make one function for cross combine

class CombinatorBaseImplementation final : public ICombine {
 public:
  CombinatorBaseImplementation(Hypergraph& hypergraph, Configuration& config) :
    _hypergraph(hypergraph),
    _config(config) { }

 private:
  Individuum combineImpl(const Individuum& parent_1, const Individuum& parent_2, bool crossComb) override final {
    bool temp = crossComb;
    Partitioner partitioner;
    EvoParameters evo(parent_1.getPartition(), parent_2.getPartition(), false, false, std::vector<double>(), crossComb);
    _hypergraph.reset();
    partitioner.partition(_hypergraph, _config, evo);
    return Population::createIndividuum(_hypergraph, _config);
  }


  Hypergraph& _hypergraph;
  Configuration& _config;
};
}
