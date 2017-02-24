#pragma once

#include <random>
#include <list>
#include "i_combine.h"
#include "i_mutate.h"
#include "individuum.h"
#include "kahypar/definitions.h"

namespace kahypar {



class Population {
  
  
  public:
  Population(Hypergraph &hypergraphInput, unsigned maxPopulationLimit) :
    _hypergraph(hypergraphInput),
      _maxPopulationLimit(maxPopulationLimit),
      _internalPopulation() {ASSERT(_maxPopulationLimit != 0);}
      
  ~Population() {}

  inline unsigned size() {return _internalPopulation.size();};
inline void setPartitionVector(Hypergraph &hypergraph, const std::vector<PartitionID>&parent);
  inline Individuum combine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator);
  inline Individuum combine(std::size_t pos1, std::size_t pos2, ICombine &combinator);
  inline void mutate(Individuum &targetIndividuum, IMutate &mutator);
  inline void mutate(std::size_t position, IMutate &mutator);
  inline double fitness(Individuum &targetIndividuum);

 
  inline void replace(Individuum &in, unsigned position);
  inline void replace(Individuum &in, Individuum &out); 
  inline void insertIndividuum(Individuum &insertTarget);
  inline Individuum getIndividuum(unsigned position) const;
  inline std::size_t getRandomIndividuum();
  inline void insertIndividuumFromFile(std::string filename);
  inline Individuum getIndividuumTournament();
  inline unsigned worstIndividuumPosition();
  inline Individuum generateRandomIndividuum();//TODO
  inline Individuum crossCombineMetric(); //TODO
  inline Individuum crossCombine(); //TODO
  inline std::size_t getRandomExcept(std::size_t except);
  inline void printInfo();
  inline void setTheBest();
 
  private: 
  std::vector<Individuum> _internalPopulation;
  Hypergraph &_hypergraph;
  unsigned _maxPopulationLimit;

};
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
 inline void Population::mutate(Individuum &targetIndividuum, IMutate &mutator) {
      mutator.mutate(targetIndividuum);
}

 inline void Population::mutate(std::size_t position, IMutate &mutator) {
   Individuum ind = getIndividuum(position);
   mutate(ind, mutator);

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
       if(size() <= _maxPopulationLimit) {
	_internalPopulation.push_back(insertTarget);
      } else {
	unsigned replaceTargetPosition = worstIndividuumPosition();
	replace(insertTarget, replaceTargetPosition);
	std::cout << "We reached the population limit, replaced the worst element";
	std::cout << std::endl;
      }
      
    }
    
    inline Individuum Population::getIndividuum(unsigned position)const {
      return _internalPopulation.at(position);
    }
    inline std::size_t Population::getRandomIndividuum() {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<unsigned int> distribution(0, size() - 1);
      return distribution(gen);
    }
    //DANGEROUS SIDE EFFECT IN PARALLEL
    inline void Population::insertIndividuumFromFile(std::string filename) {
      
      std::ifstream part_file(filename);
      if(!part_file) {
	std::cout << "FIELE DOESERE NOTE EGISST";
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
      HyperedgeWeight weight = metrics::km1(_hypergraph);
      Individuum individuum(partition, weight);
      insertIndividuum(individuum);
      _hypergraph.resetPartitioning();
      
    }
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
    inline Individuum Population::getIndividuumTournament() {
      if(size() == 0) {
	return generateRandomIndividuum();
      }
      if(size() == 1) {
	return _internalPopulation.at(0); 
      }
      std::size_t firstPosition = getRandomIndividuum();
      Individuum first = getIndividuum(firstPosition);
      Individuum second = getIndividuum(getRandomExcept(firstPosition));
      return first.getFitness() < second.getFitness() ? first : second;
      
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
      
      _internalPopulation.at(position) = in;
    }
    inline std::size_t Population::getRandomExcept(std::size_t except) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<unsigned int> distribution(0, size() - 2);
      std::size_t target = distribution(gen);
      if(target == except) {
	return size() - 1;
      }
      else {
	return target;
      }
    }
    inline void Population::printInfo() {
      std::cout << "Population INFO: ";
      std::cout << std::endl;
      for(std::size_t i = 0; i < size(); i++) {
	std::cout << _internalPopulation.at(i).getFitness();
	std::cout << " ";
	std::cout << i;
	std::cout << std::endl;
      } 
    }
    inline void Population::setTheBest() {
      std::cout << "SETTING THE BEST";
      std::cout << std::endl;
      std::size_t bestPosition = 0;
      HyperedgeWeight bestValue = INT_MAX;
       for(std::size_t i = 0; i < size(); i++) {
	 HyperedgeWeight result = _internalPopulation.at(i).getFitness();
	 if(result < bestValue) {
	   
	   bestPosition = i;
	   bestValue = result;
	 }
	 std::cout << result;
	 std::cout << " ";
	 std::cout << bestValue;
	 std::cout << " ";
	 std::cout << bestPosition;
	 std::cout << std::endl;
      }
       _hypergraph.resetPartitioning();
       setPartitionVector(_hypergraph, _internalPopulation.at(bestPosition).getPartition());
    }
} // end of namespace kahypar
