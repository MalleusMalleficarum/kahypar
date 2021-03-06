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

#include <boost/program_options.hpp>

#if defined(_MSC_VER)
#include <process.h>
#include <Windows.h>
#else
#include <sys/ioctl.h>
#endif

#include <cctype>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "kahypar/definitions.h"
#include "kahypar/io/hypergraph_io.h"
#include "kahypar/io/partitioning_output.h"
#include "kahypar/io/sql_plottools_serializer.h"
#include "kahypar/kahypar.h"
#include "kahypar/macros.h"
#include "kahypar/partition/diversifier.h"
#include "kahypar/partition/evolutionary/combine_implementation.h"
#include "kahypar/partition/evolutionary/individuum.h"
#include "kahypar/partition/evolutionary/mutate_implementation.h"
#include "kahypar/partition/evolutionary/population.h"
#include "kahypar/utils/math.h"
#include "kahypar/utils/randomize.h"

namespace po = boost::program_options;

using kahypar::HypernodeID;
using kahypar::HyperedgeID;
using kahypar::PartitionID;
using kahypar::Hypergraph;
using kahypar::HighResClockTimepoint;
using kahypar::Partitioner;
using kahypar::Configuration;
using kahypar::Mode;
using kahypar::Objective;
using kahypar::CoarseningAlgorithm;
using kahypar::RefinementAlgorithm;
using kahypar::InitialPartitionerAlgorithm;
using kahypar::RefinementStoppingRule;
using kahypar::GlobalRebalancingMode;
using kahypar::InitialPartitioningTechnique;
using kahypar::Population;
using kahypar::Individuum;
using kahypar::CombinatorBaseImplementation;
using kahypar::MutatorBaseImplementation;
using kahypar::diversifyer;
using Timepoint = HighResClockTimepoint;
using duration = std::chrono::duration<double>;
using timer = std::chrono::high_resolution_clock;
int getTerminalWidth() {
  int columns = 0;
#if defined(_MSC_VER)
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  columns = w.ws_col;
#endif
  return columns;
}

int getProcessID() {
#if defined(_MSC_VER)
  return _getpid();
#else
  return getpid();
  #endif
}

void checkRecursiveBisectionMode(RefinementAlgorithm& algo) {
  if (algo == RefinementAlgorithm::kway_fm) {
    std::cout << "WARNING: local search algorithm is set to "
    << toString(algo)
    << ". However " << toString(RefinementAlgorithm::twoway_fm)
    << "  is better and faster." << std::endl;
    std::cout << "Should the local search algorithm be changed to "
    << toString(RefinementAlgorithm::twoway_fm) << " (Y/N)?" << std::endl;
    char answer = 'N';
    std::cin >> answer;
    answer = std::toupper(answer);
    if (answer == 'Y') {
      std::cout << "Changing local search algorithm to "
      << toString(RefinementAlgorithm::twoway_fm) << std::endl;
      algo = RefinementAlgorithm::twoway_fm;
    }
  }
}

void checkDirectKwayMode(RefinementAlgorithm& algo) {
  if (algo == RefinementAlgorithm::twoway_fm) {
    std::cout << "WARNING: local search algorithm is set to "
    << toString(algo)
    << ". This algorithm cannot be used for direct k-way partitioning with k>2."
    << std::endl;
    std::cout << "Should the local search algorithm be changed to "
    << toString(RefinementAlgorithm::kway_fm) << " (Y/N)?" << std::endl;
    char answer = 'N';
    std::cin >> answer;
    answer = std::toupper(answer);
    if (answer == 'Y') {
      std::cout << "Changing local search algorithm to "
      << toString(RefinementAlgorithm::kway_fm) << std::endl;
      algo = RefinementAlgorithm::kway_fm;
    }
  }
}


