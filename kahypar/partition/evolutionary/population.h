#pragma once

#include <random>
#include <list>
#include "kahypar/utils/randomize.h"
#include "i_combine.h"
#include "i_mutate.h"
#include "individuum.h"
#include "kahypar/definitions.h"
#include "i_replace.h"
#include "kahypar/datastructure/sparse_map.h"
namespace kahypar {



class Population {
  
  
  public:
  Population(Hypergraph &hypergraphInput,Configuration &config ,unsigned maxPopulationLimit) :
    _hypergraph(hypergraphInput),
    _config(config),
      _maxPopulationLimit(maxPopulationLimit),
      _internalPopulation() {ASSERT(_maxPopulationLimit != 0);}
      
  ~Population() {}

  inline unsigned size() {return _internalPopulation.size();};
inline void setPartitionVector(Hypergraph &hypergraph, const std::vector<PartitionID>&parent);
  inline void printDifference(Individuum &printIn);
  inline Individuum combine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator);
  inline Individuum combine(std::size_t pos1, std::size_t pos2, ICombine &combinator);
  inline Individuum mutate(Individuum &targetIndividuum, IMutate &mutator);
  inline Individuum mutate(std::size_t position, IMutate &mutator);
  inline double fitness(Individuum &targetIndividuum);
  static inline Individuum createIndividuum(Hypergraph &hypergraph, Configuration &config);
  inline unsigned replaceDiverse(Individuum &in, bool acceptWorse);
  inline unsigned replaceStrategy(Individuum &in, bool acceptWorse);
  inline std::size_t replace(Individuum &in, IReplace &replicator);
  inline void replace(Individuum &in, unsigned position);
  inline void replace(Individuum &in, Individuum &out); 
  inline void insertIndividuum(Individuum &insertTarget);
  inline Individuum &getIndividuum(unsigned position);
  inline std::size_t getRandomIndividuum();
  inline void insertIndividuumFromFile(std::string filename);
  inline Individuum getIndividuumTournament();
  inline unsigned getIndividuumTournamentPosition();
  inline std::pair<Individuum, Individuum> getTwoIndividuumTournament();
  inline std::pair<unsigned, unsigned> getTwoIndividuumTournamentPosition();
  inline unsigned worstIndividuumPosition();
  inline Individuum generateIndividuum(Configuration &config);
  inline std::vector<unsigned> bestPositions(unsigned amount);
  inline unsigned difference(Individuum &in, unsigned position);
  inline Individuum crossCombineMetric(); //TODO
  inline Individuum crossCombine(); //TODO
  inline std::size_t getRandomExcept(std::size_t except);
  inline void printInfo();
  inline void setTheBest();
  inline double getAverageFitness();
  inline Individuum individuumFromEdgeFrequency(Configuration &config, std::vector<double> &edgeFrequency); //TODO
  inline std::vector<double> edgeFrequency(std::vector<unsigned> &positions); 
  
 
  private: 
  std::vector<Individuum> _internalPopulation;
  Hypergraph &_hypergraph;
  Configuration &_config;
  //std::map<HyperedgeID, Connectivity>  _edgeFrequencyMap;
  unsigned _maxPopulationLimit;

};

  inline void Population::printDifference(Individuum &in) {
    
    for(int i = 0; i < size(); ++i) {
      std::cout << difference(in, i) << ' ';
    }
    std::cout << std::endl;
  }
