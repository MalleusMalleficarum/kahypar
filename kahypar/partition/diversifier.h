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

namespace kahypar {
// TODO(robin): maintain less state:
// [ ] use a evo::diversify namespace to encapsulate free functions
// [ ] make one function for combine

class diversifyer {
 public:
  diversifyer() { }
  virtual ~diversifyer() { }
  void diversify(Configuration& config);
};
void diversifyer::diversify(Configuration& config) {
  //config.evolutionary.cross_combine_chance = 1 - Randomize::instance().getRandomFloat(0, 1);
  //config.evolutionary.mutation_chance = 1 - Randomize::instance().getRandomFloat(0, 1);
  //config.evolutionary.edge_repeat = Randomize::instance().getRandomInt(1,15);
  config.evolutionary.use_edge_combine = Randomize::instance().flipCoin();
  // TODO(robin): remove: we always use strong set difference since weak didn't make sense
  config.evolutionary.strong_set = Randomize::instance().flipCoin();
  config.evolutionary.cc_objective = static_cast<CrossCombineObjective>(Randomize::instance().getRandomInt(0, 4));
  config.evolutionary.stable_net = Randomize::instance().flipCoin();
  config.preprocessing.enable_min_hash_sparsifier = Randomize::instance().flipCoin();
  config.coarsening.max_allowed_weight_multiplier = Randomize::instance().getRandomFloat(1.0, 3.25);
  config.coarsening.contraction_limit_multiplier = Randomize::instance().getRandomInt(100, 160);

  //TODO Config detect Communities
  //config.coarsening.algorithm = ml, lazy
}
}
