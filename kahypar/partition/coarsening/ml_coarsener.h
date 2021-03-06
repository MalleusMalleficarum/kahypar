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

#include <limits>
#include <string>
#include <vector>

#include "kahypar/datastructure/fast_reset_flag_array.h"
#include "kahypar/datastructure/sparse_map.h"
#include "kahypar/definitions.h"
#include "kahypar/macros.h"
#include "kahypar/partition/coarsening/policies/rating_tie_breaking_policy.h"

namespace kahypar {
class MLCoarsener final : public ICoarsener,
                          private VertexPairCoarsenerBase<>{
 private:
  static const bool dbg_partition_rating = false;

  using Base = VertexPairCoarsenerBase;

  static constexpr HypernodeID kInvalidTarget = std::numeric_limits<HypernodeID>::max();
  static constexpr RatingType kInvalidScore = std::numeric_limits<RatingType>::min();

  struct Rating {
    Rating(HypernodeID trgt, RatingType val, bool is_valid) :
      target(trgt),
      value(val),
      valid(is_valid) { }

    Rating() :
      target(kInvalidTarget),
      value(kInvalidScore),
      valid(false) { }

    Rating(const Rating&) = delete;
    Rating& operator= (const Rating&) = delete;

    Rating(Rating&&) = default;
    Rating& operator= (Rating&&) = delete;

    HypernodeID target;
    RatingType value;
    bool valid;
  };

 public:
  MLCoarsener(Hypergraph& hypergraph, const Configuration& config,
              const HypernodeWeight weight_of_heaviest_node) :
    Base(hypergraph, config, weight_of_heaviest_node),
    _tmp_ratings(_hg.initialNumNodes()) { }

  virtual ~MLCoarsener() { }

  MLCoarsener(const MLCoarsener&) = delete;
  MLCoarsener& operator= (const MLCoarsener&) = delete;

  MLCoarsener(MLCoarsener&&) = delete;
  MLCoarsener& operator= (MLCoarsener&&) = delete;

 private:
	void coarsenImpl(const HypernodeID limit, EvoParameters &evo) override final {
                
		int pass_nr = 0;
		std::vector<HypernodeID> current_hns;
		ds::FastResetFlagArray<> already_matched(_hg.initialNumNodes());


                if(evo.stableNetVector.size() != 0) {
                  Randomize::instance().shuffleVector(evo.stableNetVector, evo.stableNetVector.size());
                  for(unsigned i = 0; i < evo.stableNetVector.size(); ++i) {
                    bool untouched = true;
                    HypernodeID anchor;
                    for(const HypernodeID u : _hg.pins(evo.stableNetVector[i])) {
                      if(already_matched[u]) {
                        untouched = false;
                      }
                      anchor = u;
                    }
                    if(untouched) {
                      already_matched.set(anchor, true);
                      for(const HypernodeID u : _hg.pins(evo.stableNetVector[i])) {
                        already_matched.set(u, true);
                        if(anchor != u) {
                          performContraction(anchor, u);
                        }
                      }
                    }
                  }
                }


		while (_hg.currentNumNodes() > limit) {

			LOGVAR(pass_nr);
			LOGVAR(_hg.currentNumNodes());

			already_matched.reset();
			current_hns.clear();

			const HypernodeID num_hns_before_pass = _hg.currentNumNodes();
			for (const HypernodeID hn : _hg.nodes()) {

				current_hns.push_back(hn);
			}
			Randomize::instance().shuffleVector(current_hns, current_hns.size());
			// std::sort(std::begin(current_hns), std::end(current_hns),
			//           [&](const HypernodeID l, const HypernodeID r) {
			//             return _hg.nodeDegree(l) < _hg.nodeDegree(r);
			//           });

			for (const HypernodeID hn : current_hns) {

				if (_hg.nodeIsEnabled(hn)) {

					const Rating rating = contractionPartner(hn, already_matched, evo);

					if (rating.target != kInvalidTarget) {

						already_matched.set(hn, true);
						already_matched.set(rating.target, true);
						// if (_hg.nodeDegree(hn) > _hg.nodeDegree(rating.target)) {
						performContraction(hn, rating.target);
						// } else {
						//   contract(rating.target, hn);
						// }
					}

					if (_hg.currentNumNodes() <= limit) {

						break;
					}
				}
			}
			if (num_hns_before_pass == _hg.currentNumNodes()) {

				break;
			}

			++pass_nr;
		}
	
	}
  void coarsenImpl(const HypernodeID limit) override final {
          
	  std::vector<PartitionID>dummy;
	  std::vector<PartitionID>dummy2;
          std::vector<double>dummy3;
          EvoParameters evo(dummy, dummy2, dummy3);
	  coarsenImpl(limit, evo);
  }

  Rating contractionPartner(const HypernodeID u, const ds::FastResetFlagArray<>& already_matched, EvoParameters &evo) {
    DBG(dbg_partition_rating, "Calculating rating for HN " << u);
    const HypernodeWeight weight_u = _hg.nodeWeight(u);
    for (const HyperedgeID he : _hg.incidentEdges(u)) {

      ASSERT(_hg.edgeSize(he) > 1, V(he));
      if (_hg.edgeSize(he) <= _config.partition.hyperedge_size_threshold) {
	
        const RatingType score = static_cast<RatingType>(_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
        
	if(evo.parent_1.size() == 0 || evo.parent_2.size() == 0){
		for (const HypernodeID v : _hg.pins(he)) {
                const RatingType score2 = static_cast<RatingType>(EdgeFrequency::rating(_hg, _config, v, u, he, evo));
          	if ((v != u && belowThresholdNodeWeight(weight_u, _hg.nodeWeight(v)))) {
           	 _tmp_ratings[v] += score2;
          	}
       	 }
	} else {
	for (const HypernodeID v : _hg.pins(he)) {
                const RatingType score2 = static_cast<RatingType>(EdgeFrequency::rating(_hg, _config, v, u, he, evo));
          	if ((v != u && belowThresholdNodeWeight(weight_u, _hg.nodeWeight(v))) && (evo.parent_1[u] == evo.parent_1[v]) && (evo.parent_2[u] == evo.parent_2[v])) {
           	 _tmp_ratings[v] += score2;
          	}
       	 }
	}
        
      }
    }
    ASSERT(!_tmp_ratings.contains(u), V(u));

    RatingType max_rating = std::numeric_limits<RatingType>::min();
    HypernodeID target = std::numeric_limits<HypernodeID>::max();
    for (auto it = _tmp_ratings.end() - 1; it >= _tmp_ratings.begin(); --it) {

      const HypernodeID tmp_target = it->key;
      RatingType tmp_rating; //const
      if(evo.frequency.size() == 0) {
        tmp_rating = it->value;
      }
      else {
        tmp_rating = it->value / std::pow((double)(_hg.nodeWeight(u) * _hg.nodeWeight(tmp_target)), 1.2);
      }
      
      DBG(false, "r(" << u << "," << tmp_target << ")=" << tmp_rating);
      if (acceptRating(tmp_rating, max_rating, target, tmp_target, already_matched)) {
        max_rating = tmp_rating;
        target = tmp_target;
      }
    }

    Rating ret;
    if (max_rating != std::numeric_limits<RatingType>::min()) {
      ASSERT(target != std::numeric_limits<HypernodeID>::max());
      //ASSERT(_tmp_ratings[target] == max_rating, V(target));
      ret.value = max_rating;
      ret.target = target;
      ret.valid = true;
    }

    _tmp_ratings.clear();

    ASSERT(!ret.valid || (_hg.partID(u) == _hg.partID(ret.target)));
    DBG(dbg_partition_rating, "rating=(" << ret.value << "," << ret.target << ","
        << ret.valid << ")");
    return ret;
  }

  bool acceptRating(const RatingType tmp, const RatingType max_rating,
                    const HypernodeID old_target, const HypernodeID new_target,
                    const ds::FastResetFlagArray<>& already_matched) const {
    return max_rating < tmp ||
           ((max_rating == tmp) &&
            ((already_matched[old_target] && !already_matched[new_target]) ||
             (already_matched[old_target] && already_matched[new_target] &&
              RandomRatingWins::acceptEqual()) ||
             (!already_matched[old_target] && !already_matched[new_target] &&
              RandomRatingWins::acceptEqual())));
  }

  bool uncoarsenImpl(IRefiner& refiner) override final {
    return doUncoarsen(refiner);
  }

  std::string policyStringImpl() const override final {
    return std::string("");
  }

  bool belowThresholdNodeWeight(const HypernodeWeight weight_u,
                                const HypernodeWeight weight_v) const {
    return weight_v + weight_u <= _config.coarsening.max_allowed_node_weight;
  }

  using Base::_pq;
  using Base::_hg;
  using Base::_config;
  using Base::_history;
  ds::SparseMap<HypernodeID, RatingType> _tmp_ratings;
};
}  // namespace kahypar
