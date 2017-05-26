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

#include "kahypar/datastructure/sparse_map.h"
#include "kahypar/definitions.h"
#include <vector>
namespace kahypar {
class Individuum {
 public:
  // TODO(robin): use move semantics whenever possible (also cut edges etc.)
  // if not possible use const references
  Individuum(std::vector<PartitionID>&& partition, std::vector<HyperedgeID> cutEdges, std::vector<HyperedgeID> strongEdges, HyperedgeWeight fitness) :
      _partition(std::move(partition)),
    _cutEdges(cutEdges),
    _cutEdgesStrong(strongEdges),
    _fitness(fitness) { }

  // TODO(robin): don't use get in method names: fitness()
  inline HyperedgeWeight getFitness() const {
    return _fitness;
  }

  inline std::vector<PartitionID> getPartition() const {
    return _partition;
  }

  inline std::vector<HyperedgeID> getCutEdges() {
    return _cutEdges;
  }

  // TODO(robin): don't copy vectors if not necessary -- use const reference const std::vector<HyperedgeID>&
  // and make method const
  inline const std::vector<HyperedgeID>& getStrongCutEdges() const {
    return _cutEdgesStrong;
  }
  inline void print() {
    std::cout << std::endl << std::endl << "Individuum Info : ----------------------" << std::endl << std::endl;
    std::cout << "Fitness: " << _fitness << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (auto i = _partition.begin(); i != _partition.end(); ++i)
      std::cout << *i << ' ';

    std::cout << std::endl << "Cut Edges ----------------------------------------" << std::endl;
    for (auto i = _cutEdges.begin(); i != _cutEdges.end(); ++i)
      std::cout << *i << ' ';

    std::cout << std::endl << "Strong Cut Edges ----------------------------------------" << std::endl;
    for (auto i = _cutEdgesStrong.begin(); i != _cutEdgesStrong.end(); ++i)
      std::cout << *i << ' ';

    std::cout << std::endl << "----------------------------------------" << std::endl;
  }
  inline void printSparse() {
    std::cout << std::endl << std::endl << "Individuum Info : ----------------------" << std::endl << std::endl;
    std::cout << "Fitness: " << _fitness << std::endl;
    std::cout << std::endl << "Cut Edges ----------------------------------------" << std::endl;
    for (auto i = _cutEdges.begin(); i != _cutEdges.end(); ++i)
      std::cout << *i << ' ';
    std::cout << std::endl << "----------------------------------------" << std::endl;
  }

 private:
  std::vector<PartitionID> _partition;
  std::vector<HyperedgeID> _cutEdges;
  std::vector<HyperedgeID> _cutEdgesStrong;
  HyperedgeWeight _fitness;
};
}
