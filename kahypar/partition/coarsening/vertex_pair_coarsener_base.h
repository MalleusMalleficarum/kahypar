/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2014 Sebastian Schlag <sebastian.schlag@kit.edu>
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

#include <algorithm>
#include <limits>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "kahypar/datastructure/binary_heap.h"
#include "kahypar/definitions.h"
#include "kahypar/meta/int_to_type.h"
#include "kahypar/partition/coarsening/coarsener_base.h"
#include "kahypar/partition/coarsening/heavy_edge_rater.h"
#include "kahypar/partition/configuration.h"
#include "kahypar/partition/metrics.h"
#include "kahypar/partition/refinement/i_refiner.h"
#include "kahypar/utils/randomize.h"

namespace kahypar {
template <class PrioQueue = ds::BinaryMaxHeap<HypernodeID, RatingType> >
class VertexPairCoarsenerBase : public CoarsenerBase {
 protected:
  using CoarsenerBase::performLocalSearch;
  using CoarsenerBase::initializeRefiner;
  using CoarsenerBase::performContraction;

 public:
  VertexPairCoarsenerBase(Hypergraph& hypergraph, const Configuration& config,
                          const HypernodeWeight weight_of_heaviest_node) :
    CoarsenerBase(hypergraph, config, weight_of_heaviest_node),
    _pq(_hg.initialNumNodes()) { }

  ~VertexPairCoarsenerBase() { }

  VertexPairCoarsenerBase(const VertexPairCoarsenerBase&) = delete;
  VertexPairCoarsenerBase& operator= (const VertexPairCoarsenerBase&) = delete;

  VertexPairCoarsenerBase(VertexPairCoarsenerBase&&) = delete;
  VertexPairCoarsenerBase& operator= (VertexPairCoarsenerBase&&) = delete;

 protected:
  FRIEND_TEST(ACoarsener, SelectsNodePairToContractBasedOnHighestRating);

