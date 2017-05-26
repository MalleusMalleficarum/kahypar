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
#include "kahypar/definitions.h"
namespace kahypar {
struct EvoParameters {
  EvoParameters(const std::vector<PartitionID> parent1, const std::vector<PartitionID> parent2, const std::vector<double> frequency, bool crossComb) :
    parent_1(parent1),
    parent_2(parent2),
    mutate(false),
    edgeFrequency(false),
    frequency(frequency),
    crossCombineIgnoreSecondPartition(crossComb) { }
  EvoParameters(const std::vector<PartitionID> parent1, const std::vector<PartitionID> parent2, bool mutate, bool edgeFrequency, const std::vector<double> frequency) :
    parent_1(parent1),
    parent_2(parent2),
    mutate(mutate),
    edgeFrequency(edgeFrequency),
    frequency(frequency),
    crossCombineIgnoreSecondPartition(false) { }
  EvoParameters(const std::vector<PartitionID> parent1, const std::vector<PartitionID> parent2, const std::vector<double> frequency) :
    parent_1(parent1),
    parent_2(parent2),
    mutate(false),
    edgeFrequency(false),
    frequency(frequency),
    crossCombineIgnoreSecondPartition(false) { }
  EvoParameters(const std::vector<PartitionID> parent1, const std::vector<PartitionID> parent2, bool mutate, bool edgeFrequency, const std::vector<double> frequency, bool crossComb) :
    parent_1(parent1),
    parent_2(parent2),
    mutate(mutate),
    edgeFrequency(edgeFrequency),
    frequency(frequency),
    crossCombineIgnoreSecondPartition(crossComb) { }


  void print() {
    std::cout << "parent1length " << parent_1.size() << ";parent2length " << parent_2.size() << ";mutate " << mutate << ";edgeFreq " << edgeFrequency << ";freqlength " << frequency.size();
  }

  const std::vector<PartitionID> parent_1;
  const std::vector<PartitionID> parent_2;
  bool mutate;
  bool edgeFrequency;
  const std::vector<double> frequency;
  bool crossCombineIgnoreSecondPartition;
  std::vector<unsigned> stableNetVector;
};
}
