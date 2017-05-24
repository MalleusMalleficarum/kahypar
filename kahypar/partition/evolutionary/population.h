#pragma once
#include <boost/program_options.hpp>
#include <random>
#include <list>
#include "kahypar/utils/randomize.h"
#include "i_combine.h"
#include "i_mutate.h"
#include "individuum.h"
#include "kahypar/definitions.h"
#include "i_replace.h"
#include "kahypar/datastructure/sparse_map.h"
#include "kahypar/partition/preprocessing/louvain.h"

namespace po = boost::program_options;

namespace kahypar {



class Population {
  
  
  public:
  Population(Hypergraph &hypergraphInput,Configuration &config ,unsigned maxPopulationLimit) :
    _hypergraph(hypergraphInput),
    _config(config),
      _maxPopulationLimit(maxPopulationLimit),
      _internalPopulation() {ASSERT(_maxPopulationLimit != 0);}
      
  ~Population() {}

  inline Individuum stableNetNoInitialPartitioning(unsigned positiion, bool vcycle = false);
  inline Individuum stableNetStrategy(unsigned position);
  inline void forceBlock(HyperedgeID he);
  inline Individuum stableNetCreate(unsigned pos, std::vector<HyperedgeID> &theEdges);
  inline Individuum singleStableNet(Configuration &config);
  inline unsigned size() {return _internalPopulation.size();};
inline void setPartitionVector(Hypergraph &hypergraph, const std::vector<PartitionID>&parent);
  inline void printDifference(Individuum &printIn);
  inline Individuum combine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator, bool crossComb = false);
  inline Individuum combine(std::size_t pos1, std::size_t pos2, ICombine &combinator, bool crossComb = false);
  inline Individuum mutate(Individuum &targetIndividuum, IMutate &mutator);
  inline Individuum mutate(std::size_t position, IMutate &mutator);
  inline unsigned bestPosition(); 
  inline void readConfigFromFile(Configuration &config, bool cutRB);
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
  inline Individuum generateIndividuum(Configuration &config, bool insert);
  inline std::vector<unsigned> bestPositions(unsigned amount);
  inline unsigned difference(Individuum &in, unsigned position);
  inline std::vector<unsigned> stableNet(double &amount, std::vector<unsigned> &positions);
  inline Individuum crossCombine(Individuum& original,const Configuration &config, ICombine &comb); 
  inline std::size_t getRandomExcept(std::size_t except);
  inline void printInfo();
  inline void setTheBest();
  inline double getAverageFitness();
  inline Individuum individuumFromEdgeFrequency(Configuration &config, std::vector<double> &edgeFrequency); 
  inline std::vector<double> edgeFrequency(std::vector<unsigned> &positions); 
  inline Individuum combineIndividuumFromEdgeFrequency(unsigned pos1, unsigned pos2, Configuration &config, std::vector<double> &edgeFrequency);
  inline std::vector<unsigned> randomPositions(unsigned amount);
  
 
  private: 
  std::vector<Individuum> _internalPopulation;
  Hypergraph &_hypergraph;
  Configuration &_config;
  