inline std::vector<unsigned> Population::bestPositions(unsigned amount) {
  std::vector<std::pair<double, unsigned>> sorting;
  for (unsigned i = 0; i < _internalPopulation.size(); ++i) {
    
    sorting.push_back(std::make_pair(_internalPopulation[i].getFitness(), i));
  }
  std::sort(sorting.begin(), sorting.end());
  std::vector<unsigned> positions;
  std::cout << std::endl << "List ";
  for (int it = 0; it < sorting.size() && it < amount; ++it) {
    std::cout << sorting[it].second << ' ';
    positions.push_back(sorting[it].second);
  }
  return positions;


  
}
inline unsigned Population::replaceStrategy(Individuum &in, bool acceptWorse = false) {
  if(_config.evolutionary.replace_diverse) {
    return replaceDiverse(in, acceptWorse);
  }
  else {
    unsigned worst = worstIndividuumPosition();
    replace(in, worst);
    return worst;
  }
}
inline Individuum Population::individuumFromEdgeFrequency(Configuration &config, std::vector<double> &edgeFrequency) {
    Partitioner partitioner;
    std::vector<PartitionID> dummy;

    EvoParameters evo(dummy, dummy, false, true, edgeFrequency);

    partitioner.partition(_hypergraph, config, evo);

    Individuum ind = createIndividuum(_hypergraph, config);

    std::cout << ind.getFitness();
    return ind;

}
inline Individuum Population::createIndividuum(Hypergraph &hypergraph, Configuration &config) {
       std::vector<PartitionID> result;
      	for (HypernodeID u : hypergraph.nodes()) {
		
	  result.push_back(hypergraph.partID(u));
	}
        HyperedgeWeight weight = metrics::km1(hypergraph);
	std::vector<HyperedgeID> cutWeak;
	std::vector<HyperedgeID> cutStrong;

	for(HyperedgeID v : hypergraph.edges()) {
	  auto km1 = hypergraph.connectivity(v) - 1;
	  if(km1 > 0) {
	    cutWeak.push_back(v);
	    for(unsigned i = 1; i < hypergraph.connectivity(v);i++) {
              cutStrong.push_back(v);
	      }
	    
	    
	  }
	}
     
	Individuum ind(result,cutWeak, cutStrong, weight);
      return ind;
    }
 inline std::vector<double> Population::edgeFrequency(std::vector<unsigned> &positions) {

   std::vector<double> edges(_hypergraph.initialNumEdges());

   
   const double increment = 1.0 / positions.size();

   for(unsigned it = 0; it < positions.size(); ++it) {
    
     std::cout << "---------------" << std::endl;
     std::cout << it << ' ' << positions[it] << std::endl;
     
     Individuum current = getIndividuum(positions[it]);
     
     current.printSparse();
     std::vector<HyperedgeID> cutEdges;

     if(_config.evolutionary.edge_strong_set) {

       cutEdges = current.getStrongCutEdges();

     }else {

       cutEdges = current.getCutEdges();

     }
     std::cout << std::endl;
     for(unsigned i = 0; i < cutEdges.size(); ++i) {
       HyperedgeID value = cutEdges[i];
       std::cout << value << " ";
       edges[value] = edges[value] + increment;
     }
     std::cout << std::endl;
   }
   /*std::cout << std::endl <<std::endl;
   for(unsigned it = 0; it < edges.size(); ++it) {
     std::cout << it << ' ' << edges[it] << " ; ";
   }
std::cout << std::endl <<std::endl;*/
   return edges;
 }
