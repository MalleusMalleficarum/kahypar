/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2014-2016 Sebastian Schlag <sebastian.schlag@kit.edu>
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
#include <stack>
#include <vector>

#include "kahypar/datastructure/sparse_map.h"
#include "kahypar/definitions.h"
#include "kahypar/macros.h"
#include "kahypar/partition/configuration.h"

namespace kahypar {
static const bool dbg_partition_rating = false;

template <typename _RatingType, class _TieBreakingPolicy>
class HeavyEdgeRater {
 public:
  using RatingType = _RatingType;

 private:
  using TieBreakingPolicy = _TieBreakingPolicy;

  struct HeavyEdgeRating {
    HeavyEdgeRating(HypernodeID trgt, RatingType val, bool is_valid) :
      target(trgt),
      value(val),
      valid(is_valid) { }

    HeavyEdgeRating() :
      target(std::numeric_limits<HypernodeID>::max()),
      value(std::numeric_limits<RatingType>::min()),
      valid(false) { }

    HeavyEdgeRating(const HeavyEdgeRating&) = delete;
    HeavyEdgeRating& operator= (const HeavyEdgeRating&) = delete;

    HeavyEdgeRating(HeavyEdgeRating&&) = default;
    HeavyEdgeRating& operator= (HeavyEdgeRating&&) = delete;

    HypernodeID target;
    RatingType value;
    bool valid;
  };

 public:
  using Rating = HeavyEdgeRating;

  HeavyEdgeRater(Hypergraph& hypergraph, const Configuration& config) :
    _hg(hypergraph),
    _config(config),
    _tmp_ratings(_hg.initialNumNodes()) { }

  HeavyEdgeRater(const HeavyEdgeRater&) = delete;
  HeavyEdgeRater& operator= (const HeavyEdgeRater&) = delete;

  HeavyEdgeRater(HeavyEdgeRater&&) = delete;
  HeavyEdgeRater& operator= (HeavyEdgeRater&&) = delete;


