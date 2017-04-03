#pragma once

#include <random>
#include <list>
#include "i_combine.h"
#include "i_mutate.h"
#include "individuum.h"
#include "kahypar/definitions.h"
#include "i_replace.h"

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
  inline Individuum mutate(Individuum &targetIndividuum, IMutate &mutator);
  inline Individuum mutate(std::size_t position, IMutate &mutator);
  inline double fitness(Individuum &targetIndividuum);

  inline unsigned replaceDiverse(Individuum &in);
  inline std::size_t replace(Individuum &in, IReplace &replicator);
  inline void replace(Individuum &in, unsigned position);
  inline void replace(Individuum &in, Individuum &out); 
  inline void insertIndividuum(Individuum &insertTarget);
  inline Individuum getIndividuum(unsigned position) const;
  inline std::size_t getRandomIndividuum();
  inline void insertIndividuumFromFile(std::string filename);
  inline Individuum getIndividuumTournament();
  inline std::pair<Individuum, Individuum> getTwoIndividuumTournament();
  inline unsigned worstIndividuumPosition();
  inline Individuum generateIndividuum(Configuration &config);//TODO
  inline Individuum crossCombineMetric(); //TODO
  inline Individuum crossCombine(); //TODO
  inline std::size_t getRandomExcept(std::size_t except);
  inline void printInfo();
  inline void setTheBest();
  inline double getAverageFitness();
 
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
      Individuum individuum(partition,cutEdges , weight);
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
    inline Individuum Population::generateIndividuum(Configuration &config) {
      _hypergraph.resetPartitioning();
      Partitioner partitioner;
      std::vector<PartitionID> dummy;
      std::vector<PartitionID> dummy2;
      partitioner.partition(_hypergraph, config, dummy, dummy2);
      std::vector<PartitionID> result;
      	for (HypernodeID u : _hypergraph.nodes()) {
		
	  result.push_back(_hypergraph.partID(u));
	}
	std::vector<HyperedgeID> cutEdges;
	for(HyperedgeID v : _hypergraph.edges()) {
	  if(_hypergraph.connectivity(v) > 1) {
	    for(unsigned i = 1; i < _hypergraph.connectivity(v);i++) {
              cutEdges.push_back(v);
            }
	    
	  }
	}
      HyperedgeWeight weight = metrics::km1(_hypergraph);
      Individuum ind(result, cutEdges, weight);
      insertIndividuum(ind);
      return ind;
    }
    inline Individuum Population::getIndividuumTournament() {
      if(size() == 0) {
	//TODO MASSIVE ERROR
      }
      if(size() == 1) {
	return _internalPopulation.at(0); 
      }
      std::size_t firstPosition = getRandomIndividuum();
      Individuum first = getIndividuum(firstPosition);
      Individuum second = getIndividuum(getRandomExcept(firstPosition));
      return first.getFitness() < second.getFitness() ? first : second;
      
    }
    inline std::pair<Individuum, Individuum> Population::getTwoIndividuumTournament() {
      Individuum firstRet = getIndividuumTournament();
      std::size_t firstPosition = getRandomIndividuum();
      Individuum one = getIndividuum(firstPosition);
      Individuum two = getIndividuum(getRandomExcept(firstPosition));
      Individuum secondRet = one.getFitness() < two.getFitness() ? one : two;
      if(firstRet.getFitness() == secondRet.getFitness()) {
        secondRet = one.getFitness() >= two.getFitness() ? one : two;
      }
      std::pair<Individuum, Individuum> p(firstRet, secondRet);
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
    
    inline unsigned Population::replaceDiverse(Individuum &in) {
      unsigned max_similarity = std::numeric_limits<unsigned>::max();
      unsigned max_similarity_id = 0; 
      if(in.getFitness() > getIndividuum(worstIndividuumPosition()).getFitness()) {
	//DO WE REALLY WANT A BAD ELEMENT?
	std::cout << "COLLAPSE";
	return std::numeric_limits<unsigned>::max();
      } 
      for(unsigned i = 0; i < size(); i ++) {
        if(_internalPopulation[i].getFitness() >= in.getFitness()) {

      std::vector<HyperedgeID> output_diff;
      std::vector<HyperedgeID> one(_internalPopulation[i].getCutEdges());
      std::vector<HyperedgeID> two(in.getCutEdges());
      std::set_symmetric_difference(one.begin(),
				    one.end(),
				    two.begin(),
				    two.end(),
				    std::back_inserter(output_diff));

       unsigned similarity = output_diff.size();

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

      }
       _hypergraph.resetPartitioning();
       setPartitionVector(_hypergraph, _internalPopulation.at(bestPosition).getPartition());
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