inline void Population::setPartitionVector(Hypergraph& hypergraph, const std::vector<PartitionID>& parent) {
	for (HypernodeID u : hypergraph.nodes()) {
		
		hypergraph.setNodePart(u, parent[u]);
	}
}
 inline Individuum Population::combine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator) {
      return combinator.combine(parent_1, parent_2);
}
 inline Individuum Population::combine(std::size_t pos1, std::size_t pos2, ICombine &combinator) {
   Individuum ind1 = getIndividuum(pos1);
   Individuum ind2 = getIndividuum(pos2);
   //TODO bound check
   return combine(ind1, ind2, combinator);
 }
 inline Individuum Population::mutate(Individuum &targetIndividuum, IMutate &mutator) {
      return mutator.mutate(targetIndividuum);
}

 inline Individuum Population::mutate(std::size_t position, IMutate &mutator) {
   Individuum ind = getIndividuum(position);
   return mutate(ind, mutator);

 }
    inline double Population::fitness(Individuum &targetIndividuum) {
      return targetIndividuum.getFitness();
    }

    inline void Population::replace(Individuum &in, Individuum &out) {
      //TODO need a mapping or an ID to reference the out individuum
    }

    /* Enforces placement even if the insertion target is worse than
       everything in the current population*/
    inline void Population::insertIndividuum(Individuum &insertTarget) {
       if(size() < _maxPopulationLimit) {
	_internalPopulation.push_back(insertTarget);
      } else {
	unsigned replaceTargetPosition = worstIndividuumPosition();
	replace(insertTarget, replaceTargetPosition);
	std::cout << "We reached the population limit, replaced the worst element";
	std::cout << std::endl;
      }
      
    }
    
    inline Individuum &Population::getIndividuum(unsigned position) {
      return _internalPopulation[position];
    }
    inline std::size_t Population::getRandomIndividuum() {
      return Randomize::instance().getRandomInt(0, size() -1);
    }
    //DANGEROUS SIDE EFFECT IN PARALLEL TODO NOT IN CREATING
    /*inline void Population::insertIndividuumFromFile(std::string filename) {
      
      std::ifstream part_file(filename);
      if(!part_file) {
	std::cout << "FIT";
	return;
      }
      //TODO ASSERT THE FILE HAS CORRECT LENGTH
      //TODO MAYBE ASSERT THAT THE FILE CONTAINS CORRECT K
      std::vector<PartitionID> partition;
      PartitionID value;
      while(part_file >> value) {
	partition.push_back(value);
      }
      _hypergraph.resetPartitioning();
      setPartitionVector(_hypergraph, partition);
      	std::vector<HyperedgeID> cutEdges;
	for(HyperedgeID v : _hypergraph.edges()) {
	  if(_hypergraph.connectivity(v) > 1) {
	    cutEdges.push_back(v);
	  }
	}
      HyperedgeWeight weight = metrics::km1(_hypergraph);
      Individuum individuum(partition, cutEdges , weight);
      insertIndividuum(individuum);
      _hypergraph.resetPartitioning();
      
      }*/
    inline unsigned Population::worstIndividuumPosition() {
      //TODO what if we have nothing in our population? Max_unsigned or exception?
      unsigned currentPosition = INT_MAX;
      double currentFitness = 0.0;
      for(unsigned i = 0; i < size(); i++) {
	
        
	//ASSUMING WE MINIMIZE ROBIN:TODO generic compare on Individuum
	if(_internalPopulation[i].getFitness() > currentFitness) {
	  currentFitness = _internalPopulation[i].getFitness();
	  currentPosition = i;
	}
	std::cout << _internalPopulation[i].getFitness();
	std::cout << " ";
	std::cout << currentPosition;
	std::cout << " ";
	std::cout << currentFitness;
	std::cout << std::endl;
      }
      return currentPosition;
    }
    inline Individuum Population::generateIndividuum(Configuration &config) {
      _hypergraph.resetPartitioning();
      Partitioner partitioner;
      std::vector<PartitionID>dummy;
      std::vector<PartitionID>dummy2;
      std::vector<double>dummy3;
      EvoParameters evo(dummy, dummy2, dummy3);
      partitioner.partition(_hypergraph, config, evo);
      Individuum ind = createIndividuum(_hypergraph, config);
      insertIndividuum(ind);
      return ind;
    }
    
    inline Individuum Population::getIndividuumTournament() {
      return getIndividuum(getIndividuumTournamentPosition());
      
    }
    inline unsigned Population::getIndividuumTournamentPosition() {
      if(size() == 1) {
        return 0;
      }
      unsigned firstPos = getRandomIndividuum();
      unsigned secondPos = getRandomExcept(firstPos);
      Individuum first = getIndividuum(firstPos);
      Individuum second = getIndividuum(secondPos);
      return first.getFitness() < second.getFitness() ? firstPos : secondPos;
    }
    inline std::pair<Individuum, Individuum> Population::getTwoIndividuumTournament() {
      std::pair<unsigned, unsigned> p = getTwoIndividuumTournamentPosition();
      std::pair<Individuum, Individuum> pReturn(getIndividuum(p.first), getIndividuum(p.second));
      return pReturn;
    }
    inline std::pair<unsigned, unsigned> Population::getTwoIndividuumTournamentPosition() {
      unsigned firstRetPos = getIndividuumTournamentPosition();
      std::size_t firstPosition = getRandomIndividuum();
      std::size_t secondPosition = getRandomExcept(firstPosition);
      Individuum one = getIndividuum(firstPosition);
      Individuum two = getIndividuum(secondPosition);
      unsigned secondRetPos = one.getFitness() < two.getFitness() ? firstPosition : secondPosition;
      if(getIndividuum(firstRetPos).getFitness() == getIndividuum(secondRetPos).getFitness()) {
	secondRetPos = one.getFitness() >= two.getFitness() ? firstPosition : secondPosition;
      }
      std::pair<unsigned, unsigned> p(firstRetPos, secondRetPos);
      return p;
}
    inline std::size_t Population::replace(Individuum &in, IReplace &replicator) {
      
      ReplaceInformation info = replicator.replace(in, _internalPopulation);
      if(!info.willReplace) {
	std::cout << "The Replace Strategy will not insert the Individuum" << std::endl;
	return std::numeric_limits<std::size_t>::max();
      } else {
	replace(in, info.position);
	return info.position;
      }
     
    }
    inline void Population::replace(Individuum &in, unsigned position) {
      if(position >= size()) {
	std::cout << "ERROR: POSITION IS NOT IN POPULATION RANGE: POS: "
		  << std::to_string(position) << " SIZE: " << std::to_string(size());
	std::cout << std::endl;
	/*Since we cannot replace something outside of our population we return
	(Possible Failsave, if the population is not maxlimit, add a new Individuum)
	REMOVES ORIGINAL USE OF REPLACE :( At least no exception*/
	return;
      }
      
      _internalPopulation[position] = in;
    }
    inline std::size_t Population::getRandomExcept(std::size_t except) {
      int target = Randomize::instance().getRandomInt(0, size() -2);
      if(target == except) {
	return size() - 1;
      }
      else {
	return target;
      }
    }
    inline unsigned Population::difference(Individuum &in, unsigned position) {
      std::vector<HyperedgeID> output_diff;
      std::vector<HyperedgeID> one;
      std::vector<HyperedgeID> two;
      if(_config.evolutionary.strong_set) {
	one = _internalPopulation[position].getStrongCutEdges();
	two = in.getStrongCutEdges();
      }
      else {
	one = _internalPopulation[position].getCutEdges();
	two = in.getCutEdges(); 
      }
       
      std::set_symmetric_difference(one.begin(),
				    one.end(),
				    two.begin(),
				    two.end(),
				    std::back_inserter(output_diff));

       return output_diff.size();
    }
    inline unsigned Population::replaceDiverse(Individuum &in, bool acceptWorse) {
      if(size() < _maxPopulationLimit) {
        _internalPopulation.push_back(in);
      }
      unsigned max_similarity = std::numeric_limits<unsigned>::max();
      unsigned max_similarity_id = 0; 
      if(in.getFitness() > getIndividuum(worstIndividuumPosition()).getFitness()) {
	//DO WE REALLY WANT A BAD ELEMENT?
	std::cout << "COLLAPSE";
	return std::numeric_limits<unsigned>::max();
      } 
      for(unsigned i = 0; i < size(); ++i) {
        if(_internalPopulation[i].getFitness() >= in.getFitness() || acceptWorse) {

      unsigned similarity = difference(in, i);
      std::cout << "SYMMETRIC DIFFERENCE: " << similarity << " from " << i <<std::endl;
      if(similarity < max_similarity) {
      max_similarity = similarity;
      max_similarity_id = i;
    }
    }
      }
      replace(in, max_similarity_id);
      return max_similarity_id;
    }
    inline void Population::printInfo() {
      std::cout << "Population INFO: ";
      std::cout << std::endl;
      for(std::size_t i = 0; i < size(); i++) {
	std::cout << _internalPopulation[i].getFitness();
	std::cout << " ";
	std::cout << i;
        //_internalPopulation[i].print(); 
	std::cout << std::endl;
        printDifference(_internalPopulation[i]);
      } 
    }

    inline void Population::setTheBest() {
      std::cout << "SETTING THE BEST";
      std::cout << std::endl;
      std::size_t bestPosition = 0;
      HyperedgeWeight bestValue = INT_MAX;
       for(std::size_t i = 0; i < size(); i++) {
	 HyperedgeWeight result = _internalPopulation[i].getFitness();
	 if(result < bestValue) {
	   
	   bestPosition = i;
	   bestValue = result;
	 }

      }
       _hypergraph.resetPartitioning();
       setPartitionVector(_hypergraph, _internalPopulation[bestPosition].getPartition());
    }
    inline double Population::getAverageFitness() {
      double result = 0;
      for(unsigned i = 0; i < _internalPopulation.size(); i++) {
	result += _internalPopulation[i].getFitness();
      }
      result = result / _internalPopulation.size();
      return result;
    }
} // end of namespace kahypar
