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
namespace kahypar {
// TODO(robin): maintain less state:
// [ ] use a evo::mutate namespace to encapsulate free functions

class MutatorBaseImplementation final : public IMutate {
 public:
  MutatorBaseImplementation(Hypergraph& hypergraph, Configuration& config) :
    _config(config),
    _hypergraph(hypergraph) { }

 private:
  Configuration& _config;
  Hypergraph& _hypergraph;
  Individuum mutateImpl(Individuum& target) override final {
    Partitioner partitioner;
    std::vector<PartitionID> vect = target.getPartition();
    partitioner.mutateHypergraphPartition(_hypergraph, _config, vect);
    return Population::createIndividuum(_hypergraph, _config);
  }
};
}