void sanityCheck(Configuration& config) {
  switch (config.partition.mode) {
    case Mode::recursive_bisection:
      // Prevent configurations that currently don't make sense.
      // If KaHyPar operates in recursive bisection mode, than the
      // initial partitioning algorithm is called on the coarsest graph to
      // create a bipartition (i.e. it is only called for k=2).
      // Therefore, the initial partitioning algorithm
      // should run in direct mode and not in recursive bisection mode (since there
      // is nothing left to bisect).
      ALWAYS_ASSERT(config.initial_partitioning.mode == Mode::direct_kway,
                    toString(config.initial_partitioning.mode));
      // Furthermore, the IP algorithm should not use the multilevel technique itself,
      // because the hypergraph is already coarse enough for initial partitioning.
      // Therefore we prevent further coarsening by enforcing flat rather than multilevel.
      // If initial partitioning is set to direct k-way, it does not make sense to use
      // multilevel as initial partitioning technique, because in this case KaHyPar
      // could just do the additional multilevel coarsening and then call the initial
      // partitioning algorithm as a flat algorithm.
      ALWAYS_ASSERT(config.initial_partitioning.technique == InitialPartitioningTechnique::flat,
                    toString(config.initial_partitioning.technique));
      checkRecursiveBisectionMode(config.local_search.algorithm);
      break;
    case Mode::direct_kway:
      // When KaHyPar runs in direct k-way mode, it makes no sense to use the initial
      // partitioner in direct multilevel mode, because we could essentially get the same
      // behavior if we would just use a smaller contraction limit in the main partitioner.
      // Since the contraction limits for main and initial partitioner are specifically tuned,
      // we currently forbid this configuration.
      ALWAYS_ASSERT(config.initial_partitioning.mode != Mode::direct_kway ||
                    config.initial_partitioning.technique == InitialPartitioningTechnique::flat,
                    toString(config.initial_partitioning.mode)
                    << " " << toString(config.initial_partitioning.technique));
      checkDirectKwayMode(config.local_search.algorithm);
      break;
  }
  switch (config.initial_partitioning.mode) {
    case Mode::recursive_bisection:
      checkRecursiveBisectionMode(config.initial_partitioning.local_search.algorithm);
      break;
    case Mode::direct_kway:
      // If the main partitioner runs in recursive bisection mode, then the initial
      // partitioner running in direct mode can use two-way FM as a local search
      // algorithm because we only perform bisections.
      if (config.partition.mode != Mode::recursive_bisection) {
        checkDirectKwayMode(config.initial_partitioning.local_search.algorithm);
      }
      break;
  }
}

