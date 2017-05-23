#pragma once

namespace kahypar {
class diversifyer {
  public :
    diversifyer() {};
    virtual ~diversifyer() {};
    void diversify(Configuration &config);
};
    void diversifyer::diversify(Configuration &config) {
      //config.evolutionary.cross_combine_chance = 1 - Randomize::instance().getRandomFloat(0, 1);
      //config.evolutionary.mutation_chance = 1 - Randomize::instance().getRandomFloat(0, 1);
      //config.evolutionary.edge_repeat = Randomize::instance().getRandomInt(1,15);
      config.evolutionary.use_edge_combine = Randomize::instance().flipCoin();
      config.evolutionary.strong_set = Randomize::instance().flipCoin();
      config.evolutionary.cc_objective = static_cast<CrossCombineObjective>(Randomize::instance().getRandomInt(0,4));
      config.evolutionary.stable_net = Randomize::instance().flipCoin();
      config.preprocessing.enable_min_hash_sparsifier = Randomize::instance().flipCoin();
      config.coarsening.max_allowed_weight_multiplier = Randomize::instance().getRandomFloat(1.0, 3.25);
      config.coarsening.contraction_limit_multiplier = Randomize::instance().getRandomInt(100, 160);

      //TODO Config detect Communities
      //config.coarsening.algorithm = ml, lazy
    }
}
