#pragma once

#include <random>
#include <list>
#include "i_combine.h"
#include "i_mutate.h"
#include "individuum.h"
#include "kahypar/definitions.h"
namespace kahypar {



class Population{
  
  
  public:
  Population(Hypergraph &hypergraphInput, unsigned maxPopulationLimit) :
    _hypergraph(hypergraphInput),
      _maxPopulationLimit(maxPopulationLimit),
      _internalPopulation() {}
      
  ~Population() {}

  inline unsigned size() {return _internalPopulation.size();};
  
  inline Individuum combine(Individuum &parent_1, Individuum &parent_2);
  inline Individuum performCombine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator);
  inline void mutate(Individuum &targetIndividuum);
  inline void performMutation(Individuum &target, IMutate &mutator);
  inline double fitness(Individuum &targetIndividuum);

  inline void replace(Individuum &in, unsigned position);//TODO
  inline void replace(Individuum &in, Individuum &out); //TODO
  inline void insertIndividuum(Individuum &insertTarget);
  inline Individuum getIndividuum(unsigned position);
  inline Individuum getRandomIndividuum();
  inline Individuum getIndividuumTournament();
  inline unsigned worstIndividuumPosition();
  inline Individuum generateRandomIndividuum();//TODO
 
 
  private: 
  std::unique_ptr<ICombine> combinator;
  std::unique_ptr<IMutate> mutator;
  std::vector<Individuum> _internalPopulation;
  Hypergraph& _hypergraph;
  unsigned _maxPopulationLimit;

};

    inline Individuum Population::combine(Individuum &parent_1, Individuum &parent_2) {
      return performCombine(parent_1, parent_2, *combinator);
}
    inline Individuum performCombine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator){
      return combinator.combine(parent_1, parent_2);
    }
    inline void Population::mutate(Individuum &targetIndividuum) {
      performMutation(targetIndividuum, *mutator);
}
    inline void performMutation(Individuum &target, IMutate &mutator) {
      mutator.mutate(target);
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
    inline Individuum Population::getIndividuum(unsigned position) {
      return _internalPopulation.at(position);
    }
    inline Individuum Population::getRandomIndividuum() {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<unsigned int> distribution(0, size() -1);
      return getIndividuum(distribution(gen));
    }
    inline unsigned Population::worstIndividuumPosition() {
      //TODO what if we have nothing in our population? Max_unsigned or exception?
      unsigned currentPosition = UINT_MAX;
      double currentFitness = 0.0;
      for(unsigned i = 0; i < size(); i++) {
	//ASSUMING WE MINIMIZE ROBIN:TODO generic compare on Individuum
	if(_internalPopulation[i].getFitness() > currentFitness) {
	  currentFitness = _internalPopulation[i].getFitness();
	  currentPosition = i;
	}
      }
      return currentPosition;
    }
    //TODO if both are the same is bad
    inline Individuum Population::getIndividuumTournament() {
      if(size() == 0) {
	return generateRandomIndividuum();
      }
      if(size() == 1) {
	return _internalPopulation.at(0); 
      }
      Individuum first = getRandomIndividuum();
      Individuum second = getRandomIndividuum();
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
} // end of namespace kahypar