  bool doUncoarsen(IRefiner& refiner) {
    Metrics current_metrics = { metrics::hyperedgeCut(_hg),
                                metrics::km1(_hg),
                                metrics::imbalance(_hg, _config) };
    HyperedgeWeight initial_objective = std::numeric_limits<HyperedgeWeight>::min();

    switch (_config.partition.objective) {
      case Objective::cut:
        initial_objective = current_metrics.cut;
        Stats::instance().add(_config, "initialCut", initial_objective);
        if (_config.partition.verbose_output) {
          LOG("initial cut =" << initial_objective);
        }
        break;
      case Objective::km1:
        initial_objective = current_metrics.km1;
        Stats::instance().add(_config, "initialKm1", initial_objective);
        if (_config.partition.verbose_output) {
          LOG("initial km1 =" << initial_objective);
        }
        break;
      default:
        LOG("Unknown Objective");
        exit(-1);
    }

    Stats::instance().add(_config, "initialImbalance", current_metrics.imbalance);
    if (_config.partition.verbose_output) {
      LOG("initial imbalance=" << current_metrics.imbalance);
      LOG("target  weights (RB): w(0)=" << _config.partition.max_part_weights[0]
          << " w(1)=" << _config.partition.max_part_weights[1]);
      LOG("initial weights (RB): w(0)=" << _hg.partWeight(0) << " w(1)=" << _hg.partWeight(1));
    }


    initializeRefiner(refiner);
	std::cout << "This was a triumph";
	std::cout << std::endl;
    std::vector<HypernodeID> refinement_nodes(2, 0);
	std::cout << "I'm making a note here";
	std::cout << std::endl;
    UncontractionGainChanges changes;
	std::cout << "Huge Success";
	std::cout << std::endl;
    changes.representative.push_back(0);
	std::cout << "Even though";
	std::cout << std::endl;
    changes.contraction_partner.push_back(0);
	std::cout << "You broke my heart";
	std::cout << std::endl;
    while (!_history.empty()) {
      restoreParallelHyperedges();
      restoreSingleNodeHyperedges();

      DBG(dbg_coarsening_uncoarsen, "Uncontracting: (" << _history.back().contraction_memento.u << ","
          << _history.back().contraction_memento.v << ")");

      refinement_nodes.clear();
      refinement_nodes.push_back(_history.back().contraction_memento.u);
      refinement_nodes.push_back(_history.back().contraction_memento.v);

      if (_hg.currentNumNodes() > _max_hn_weights.back().num_nodes) {
        _max_hn_weights.pop_back();
      }

      switch (_config.local_search.algorithm) {
        case RefinementAlgorithm::twoway_fm:
          _hg.uncontract(_history.back().contraction_memento, changes,
                         meta::Int2Type<static_cast<int>(RefinementAlgorithm::twoway_fm)>());
          break;
        default:
          _hg.uncontract(_history.back().contraction_memento);
      }
      performLocalSearch(refiner, refinement_nodes, current_metrics, changes);
      changes.representative[0] = 0;
      changes.contraction_partner[0] = 0;
      _history.pop_back();
    }

    // This currently cannot be guaranteed for RB-partitioning and k != 2^x, since it might be
    // possible that 2FM cannot re-adjust the part weights to be less than Lmax0 and Lmax1.
    // In order to guarantee this, 2FM would have to force rebalancing by sacrificing cut-edges.
    // ASSERT(current_imbalance <= _config.partition.epsilon,
    //        "balance_constraint is violated after uncontraction:" << metrics::imbalance(_hg, _config)
    //        << " > " << _config.partition.epsilon);
    Stats::instance().add(_config, "finalImbalance", current_metrics.imbalance);
    if (_config.partition.verbose_output) {
      LOG("final weights (RB):   w(0)=" << _hg.partWeight(0) << " w(1)=" << _hg.partWeight(1));
    }
    bool improvement_found = false;
    switch (_config.partition.objective) {
      case Objective::cut:
        Stats::instance().add(_config, "finalCut", current_metrics.cut);
        if (_config.partition.verbose_output) {
          LOG("final cut: " << current_metrics.cut);
        }
        improvement_found = current_metrics.cut < initial_objective;
        break;
      case Objective::km1:
        if (_config.partition.mode == Mode::recursive_bisection) {
          // In recursive bisection-based (initial) partitioning, km1
          // is optimized using TwoWayFM and cut-net splitting. Since
          // TwoWayFM optimizes cut, current_metrics.km1 is not updated
          // during local search (it is currently only updated/maintained
          // during k-way k-1 refinement). In order to provide correct outputs,
          // we explicitly calculated the metric after uncoarsening.
          current_metrics.km1 = metrics::km1(_hg);
        }
        Stats::instance().add(_config, "finalKm1", current_metrics.km1);
        if (_config.partition.verbose_output) {
          LOG("final km1: " << current_metrics.km1);
        }
        improvement_found = current_metrics.km1 < initial_objective;
        break;
      default:
        LOG("Unknown Objective");
        exit(-1);
    }

    return improvement_found;
  }

  template <typename Rater>
  void rateAllHypernodes(Rater& rater,
                         std::vector<HypernodeID>& target) {
    std::vector<HypernodeID> permutation;
    createHypernodePermutation(permutation);
    for (size_t i = 0; i < permutation.size(); ++i) {
      const typename Rater::Rating rating = rater.rate(permutation[i]);
      if (rating.valid) {
        _pq.push(permutation[i], rating.value);
        target[permutation[i]] = rating.target;
      }
    }
  }
  template <typename Rater>
  void rateAllHypernodes(Rater& rater,
	  std::vector<HypernodeID>& target, const std::vector<PartitionID>& parent_1, const std::vector<PartitionID>& parent_2) {
	  std::vector<HypernodeID> permutation;
	  createHypernodePermutation(permutation);
	  for (size_t i = 0; i < permutation.size(); ++i) {
		  const typename Rater::Rating rating = rater.rate(permutation[i], parent_1, parent_2);
		  if (rating.valid) {
			  _pq.push(permutation[i], rating.value);
			  target[permutation[i]] = rating.target;
		  }
	  }
  }


  void createHypernodePermutation(std::vector<HypernodeID>& permutation) {
    permutation.reserve(_hg.initialNumNodes());
    for (HypernodeID hn : _hg.nodes()) {
      permutation.push_back(hn);
    }
    Randomize::instance().shuffleVector(permutation, permutation.size());
  }

  using CoarsenerBase::_hg;
  using CoarsenerBase::_config;
  PrioQueue _pq;
};
}  // namespace kahypar