  //std::map<HyperedgeID, Connectivity>  _edgeFrequencyMap;
  unsigned _maxPopulationLimit;

};
  inline std::vector<unsigned> Population::randomPositions(unsigned amount) {
    std::vector<unsigned> fill;
    for(unsigned i = 0; i < _internalPopulation.size(); ++i) {
      fill.push_back(i);
    }
    Randomize::instance().shuffleVector(fill, fill.size());
    std::vector<unsigned> ret;
    for(unsigned i = 0; i < _internalPopulation.size() && i < amount; ++i) {ret.push_back(i);}
    return ret;
  }
  inline Individuum Population::stableNetStrategy(unsigned pos) {
    if(_config.evolutionary.population_stable_net) {
      std::vector<unsigned> positions = bestPositions(_config.evolutionary.stable_net_pop_amount);
      //std::vector<unsigned> positions = randomPositions(_config.evolutionary.stable_net_pop_amount);
      //std::vector<unsigned> positions;
      //positions.push_back(pos);
      std::vector<unsigned> vec = stableNet(_config.evolutionary.stable_net_threshold, positions);
      return stableNetCreate(pos, vec);
    }
    else {
      return stableNetNoInitialPartitioning(pos, _config.evolutionary.stable_net_vcycle);
    }
  }
  inline Individuum Population::stableNetNoInitialPartitioning(unsigned pos, bool vcycle) {
      _hypergraph.reset();
      Individuum indtemp = getIndividuum(pos);
      Partitioner partitioner;
      std::vector<PartitionID>dummy = indtemp.getPartition();
      std::vector<PartitionID>dummy2 = indtemp.getPartition();
      std::vector<double>dummy3;
      EvoParameters evo(dummy, dummy2, dummy3);
      std::vector<HyperedgeID> printy = partitioner.partition(_hypergraph, _config, evo);
      Randomize::instance().shuffleVector(printy, printy.size());
      std::cout << std::endl << std::endl;
      for(int i = 0; i < printy.size(); ++i) {
        std::cout << printy[i] << ' '; 
      }
      std::cout << std::endl << std::endl;
      bool touched [_hypergraph.initialNumNodes()] = {};

      for(int i = 0; i < printy.size(); ++i) {
        bool touch = false;
        for(HypernodeID u : _hypergraph.pins(printy[i])) {
          if(touched[u]) {
            touch = true;
          }
        }

        if(!touch) {
          for(HypernodeID u :_hypergraph.pins(printy[i])) {
            touched[u] = true;
          } 
          forceBlock(printy[i]);
        }
        else {std::cout << "-"<<printy[i] << ' ';}
      }
      /*for(int i = 0; i < printy.size(); ++i) {
        std::cout << printy[i] << ' ';
      }*/
      Individuum ind = createIndividuum(_hypergraph, _config);
      ind.printSparse();
      if(vcycle) {
        std::vector<PartitionID>dummi = ind.getPartition();
        std::vector<PartitionID>dummi2 = ind.getPartition();
        std::vector<double>dummi3;
        
        EvoParameters ewo(dummi, dummi2, dummi3);
        partitioner.partition(_hypergraph, _config, ewo);
      }
       

      std::vector<HyperedgeID> one = printy;
      std::vector<HyperedgeID> two = ind.getCutEdges();
      std::vector<HyperedgeID> output_diff;
      std::sort(one.begin(), one.end());
      std::set_intersection(one.begin(),
				    one.end(),
				    two.begin(),
				    two.end(),
				    std::back_inserter(output_diff));
      std::cout << std::endl <<std::endl << "====================="<< std::endl <<std::endl;
      std::cout <<output_diff.size() << std::endl;
      for(int i = 0; i < output_diff.size(); ++i) {
        std::cout << output_diff[i] << ' ';
      }
      std::cout << std::endl <<std::endl << "====================="<< std::endl <<std::endl;
      
      return ind;
  }
  inline Individuum Population::stableNetCreate(unsigned pos, std::vector<HyperedgeID> &theEdges) {
    _hypergraph.reset();
    
    std::vector<PartitionID> partition = getIndividuum(pos).getPartition();
    setPartitionVector(_hypergraph, partition);
    Randomize::instance().shuffleVector(theEdges, theEdges.size());
    bool touchedA [_hypergraph.initialNumNodes()] = {};
    for(int i = 0; i < theEdges.size(); ++i) {
      bool touch = false;
      for(HypernodeID u : _hypergraph.pins(theEdges[i])) {
        if(touchedA[u]) {
          touch = true;
        }
      }
      if(!touch) {
        for(HypernodeID u :_hypergraph.pins(theEdges[i])) {
          touchedA[u] = true;
        } 
        forceBlock(theEdges[i]);
      }
    }    
    return createIndividuum(_hypergraph, _config);
  }
  
  inline void Population::forceBlock(HyperedgeID he) {
    int amount[_hypergraph.k()] = { };
    for(HypernodeID u : _hypergraph.nodes()) {
      amount[_hypergraph.partID(u)] += 1;
    }
    int smallestBlock = std::numeric_limits<int>::max();
    int smallestBlockValue = std::numeric_limits<int>::max();
    for(int i = 0; i < _hypergraph.k(); ++i) {
      if(amount[i] < smallestBlockValue) { 
        smallestBlock = i;
        smallestBlockValue = amount[i];
      }
    }
    for(HypernodeID u : _hypergraph.pins(he)) {
      std::cout << he << ' ';
      _hypergraph.changeNodePart(u, _hypergraph.partID(u), smallestBlock);
    }
  }

  inline Individuum Population::singleStableNet(Configuration &config) {
      _hypergraph.reset();
      Partitioner partitioner;
      std::vector<PartitionID>dummy;
      std::vector<PartitionID>dummy2;
      std::vector<double>dummy3;
      EvoParameters evo(dummy, dummy2, dummy3);
      std::vector<HyperedgeID> printy = partitioner.partition(_hypergraph, config, evo);
      Randomize::instance().shuffleVector(printy, printy.size());
      std::cout << std::endl << std::endl;
      for(int i = 0; i < printy.size(); ++i) {
        std::cout << printy[i] << ' '; 
      }
      std::cout << std::endl << std::endl;
      bool touched [_hypergraph.initialNumNodes()] = {};

      for(int i = 0; i < printy.size(); ++i) {
        bool touch = false;
        for(HypernodeID u : _hypergraph.pins(printy[i])) {
          if(touched[u]) {
            touch = true;
          }
        }

        if(!touch) {
          for(HypernodeID u :_hypergraph.pins(printy[i])) {
            touched[u] = true;
          } 
          forceBlock(printy[i]);
        }
        else {std::cout << "-"<<printy[i] << ' ';}
      }
      /*for(int i = 0; i < printy.size(); ++i) {
        std::cout << printy[i] << ' ';
      }*/
      Individuum ind = createIndividuum(_hypergraph, config);
      std::vector<HyperedgeID> one = printy;
      std::vector<HyperedgeID> two = ind.getCutEdges();
      std::vector<HyperedgeID> output_diff;
      std::sort(one.begin(), one.end());
      std::set_intersection(one.begin(),
				    one.end(),
				    two.begin(),
				    two.end(),
				    std::back_inserter(output_diff));
      std::cout << std::endl <<std::endl << "====================="<< std::endl <<std::endl;
      std::cout <<output_diff.size() << std::endl;
      for(int i = 0; i < output_diff.size(); ++i) {
        std::cout << output_diff[i] << ' ';
      }
      std::cout << std::endl <<std::endl << "====================="<< std::endl <<std::endl;
      bool insert = true;
      if(insert) {
        insertIndividuum(ind);
      }
      
      return ind;
  }

  inline void Population::readConfigFromFile(Configuration& config, bool cutRB) {
    po::variables_map cmd_vm;

    po::options_description ini_line_options;
    std::string filename;
    if(cutRB) {
      filename = "../../../config/cut_rb_alenex16.ini";
    }
    else {
      filename = "../../../config/km1_direct_alenex17.ini";
    } 
    std::ifstream file2(filename);
    if (!file2) {
      std::cerr << "Could not load config file at: " << "LOL THIS ONE IS HARDCODED" << std::endl;
      std::exit(-1);
    }
    po::store(po::parse_config_file(file2,ini_line_options, true), cmd_vm);
    po::notify(cmd_vm);
  }
  inline Individuum Population::crossCombine(Individuum &original,const Configuration &config, ICombine &comb) {
    std::cout << std::endl << std::endl << std::endl<< "==============================================" << std::endl << std::endl << std::endl;
    Configuration configTemp = config;
    switch(config.evolutionary.cc_objective) {
      case CrossCombineObjective::k : {
        int lowerbound = std::max(config.partition.k / 4, 2);
        int kFactor = Randomize::instance().getRandomInt(lowerbound, config.partition.k * 4);
        configTemp.partition.k = kFactor;
//break; //Do not want until experiments confirm
      }
      case CrossCombineObjective::epsilon : {
        float epsilonFactor = Randomize::instance().getRandomFloat(config.partition.epsilon, 0.25);
        configTemp.partition.epsilon = epsilonFactor;
        break;
      }
      case CrossCombineObjective::metric : {
        if(config.partition.objective == Objective::km1) {
          readConfigFromFile(configTemp, true);
          configTemp.partition.objective = Objective::cut;
        }
        else {
          readConfigFromFile(configTemp, false);
          configTemp.partition.objective = Objective::km1;
        }
        break;
      }
      case CrossCombineObjective::mode : {
        if(config.partition.mode == Mode::recursive_bisection){
          readConfigFromFile(configTemp, false);
          configTemp.partition.mode = Mode::direct_kway;
        }
        else {
          readConfigFromFile(configTemp, true);
          configTemp.partition.mode = Mode::recursive_bisection;
        }
        break;
      }
      case CrossCombineObjective::louvain : {

        Context con(config);

        std::vector<PartitionID> tempVect = detectCommunities(_hypergraph, con);

        /*for(unsigned i = 0; i < tempVect.size(); ++i) {

          std::cout << tempVect[i] << " ";
        }*/
        std::vector<HyperedgeID> dummy;
        
        Individuum indTempLouv(tempVect, dummy, dummy, std::numeric_limits<double>::max());
        return combine(original, indTempLouv, comb, true);
        break;
      }
    }
    std::cout << toString(configTemp) <<std::endl;
    _hypergraph.changeK(configTemp.partition.k);
    Individuum indTemp = generateIndividuum(configTemp, false);
    _hypergraph.changeK(config.partition.k);
    _hypergraph.reset();
    return combine(original, indTemp, comb, true);

  }

 
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
inline Individuum Population::combineIndividuumFromEdgeFrequency(unsigned pos1, unsigned pos2, Configuration &config, std::vector<double> &edgeFrequency) {
    Partitioner partitioner;
    

    EvoParameters evo(getIndividuum(pos1).getPartition(), getIndividuum(pos2).getPartition(), false, true, edgeFrequency);

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

   
   const double increment = 1.0;

   for(unsigned it = 0; it < positions.size(); ++it) {
    
     std::cout << "---------------" << std::endl;
     std::cout << it << ' ' << positions[it] << std::endl;
     
     Individuum current = getIndividuum(positions[it]);
     
     //current.printSparse();
     std::vector<HyperedgeID> cutEdges;

     if(_config.evolutionary.edge_strong_set) {

       cutEdges = current.getStrongCutEdges();

     }else {

       cutEdges = current.getCutEdges();

     }
     std::cout << std::endl;
     for(unsigned i = 0; i < cutEdges.size(); ++i) {
       HyperedgeID value = cutEdges[i];
       
       edges[value] = edges[value] + increment;
     }
     std::cout << std::endl;
   }
   std::cout << std::endl <<std::endl;
   for(unsigned it = 0; it < edges.size(); ++it) {
     if(edges[it] >= 0.8 * ((double) positions.size())) {
       std::cout << it << ' ' << edges[it] << " ; ";
     }
    
   }
std::cout << std::endl <<std::endl;
   return edges;
 }