void readConfigFromFile(Configuration& config) {
  po::variables_map cmd_vm;

  po::options_description ini_line_options;

  std::ifstream file2("../../../config/cut_rb_alenex16.ini");
  if (!file2) {
    std::cerr << "Could not load config file at: " << "LOL THIS ONE IS HARDCODED" << std::endl;
    std::exit(-1);
  }
  po::store(po::parse_config_file(file2, ini_line_options, true), cmd_vm);
  po::notify(cmd_vm);
}
void processCommandLineInput(Configuration& config, int argc, char* argv[]) {
  const int num_columns = getTerminalWidth();

  po::options_description generic_options("Generic Options", num_columns);
  generic_options.add_options()
    ("help", "show help message")
    ("verbose,v", po::value<bool>(&config.partition.verbose_output)->value_name("<bool>"),
    "Verbose partitioning output");

  po::options_description required_options("Required Options", num_columns);
  required_options.add_options()
    ("hypergraph,h",
    po::value<std::string>(&config.partition.graph_filename)->value_name("<string>")->required(),
    "Hypergraph filename")
    ("blocks,k",
    po::value<PartitionID>(&config.partition.k)->value_name("<int>")->required()->notifier(
      [&](const PartitionID) {
    config.partition.rb_lower_k = 0;
    config.partition.rb_upper_k = config.partition.k - 1;
  }),
    "Number of blocks")
    ("epsilon,e",
    po::value<double>(&config.partition.epsilon)->value_name("<double>")->required(),
    "Imbalance parameter epsilon")
    ("objective,o",
    po::value<std::string>()->value_name("<string>")->required()->notifier([&](const std::string& s) {
    if (s == "cut") {
      config.partition.objective = Objective::cut;
    } else if (s == "km1") {
      config.partition.objective = Objective::km1;
    }
  }),
    "Objective: \n"
    " - cut : cut-net metric \n"
    " - km1 : (lambda-1) metric")
    ("mode,m",
    po::value<std::string>()->value_name("<string>")->required()->notifier(
      [&](const std::string& mode) {
    config.partition.mode = kahypar::modeFromString(mode);
  }),
    "Partitioning mode: \n"
    " - (recursive) bisection \n"
    " - (direct) k-way");

  std::string config_path;
  po::options_description preset_options("Preset Options", num_columns);
  preset_options.add_options()
    ("preset,p", po::value<std::string>(&config_path)->value_name("<string>"),
    "Configuration Presets:\n"
    " - direct_kway_km1_alenex17\n"
    " - rb_cut_alenex16\n"
    " - <path-to-custom-ini-file>");

  po::options_description general_options("General Options", num_columns);
  general_options.add_options()
    ("seed",
    po::value<int>(&config.partition.seed)->value_name("<int>"),
    "Seed for random number generator \n"
    "(default: -1)")
    ("cmaxnet",
    po::value<HyperedgeID>(&config.partition.hyperedge_size_threshold)->value_name("<int>")->notifier(
      [&](const HyperedgeID) {
    if (config.partition.hyperedge_size_threshold == -1) {
      config.partition.hyperedge_size_threshold = std::numeric_limits<HyperedgeID>::max();
    }
  }),
    "Hyperedges larger than cmaxnet are ignored during partitioning process. \n"
    "(default: -1, disabled)")
    ("vcycles",
    po::value<int>(&config.partition.global_search_iterations)->value_name("<int>"),
    "# V-cycle iterations for direct k-way partitioning \n"
    "(default: 0)");

  po::options_description preprocessing_options("Preprocessing Options", num_columns);
  preprocessing_options.add_options()
    ("p-use-sparsifier",
    po::value<bool>(&config.preprocessing.enable_min_hash_sparsifier)->value_name("<bool>"),
    "Use min-hash pin sparsifier before partitioning \n"
    "(default: false)")
    ("p-sparsifier-min-median-he-size",
    po::value<HypernodeID>(&config.preprocessing.min_hash_sparsifier.min_median_he_size)->value_name("<int>"),
    "Minimum median hyperedge size necessary for sparsifier application \n"
    "(default: 28)")
    ("p-sparsifier-max-hyperedge-size",
    po::value<uint32_t>(&config.preprocessing.min_hash_sparsifier.max_hyperedge_size)->value_name("<int>"),
    "Max hyperedge size allowed considered by sparsifier")
    ("p-sparsifier-max-cluster-size",
    po::value<uint32_t>(&config.preprocessing.min_hash_sparsifier.max_cluster_size)->value_name("<int>"),
    "Max cluster size which is built by sparsifier")
    ("p-sparsifier-min-cluster-size",
    po::value<uint32_t>(&config.preprocessing.min_hash_sparsifier.min_cluster_size)->value_name("<int>"),
    "Min cluster size which is built by sparsifier")
    ("p-sparsifier-num-hash-func",
    po::value<uint32_t>(&config.preprocessing.min_hash_sparsifier.num_hash_functions)->value_name("<int>"),
    "Number of hash functions")
    ("p-sparsifier-combined-num-hash-func",
    po::value<uint32_t>(&config.preprocessing.min_hash_sparsifier.combined_num_hash_functions)->value_name("<int>"),
    "Number of combined hash functions")
    ("p-parallel-net-removal",
    po::value<bool>(&config.preprocessing.remove_parallel_hes)->value_name("<bool>"),
    "Remove parallel hyperedges before partitioning \n"
    "(default: false)")
    ("p-large-net-removal",
    po::value<bool>(&config.preprocessing.remove_always_cut_hes)->value_name("<bool>"),
    "Remove hyperedges that will always be cut because"
    " of the weight of their pins \n"
    "(default: false)");

  po::options_description coarsening_options("Coarsening Options", num_columns);
  coarsening_options.add_options()
    ("c-type",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ctype) {
    config.coarsening.algorithm = kahypar::coarseningAlgorithmFromString(ctype);
  }),
    "Algorithm:\n"
    " - ml_style\n"
    " - heavy_full\n"
    " - heavy_lazy \n"
    "(default: ml_style)")
    ("c-s",
    po::value<double>(&config.coarsening.max_allowed_weight_multiplier)->value_name("<double>"),
    "The maximum weight of a vertex in the coarsest hypergraph H is:\n"
    "(s * w(H)) / (t * k)\n"
    "(default: 1)")
    ("c-t",
    po::value<HypernodeID>(&config.coarsening.contraction_limit_multiplier)->value_name("<int>"),
    "Coarsening stops when there are no more than t * k hypernodes left\n"
    "(default: 160)");


  po::options_description ip_options("Initial Partitioning Options", num_columns);
  ip_options.add_options()
    ("i-mode",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_mode) {
    config.initial_partitioning.mode = kahypar::modeFromString(ip_mode);
  }),
    "IP mode: \n"
    " - (recursive) bisection  \n"
    " - (direct) k-way\n"
    "(default: rb)")
    ("i-technique",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_technique) {
    config.initial_partitioning.technique =
      kahypar::inititalPartitioningTechniqueFromString(ip_technique);
  }),
    "IP Technique:\n"
    " - flat\n"
    " - (multi)level\n"
    "(default: multi)")
    ("i-algo",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_algo) {
    config.initial_partitioning.algo =
      kahypar::initialPartitioningAlgorithmFromString(ip_algo);
  }),
    "Algorithm used to create initial partition: pool (default)")
    ("i-c-type",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_ctype) {
    config.initial_partitioning.coarsening.algorithm =
      kahypar::coarseningAlgorithmFromString(ip_ctype);
  }),
    "IP Coarsening Algorithm:\n"
    " - ml_style\n"
    " - heavy_full\n"
    " - heavy_lazy \n"
    "(default: ml_style)")
    ("i-c-s",
    po::value<double>(&config.initial_partitioning.coarsening.max_allowed_weight_multiplier)->value_name("<double>"),
    "The maximum weight of a vertex in the coarsest hypergraph H is:\n"
    "(i-c-s * w(H)) / (i-c-t * k)\n"
    "(default: 1)")
    ("i-c-t",
    po::value<HypernodeID>(&config.initial_partitioning.coarsening.contraction_limit_multiplier)->value_name("<int>"),
    "IP coarsening stops when there are no more than i-c-t * k hypernodes left \n"
    "(default: 150)")
    ("i-runs",
    po::value<int>(&config.initial_partitioning.nruns)->value_name("<int>"),
    "# initial partition trials \n"
    "(default: 20)")
    ("i-r-type",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_rtype) {
    config.initial_partitioning.local_search.algorithm =
      kahypar::refinementAlgorithmFromString(ip_rtype);
  }),
    "IP Local Search Algorithm:\n"
    " - twoway_fm   : 2-way FM algorithm\n"
    " - kway_fm     : k-way FM algorithm\n"
    " - kway_fm_km1 : k-way FM algorithm optimizing km1 metric\n"
    " - sclap       : Size-constrained Label Propagation \n"
    "(default: twoway_fm)")
    ("i-r-fm-stop",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ip_stopfm) {
    config.initial_partitioning.local_search.fm.stopping_rule =
      kahypar::stoppingRuleFromString(ip_stopfm);
  }),
    "Stopping Rule for IP Local Search: \n"
    " - adaptive_opt: ALENEX'17 stopping rule \n"
    " - simple:       ALENEX'16 threshold based on i-r-i\n"
    "(default: simple)")
    ("i-r-fm-stop-i",
    po::value<int>(&config.initial_partitioning.local_search.fm.max_number_of_fruitless_moves)->value_name("<int>"),
    "Max. # fruitless moves before stopping local search \n"
    "(default: 50)")
    ("i-r-runs",
    po::value<int>(&config.initial_partitioning.local_search.iterations_per_level)->value_name("<int>")->notifier(
      [&](const int) {
    if (config.initial_partitioning.local_search.iterations_per_level == -1) {
      config.initial_partitioning.local_search.iterations_per_level =
        std::numeric_limits<int>::max();
    }
  }),
    "Max. # local search repetitions on each level \n"
    "(default:1, no limit:-1)");

  po::options_description refinement_options("Refinement Options", num_columns);
  refinement_options.add_options()
    ("r-type",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& rtype) {
    config.local_search.algorithm = kahypar::refinementAlgorithmFromString(rtype);
  }),
    "Local Search Algorithm:\n"
    " - twoway_fm   : 2-way FM algorithm\n"
    " - kway_fm     : k-way FM algorithm (cut) \n"
    " - kway_fm_km1 : k-way FM algorithm (km1)\n"
    " - sclap       : Size-constrained Label Propagation \n"
    "(default: twoway_fm)")
    ("r-runs",
    po::value<int>(&config.local_search.iterations_per_level)->value_name("<int>")->notifier(
      [&](const int) {
    if (config.local_search.iterations_per_level == -1) {
      config.local_search.iterations_per_level = std::numeric_limits<int>::max();
    }
  }),
    "Max. # local search repetitions on each level\n"
    "(default:1, no limit:-1)")
    ("r-sclap-runs",
    po::value<int>(&config.local_search.sclap.max_number_iterations)->value_name("<int>"),
    "Maximum # iterations for ScLaP-based refinement \n"
    "(default: -1 infinite)")
    ("r-fm-stop",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& stopfm) {
    config.local_search.fm.stopping_rule = kahypar::stoppingRuleFromString(stopfm);
  }),
    "Stopping Rule for Local Search: \n"
    " - adaptive_opt: ALENEX'17 stopping rule \n"
    " - adaptive1:    new nGP implementation \n"
    " - adaptive2:    original nGP implementation \n"
    " - simple:       threshold based on r-fm-stop-i \n"
    "(default: simple)")
    ("r-fm-stop-i",
    po::value<int>(&config.local_search.fm.max_number_of_fruitless_moves)->value_name("<int>"),
    "Max. # fruitless moves before stopping local search using simple stopping rule \n"
    "(default: 250)")
    ("r-fm-stop-alpha",
    po::value<double>(&config.local_search.fm.adaptive_stopping_alpha)->value_name("<double>"),
    "Parameter alpha for adaptive stopping rules \n"
    "(default: 1,infinity: -1)")
    ("r-fm-global-rebalancing",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool global_rebalancing) {
    if (global_rebalancing) {
      config.local_search.fm.global_rebalancing = GlobalRebalancingMode::on;
    } else {
      config.local_search.fm.global_rebalancing = GlobalRebalancingMode::off;
    }
  }),
    "Use global rebalancing PQs in twoway_fm \n"
    "(default: false)");

  po::options_description evolutionary_options("Evolutionary Options", num_columns);
  evolutionary_options.add_options()
    ("iteration-limit",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& iterationlimit) {
    config.evolutionary.iteration_limit = iterationlimit;
  }), "text")
    ("cross-combine-objective",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ccobj) {
    config.evolutionary.cc_objective = kahypar::crossCombineObjectiveFromString(ccobj);
  }),
    "Cross Combine Objective: \n"
    " - mode = Partition Mode \n"
    " - objective = Optimization Metric \n"
    " - epsilon = epsilon \n"
    " - k = k (and currently epsilon too) \n"
    "(default: k(+epsilon)")
    ("time-limit",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& timelimit) {
    config.evolutionary.time_limit = timelimit;
  }), "Text")
    ("edge-repeat",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& rep) {
    config.evolutionary.edge_repeat = rep;
  }), "Text")
    ("population-size",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& populationsize) {
    config.evolutionary.population_size = populationsize;
  }), "text")
    ("mutation-chance",
    po::value<float>()->value_name("<float>")->notifier(
      [&](const float& mutationchance) {
    config.evolutionary.mutation_chance = mutationchance;
  }), "TExT")
    ("cross-combine-chance",
    po::value<float>()->value_name("<float>")->notifier(
      [&](const float& ccchance) {
    config.evolutionary.cross_combine_chance = ccchance;
  }), "TExT")
    ("strong-set",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& strong_set) {
    config.evolutionary.strong_set = strong_set;
  }), "TExT")
    ("replace-diverse",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& replacediverse) {
    config.evolutionary.replace_diverse = replacediverse;
  }), "TExT")
    ("evo-verbose",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& verbose) {
    config.evolutionary.verbose = verbose;
  }), "TExT")
    ("edge-strong-set",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.edge_strong_set = ess;
  }), "TExT")
    ("edge-combine",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.use_edge_combine = ess;
  }), "TExT")
    ("best-positions",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& ess) {
    config.evolutionary.best_positions = ess;
  }), "TExT")
    ("combine-positions",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.combine_positions = ess;
  }), "TExT")
    ("random-positions",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& ess) {
    config.evolutionary.random_positions = ess;
  }), "TExT")
    ("stable-net",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.stable_net = ess;
  }), "TExT")
    ("stable-net-vcycle",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.stable_net_vcycle = ess;
  }), "TExT")
    ("population-stable-net",
    po::value<bool>()->value_name("<bool>")->notifier(
      [&](const bool& ess) {
    config.evolutionary.population_stable_net = ess;
  }), "TExT")
    ("diversify",
    po::value<int>()->value_name("<int>")->notifier(
      [&](const int& ess) {
    config.evolutionary.diversify = ess;
  }), "TExT")
    ("filename",
    po::value<std::string>()->value_name("<string>")->notifier(
      [&](const std::string& ess) {
    config.evolutionary.filename = ess;
  }), "TExT")
    ("fill-limit",
    po::value<unsigned>()->value_name("<unsigned>")->notifier(
      [&](const unsigned& filllimit) {
    config.evolutionary.fill_limit = filllimit;
  }), "TExt");
  po::options_description cmd_line_options;
  cmd_line_options.add(generic_options)
  .add(required_options)
  .add(preset_options)
  .add(general_options)
  .add(preprocessing_options)
  .add(coarsening_options)
  .add(ip_options)
  .add(refinement_options)
  .add(evolutionary_options);

  po::variables_map cmd_vm;
  po::store(po::parse_command_line(argc, argv, cmd_line_options), cmd_vm);

  // placing vm.count("help") here prevents required attributes raising an
  // error if only help was supplied
  if (cmd_vm.count("help")) {
    std::cout << cmd_line_options << "n";
    exit(0);
  }

  po::notify(cmd_vm);

  std::ifstream file(config_path.c_str());
  if (!file) {
    std::cerr << "Could not load config file at: " << config_path << std::endl;
    std::exit(-1);
  }

  po::options_description ini_line_options;
  ini_line_options.add(general_options)
  .add(preprocessing_options)
  .add(coarsening_options)
  .add(ip_options)
  .add(refinement_options)
  .add(evolutionary_options);

  po::store(po::parse_config_file(file, ini_line_options, true), cmd_vm);
  po::notify(cmd_vm);
  std::ifstream file2("../../../config/cut_rb_alenex16.ini");
  if (!file2) {
    std::cerr << "Could not load config file at: " << config_path << std::endl;
    std::exit(-1);
  }
  //po::store(po::parse_config_file(file2, true), cmd_vm);
  //po::notify(cmd_vm);
  std::string epsilon_str = std::to_string(config.partition.epsilon);
  epsilon_str.erase(epsilon_str.find_last_not_of('0') + 1, std::string::npos);

  config.partition.graph_partition_filename =
    config.partition.graph_filename
    + ".result"
    + ".part"
    + std::to_string(config.partition.k)
    + ".epsilon"
    + epsilon_str
    + ".seed"
    + std::to_string(config.partition.seed)
    + ".KaHyPar";
}