  HeavyEdgeRating rate(const HypernodeID u, const std::vector<PartitionID>& parent_1, const std::vector<PartitionID>& parent_2) {
		std::cout << "################";
		std::cout << std::endl;
		std::cout << "Good Rate :)";
		std::cout << std::endl;
		std::cout << "################";
		std::cout << std::endl;
	  DBG(dbg_partition_rating, "Calculating rating for HN " << u);
	  const HypernodeWeight weight_u = _hg.nodeWeight(u);

	  if (parent_1.empty() || parent_2.empty()) {
				std::cout << "################";
		std::cout << std::endl;
		std::cout << "No parents :(";
		std::cout << std::endl;
		std::cout << "################";
		  // If the vectors were not passed it is likely that they should not  be used, in that case
		  // the graph should be getting an initial partition and part_u should exist.
		  const PartitionID part_u = _hg.partID(u);
		  for (const HyperedgeID he : _hg.incidentEdges(u)) {
			  ASSERT(_hg.edgeSize(he) > 1, V(he));
			  const RatingType score = static_cast<RatingType>(_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
			  for (const HypernodeID v : _hg.pins(he)) {
				  if (v != u &&
					  belowThresholdNodeWeight(weight_u, _hg.nodeWeight(v)) &&
					  (part_u == _hg.partID(v))) {
					  _tmp_ratings[v] += score;
				  }
			  }
		  }
	  }
	  else {
		std::cout << "################";
		std::cout << std::endl;
		std::cout << "Parents :)";
		std::cout << std::endl;
		std::cout << "################";
		  //if both parents are defined
		  for (const HyperedgeID he : _hg.incidentEdges(u)) {

			  ASSERT(_hg.edgeSize(he) > 1, V(he));
			  const RatingType score = static_cast<RatingType>(_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
			  for (const HypernodeID v : _hg.pins(he)) {
				  if (v != u &&
					  belowThresholdNodeWeight(weight_u, _hg.nodeWeight(v)) &&
					  (parent_1[u] == parent_1[v]) && (parent_2[u] == parent_2[v])) {
					  _tmp_ratings[v] += score;
				  }
			  }
		  }
	  } 


	  RatingType max_rating = std::numeric_limits<RatingType>::min();
	  HypernodeID target = std::numeric_limits<HypernodeID>::max();
	  for (auto it = _tmp_ratings.end() - 1; it >= _tmp_ratings.begin(); --it) {
		  const HypernodeID tmp_target = it->key;
		  const RatingType tmp = it->value /
			  (weight_u * _hg.nodeWeight(tmp_target));
		  DBG(false, "r(" << u << "," << tmp_target << ")=" << tmp);
		  if (acceptRating(tmp, max_rating)) {
			  max_rating = tmp;
			  target = tmp_target;
		  }
	  }
	  _tmp_ratings.clear();
	  HeavyEdgeRating ret;
	  if (max_rating != std::numeric_limits<RatingType>::min()) {
		  ASSERT(target != std::numeric_limits<HypernodeID>::max(), "invalid contraction target");
		  ret.value = max_rating;
		  ret.target = target;
		  ret.valid = true;
	  }
	  ASSERT([&]() {
		  bool flag = true;
		  if (ret.valid && (_hg.partID(u) != _hg.partID(ret.target))) {
			  flag = false;
		  }
		  return flag;
	  } (), "Representative " << u << " & contraction target " << ret.target
		  << " are in different parts!");
	  DBG(dbg_partition_rating, "rating=(" << ret.value << "," << ret.target << ","
		  << ret.valid << ")");
	  return ret;
  }

  HeavyEdgeRating rate(const HypernodeID u) {
		std::cout << "################";
		std::cout << std::endl;
		std::cout << "Stupid RATE" << u;
		std::cout << std::endl;
		std::cout << "################";
		std::cout << std::endl;
    DBG(dbg_partition_rating, "Calculating rating for HN " << u);
    const HypernodeWeight weight_u = _hg.nodeWeight(u);
    const PartitionID part_u = _hg.partID(u);
    for (const HyperedgeID he : _hg.incidentEdges(u)) {
      ASSERT(_hg.edgeSize(he) > 1, V(he));
      const RatingType score = static_cast<RatingType>(_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
      for (const HypernodeID v : _hg.pins(he)) {
        if (v != u &&
            belowThresholdNodeWeight(weight_u, _hg.nodeWeight(v)) &&
            (part_u == _hg.partID(v))) {
          _tmp_ratings[v] += score;
        }
      }
    }

    RatingType max_rating = std::numeric_limits<RatingType>::min();
    HypernodeID target = std::numeric_limits<HypernodeID>::max();
    for (auto it = _tmp_ratings.end() - 1; it >= _tmp_ratings.begin(); --it) {
      const HypernodeID tmp_target = it->key;
      const RatingType tmp = it->value /
                             (weight_u * _hg.nodeWeight(tmp_target));
      DBG(false, "r(" << u << "," << tmp_target << ")=" << tmp);
      if (acceptRating(tmp, max_rating)) {
        max_rating = tmp;
        target = tmp_target;
      }
    }
    _tmp_ratings.clear();
    HeavyEdgeRating ret;
    if (max_rating != std::numeric_limits<RatingType>::min()) {
      ASSERT(target != std::numeric_limits<HypernodeID>::max(), "invalid contraction target");
      ret.value = max_rating;
      ret.target = target;
      ret.valid = true;
    }
    ASSERT([&]() {
        bool flag = true;
        if (ret.valid && (_hg.partID(u) != _hg.partID(ret.target))) {
          flag = false;
        }
        return flag;
      } (), "Representative " << u << " & contraction target " << ret.target
           << " are in different parts!");
    DBG(dbg_partition_rating, "rating=(" << ret.value << "," << ret.target << ","
        << ret.valid << ")");
    return ret;
  }


  HypernodeWeight thresholdNodeWeight() const {
    return _config.coarsening.max_allowed_node_weight;
  }

 private:
  bool belowThresholdNodeWeight(const HypernodeWeight weight_u,
                                const HypernodeWeight weight_v) const {
    return weight_v + weight_u <= _config.coarsening.max_allowed_node_weight;
  }

  bool acceptRating(const RatingType tmp, const RatingType max_rating) const {
    return max_rating < tmp || (max_rating == tmp && TieBreakingPolicy::acceptEqual());
  }

  Hypergraph& _hg;
  const Configuration& _config;
  ds::SparseMap<HypernodeID, RatingType> _tmp_ratings;
};
}  // namespace kahypar