inline std::vector<unsigned> Population::stableNet(double &amount,std::vector<unsigned> &positions) {
  std::vector<unsigned> retvect;
  //std::vector<unsigned> myVec;
  //for(unsigned i = 0; i < positions; i++ )
  //  myVec.push_back( i );


  std::vector<double> edge = edgeFrequency(positions);
  for(unsigned i = 0; i < edge.size(); i++){
    if(edge[i] >= amount * positions.size()) {
      
      retvect.push_back(i);
    }
  }
  ASSERT(retvect.size != 0);
  return retvect;

}
inline void Population::setPartitionVector(Hypergraph& hypergraph, const std::vector<PartitionID>& parent) {
	for (HypernodeID u : hypergraph.nodes()) {
		
		hypergraph.setNodePart(u, parent[u]);
	}
}
 inline Individuum Population::combine(Individuum &parent_1, Individuum &parent_2, ICombine &combinator, bool crossComb) {
      return combinator.combine(parent_1, parent_2, crossComb);
}
 inline Individuum Population::combine(std::size_t pos1, std::size_t pos2, ICombine &combinator, bool crossComb) {
   Individuum ind1 = getIndividuum(pos1);
   Individuum ind2 = getIndividuum(pos2);
   //TODO bound check
   return combine(ind1, ind2, combinator, crossComb);
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
    inline Individuum Population::generateIndividuum(Configuration &config, bool insert = true) {
      _hypergraph.reset();
      Partitioner partitioner;
      std::vector<PartitionID>dummy;
      std::vector<PartitionID>dummy2;
      std::vector<double>dummy3;
      EvoParameters evo(dummy, dummy2, dummy3);
      std::vector<HyperedgeID> printy = partitioner.partition(_hypergraph, config, evo);
      for(int i = 0; i < printy.size(); ++i) {
        std::cout << printy[i] << ' ';
      }
      Individuum ind = createIndividuum(_hypergraph, config);
      if(insert) {
        insertIndividuum(ind);
      }
      
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
      std::cout << "Best Posittion: " << bestPosition << std::endl;
       _hypergraph.reset();
       setPartitionVector(_hypergraph, _internalPopulation[bestPosition].getPartition());
    }
    inline unsigned Population::bestPosition() {
      std::size_t bestPosition = 0;
      HyperedgeWeight bestValue = INT_MAX;
       for(std::size_t i = 0; i < size(); i++) {
	 HyperedgeWeight result = _internalPopulation[i].getFitness();
	 if(result < bestValue) {
	   
	   bestPosition = i;
	   bestValue = result;
	 }

      }
      return bestPosition;
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