// TODO(robin): rename and move to io::partitioning_output
// TODO(robin): make all parameters const
// TODO(robin): use camelCase only for method names and classes - variables are
//              lower_case_with_underscore
// TODO(robin): use sqlplottools style and append standard partitioner output
void writeShitEvo(int i, std::string filename, std::chrono::duration<double> duration, Hypergraph& hypergraph, Configuration& config, double currentFitness, std::size_t parent1, std::size_t parent2, unsigned worstPos, double averageFitness, double best, bool mutation, bool edgeFrequency, bool crossCombine, int difference, std::chrono::duration<double> totalDuration) {
  std::size_t found = filename.find_last_of("/");
  std::string useThis = filename.substr(found + 1);
  std::ofstream out_file;

  out_file.open(std::string("../../../../results/") + std::string(config.evolutionary.filename), std::ios_base::app);
  /*  out_file << "RESULT" << " k=" << config.partition.k
           << " epsilon=" << config.partition.epsilon
       << " seed=" << config.partition.seed
       << " iteration=" << i
       << " duration=" << duration.count()
       << " parent1=" << parent1
       << " parent2=" << parent2
       << " replacePosition=" << worstPos
       << " averageFitness=" << averageFitness
       << " cut=" << kahypar::metrics::hyperedgeCut(hypergraph)
       << " SOED=" << kahypar::metrics::soed(hypergraph)
       << " km-1=" << currentFitness
       << " absorption=" << kahypar::metrics::absorption(hypergraph)
       << " best=" << best
       << std::endl;*/

  out_file << config.partition.k << " "
  << config.partition.epsilon << " "
  << config.partition.seed << " "
  << i << " "
  << duration.count() << " "
  << parent1 << " "
  << parent2 << " "
  << worstPos << " "
  << averageFitness << " "
  << kahypar::metrics::hyperedgeCut(hypergraph) << " "
  << kahypar::metrics::soed(hypergraph) << " "
  << currentFitness << " "
  << kahypar::metrics::absorption(hypergraph) << " "
  << best << " "
  << useThis << " "
  << kahypar::metrics::imbalance(hypergraph, config) << " "
  << mutation << " "
  << config.evolutionary.mutation_chance << " "
  << config.evolutionary.fill_limit << " "
  << config.evolutionary.time_limit << " "
  << config.evolutionary.population_size << " "
  << config.evolutionary.iteration_limit << " "
  << edgeFrequency << " "
  << config.evolutionary.strong_set << " "
  << config.evolutionary.edge_strong_set << " "
  << config.evolutionary.edgeFrequencyUsesWeight << " "
  << config.evolutionary.gamma << " "
  << config.evolutionary.use_edge_combine << " "
  << crossCombine << " "
  << kahypar::toString(config.evolutionary.cc_objective) << " "
  << config.evolutionary.cross_combine_chance << " "
  << difference << " "
  << config.evolutionary.stable_net << " "
  << config.evolutionary.stable_net_vcycle << " "
  << totalDuration.count() << " "
  << config.evolutionary.diversify
  //<< kahypar::metrics::imbalance(hypergraph, config.partition.k) << " "
  << std::endl;
  out_file.close();
}

