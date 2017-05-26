/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2015 Tobias Heuer <tobias.heuer@gmx.net>
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

#include <limits>
#include <vector>

#include "kahypar/partition/configuration.h"
#include "kahypar/partition/metrics.h"

namespace kahypar {
class IInitialPartitioner {
 public:
  IInitialPartitioner(const IInitialPartitioner&) = delete;
  IInitialPartitioner(IInitialPartitioner&&) = delete;
  IInitialPartitioner& operator= (const IInitialPartitioner&) = delete;
  IInitialPartitioner& operator= (IInitialPartitioner&&) = delete;


  void partition(Hypergraph& hg, const Configuration& config) {
    std::cout << "IP:" << config.partition.k << std::endl;
    HyperedgeWeight best_cut = std::numeric_limits<HyperedgeWeight>::max();
    std::vector<PartitionID> best_partition(hg.initialNumNodes(), 0);
    for (int i = 0; i < config.initial_partitioning.nruns; ++i) {
      // hg.resetPartitioning() is called in partitionImpl
      partitionImpl();
      const HyperedgeWeight current_cut = metrics::hyperedgeCut(hg);
      if (current_cut < best_cut) {
        best_cut = current_cut;
        for (HypernodeID hn : hg.nodes()) {
          best_partition[hn] = hg.partID(hn);
        }
      }
    }
    hg.resetPartitioning();
    for (HypernodeID hn : hg.nodes()) {
      hg.setNodePart(hn, best_partition[hn]);
    }
  }


  virtual ~IInitialPartitioner() { }

 protected:
  IInitialPartitioner() { }

 private:
  virtual void partitionImpl() = 0;
};
}  // namespace kahypar
