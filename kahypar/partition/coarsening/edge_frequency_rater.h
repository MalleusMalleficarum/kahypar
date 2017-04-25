#pragma once
#include <math.h>
#include "kahypar/datastructure/sparse_map.h"
#include "kahypar/definitions.h"
#include "kahypar/macros.h"
#include "kahypar/partition/configuration.h"
#include "kahypar/partition/evo_parameters.h"
//
namespace kahypar {
  class EdgeFrequency {
    private:
    public:
    static double rating(Hypergraph &_hg, const Configuration &config, const HypernodeID& v, const HypernodeID& u, const HyperedgeID& he, EvoParameters &evo);

  };
  inline double EdgeFrequency::rating(Hypergraph &_hg, const Configuration &config, const HypernodeID& v, const HypernodeID& u, const HyperedgeID& he, EvoParameters &evo) {
//
    if(!evo.edgeFrequency) {
      return ((double)_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
    } 
    else { 
      double currentValue = exp(-config.evolutionary.gamma * evo.frequency[he]) * ((double)_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
      if(config.evolutionary.edgeFrequencyUsesWeight) {
        currentValue = currentValue / (_hg.nodeWeight(u) * _hg.nodeWeight(v));
      }
      //return (currentValue);
      return ((double)_hg.edgeWeight(he)) / (_hg.edgeSize(he) - 1);
      //return 0;
    }
  }
  

}