int main(int argc, char* argv[]) {
  // TODO(robin): general todos:
  // [ ] refactor everything according to the structure we devised on the whiteboard
  // [ ] refactor variable and method names to adhere to naming convetions
  // [ ] improve const-correctness:
  //     - make function parameters const
  //     - make all local variables that don't change const
  // [ ] update uncrustify config
  // [ ] use make AnalyzeModifiedSources / make AnalyzeAllSources to enforce codestyle
  // [ ] fix all suggestions warnings output by AnalyzeModifiedSources
  // [ ] fix compiler warnings
  // [ ] remove i_combine, i_mutate, i_replace interfaces
  // [ ] remove dead code
  // [ ] unsigned -> size_t
  // [ ] refactor population methods
  // [ ] attache evo params to config


  Configuration config;
  Configuration config2;
  processCommandLineInput(config, argc, argv);
  sanityCheck(config);
  // readConfigFromFile(config2);

  // kahypar::io::printPartitionerConfiguration(config2);
  //exit(1);
  if (config.partition.global_search_iterations != 0) {
    std::cerr << "Coarsened does not check if HNs are in same part." << std::endl;
    std::cerr << "Therefore v-cycles are currently disabled." << std::endl;
    std::exit(-1);
  }

  std::string filename = config.partition.graph_filename;
  kahypar::Randomize::instance().setSeed(config.partition.seed);

  Hypergraph hypergraph(
    kahypar::io::createHypergraphFromFile(config.partition.graph_filename,
                                          config.partition.k));

  if (config.preprocessing.enable_min_hash_sparsifier) {
    // determine whether or not to apply the sparsifier
    std::vector<HypernodeID> he_sizes;
    he_sizes.reserve(hypergraph.currentNumEdges());
    for (auto he : hypergraph.edges()) {
      he_sizes.push_back(hypergraph.edgeSize(he));
    }
    std::sort(he_sizes.begin(), he_sizes.end());
    if (kahypar::math::median(he_sizes) >=
        config.preprocessing.min_hash_sparsifier.min_median_he_size) {
      config.preprocessing.min_hash_sparsifier.is_active = true;
    }
  }

  if (config.partition.verbose_output) {
    kahypar::io::printHypergraphInfo(hypergraph,
                                     config.partition.graph_filename.substr(
                                       config.partition.graph_filename.find_last_of("/") + 1));
  }


  /*unsigned const POPULATION_SIZE = config.evolutionary.population_size;
  float const MUTATION_CHANCE = config.evolutionary.mutation_chance;
  //unsigned const TOTAL_CHANCE = 10;
  unsigned const FILL_AMOUNT = config.evolutionary.fill_limit;
  double const TIME_LIMIT_SECONDS = config.evolutionary.time_limit;
  double const ITERATION_LIMIT = config.evolutionary.iteration_limit;*/
  //replaceStrategy
  //combineStrategy
  //mutateStrategy


  //clearFile(config.partition.graph_filename);

  // TODO(robin): move evolutionary code into partition::evo_partitioner.h
  // TODO(robin): include 'advanced repetitions config' for standard kahypar
  Partitioner partitioner;
  Population populus(hypergraph, config, config.evolutionary.population_size);
  double membaBest = DBL_MAX;

  // TODO(robin): look at mutate_implementation.h and
  // combine_implementation.h for detail
  CombinatorBaseImplementation comb(hypergraph, config);
  MutatorBaseImplementation mut(hypergraph, config);

  Timepoint start = timer::now();
  Timepoint currentTime = timer::now();
  duration iterationSeconds = currentTime - start;
  unsigned i = 1;
  // TODO(robin): what is n? use variable names that speak for themselves
  float n;
  duration elapsed_total = currentTime - start;

  while (i < (config.evolutionary.fill_limit + 1) && elapsed_total.count() <= config.evolutionary.time_limit) {    //INTENDED TO START THE ITERATION NUMBERS AT 1 instead of 0
    Timepoint startIteration = timer::now();
    // TODO(robin): Individuum --> Individual (and also the corresponding methods)
    // TODO(robin): populus --> population
    Individuum indi = populus.generateIndividuum(config);
    // indi.print();
    double currentFitness = indi.getFitness();
    if (currentFitness < membaBest) {
      membaBest = currentFitness;
    }
    Timepoint endIteration = timer::now();
    duration elapsed_secondsIteration = endIteration - startIteration;
    duration elapsed_total = endIteration - start;

    populus.printInfo();
    writeShitEvo(i, filename, elapsed_secondsIteration, hypergraph, config, currentFitness, 0, 0, i, populus.getAverageFitness(), membaBest, false, false, false, 0, iterationSeconds);

    ++i;
  }
  currentTime = timer::now();
  iterationSeconds = currentTime - start;


  while (iterationSeconds.count() <= config.evolutionary.time_limit) {
    if (i % config.evolutionary.diversify == 0) {
      if (config.evolutionary.diversify) {
        // TODO(robin): see diversifyer.h
        diversifyer div;
        div.diversify(config);
      }

      Timepoint startIteration = timer::now();
      std::size_t firstPos;
      std::size_t secondPos;
      n = kahypar::Randomize::instance().getRandomFloat(0, 1);
      double currentFitness;
      unsigned replacePosition;
      // TODO(robin): Why do you need these parameters? What do they actually do?
      // --> use enum class instead with 3 types of ops
      bool mutation;
      bool edgeFreqBool;
      bool crossCombine;
      int diff = -1;


      // TODO(robin): enhance readability:
      // use separate methods / free functions instead of comments
      // i.e. performMutation(.....), performCombine(.....)

      //Mutate
      // TODO(robin): move this to evo_partitioner.partition(.....)
      if (n >= (1 - config.evolutionary.mutation_chance)) {  //Trickery since [0,1) can roll a 0 whereas 1 -[0,1) never will
        firstPos = populus.getRandomExcept(populus.bestPosition());
        secondPos = firstPos;

        //indi.print();

        replacePosition = firstPos;
        mutation = true;
        edgeFreqBool = false;
        crossCombine = false;
        if (config.evolutionary.stable_net) {
          // TODO(robin): stableNet should be a free function in evo::mutation namespace
          Individuum indi = populus.stableNetStrategy(firstPos);
          populus.replace(indi, firstPos);
          currentFitness = indi.getFitness();
        } else {
          // TODO(robin): mutate should be a free function in evo::mutation namespace
          Individuum indi = populus.mutate(firstPos, mut);
          populus.replace(indi, firstPos);
          currentFitness = indi.getFitness();
        }
      }
      // TODO(robin): also enhance readability here by extracting combine and cross combine
      // code in separate methods
      //Combine
      else {
        // TODO(robin): rename getTwoIndividuumTournamentPosition to sth like
        // tournametSelect(....)
        std::pair<unsigned, unsigned> tournamentWinners = populus.getTwoIndividuumTournamentPosition();
        float cc_roll = kahypar::Randomize::instance().getRandomFloat(0, 1);
        if (cc_roll >= (1 - config.evolutionary.cross_combine_chance)) {
          // TODO(robin): do these parameters actually do something?
          crossCombine = true;
          mutation = false;
          edgeFreqBool = false;
          // =======================================================
          // TODO(robin): crossCombine should be a free function in evo::combine namespace
          Individuum indi = populus.crossCombine(populus.getIndividuum(tournamentWinners.first), config, comb);
          firstPos = tournamentWinners.first;
          secondPos = tournamentWinners.second;
          currentFitness = indi.getFitness();
          replacePosition = populus.replaceStrategy(indi);
        } else {
          crossCombine = false;
          if (config.evolutionary.use_edge_combine) {
            edgeFreqBool = true;
            std::vector<unsigned> positions;
            if (config.evolutionary.best_positions > 0) {
              std::vector<unsigned> bestPos = populus.bestPositions(config.evolutionary.best_positions);
              // TODO(robin):  this can be done by copy assignment: positions = best_positions
              positions.insert(positions.end(), bestPos.begin(), bestPos.end());
            }
            // TODO(robin): ???? remove dead code
            if (config.evolutionary.combine_positions) {
              positions.push_back(tournamentWinners.first);
              positions.push_back(tournamentWinners.second);
            }
            if (config.evolutionary.random_positions > 0) {
              for (int i = 0; i < config.evolutionary.random_positions; ++i) {
                positions.push_back(kahypar::Randomize::instance().getRandomInt(0, populus.size()));
              }
            }

            std::vector<double> edgeFreq = populus.edgeFrequency(positions);
            // TODO(robin): edgeFrequencyCombine should be a free function in evo::combine namespace
            Individuum indi = populus.combineIndividuumFromEdgeFrequency(tournamentWinners.first, tournamentWinners.second, config, edgeFreq);

            firstPos = tournamentWinners.first;
            secondPos = tournamentWinners.second;
            // TODO(robin): hide difference within replaceStrategy method
            diff = populus.difference(populus.getIndividuum(tournamentWinners.first), tournamentWinners.second);
            // TODO(robin): rename replaceStrategy to sth. more meaningful
            replacePosition = populus.replaceStrategy(indi);
            currentFitness = indi.getFitness();
            mutation = false;
          } else {
            // TODO(robin): due to all of these if-elses its hard to see what we are actually doing here
            // ---> increase readability be refactoring this into a separate function
            edgeFreqBool = false;
            // TODO(robin): combine should be a free function in evo::combine namespace
            Individuum indi = populus.combine(tournamentWinners.first, tournamentWinners.second, comb);
            firstPos = tournamentWinners.first;
            secondPos = tournamentWinners.second;
            // TODO(robin): see comments above for replace Strategy and difference
            replacePosition = populus.replaceStrategy(indi);
            diff = populus.difference(populus.getIndividuum(tournamentWinners.first), tournamentWinners.second);
            currentFitness = indi.getFitness();
            mutation = false;
          }
        }
        //indi.print();
      }


      if (currentFitness < membaBest) {
        membaBest = currentFitness;
      }


      Timepoint endIteration = timer::now();
      duration elapsed_secondsIteration = endIteration - startIteration;
      duration elapsed_total = endIteration - start;
      writeShitEvo(i, filename, elapsed_secondsIteration, hypergraph, config, currentFitness, firstPos, secondPos, replacePosition, populus.getAverageFitness(), membaBest, mutation, edgeFreqBool, crossCombine, diff, elapsed_total);


      // TODO(robin): ???????
      //EdgeFrequency Without anything special
      // config.evolutionary.edge_repeat --> config.evolutionary.edge_frequency_repeat
      if (i % config.evolutionary.edge_repeat == 0) {
        std::vector<unsigned> pos = populus.bestPositions(config.evolutionary.best_positions);

        std::vector<double> edgeFreq = populus.edgeFrequency(pos);

        Individuum indi = populus.individuumFromEdgeFrequency(config, edgeFreq);
        currentFitness = indi.getFitness();
        if (currentFitness < membaBest) {
          membaBest = currentFitness;
        }
        // TODO(robin): see comments above for replace Strategy and difference
        unsigned replacePos = populus.replaceStrategy(indi, false);
        elapsed_total = endIteration - start;
        writeShitEvo(i, filename, elapsed_secondsIteration, hypergraph, config, currentFitness, (int)sqrt(config.evolutionary.population_size), (int)sqrt(config.evolutionary.population_size), replacePos, populus.getAverageFitness(), membaBest, false, true, false, 0, elapsed_total);
      }
      currentTime = timer::now();
      iterationSeconds = currentTime - start;
      i++;
    }


    Timepoint end = timer::now();
    duration elapsed_seconds = end - start;
    //Individuum ind = populus.singleStableNet(config);
    //std::cout <<  kahypar::metrics::imbalance(hypergraph,config);
    populus.printInfo();

    populus.setTheBest();
#ifdef GATHER_STATS
    LOG("*******************************");
    LOG("***** GATHER_STATS ACTIVE *****");
    LOG("*******************************");
    kahypar::io::printPartitioningStatistics();
#endif

    kahypar::io::printPartitioningResults(hypergraph, config, elapsed_seconds);
    kahypar::io::writePartitionFile(hypergraph,
                                    config.partition.graph_partition_filename);

    kahypar::io::serializer::serialize(config, hypergraph, partitioner, elapsed_seconds);
    return 0;
  }
