#ifndef LIB_DEFINITIONS_H_
#define LIB_DEFINITIONS_H_
#include "lib/datastructure/GenericHypergraph.h"

namespace defs {
typedef unsigned int hypernode_id_t;
typedef unsigned int hyperedge_id_t;
typedef unsigned int hypernode_weight_t;
typedef int hyperedge_weight_t;

typedef int partition_id_t;
const partition_id_t INVALID_PARTITION = -1;
typedef double RatingType;

typedef datastructure::GenericHypergraph<hypernode_id_t, hyperedge_id_t, hypernode_weight_t,
                                         hyperedge_weight_t, partition_id_t> Hypergraph;

typedef Hypergraph::HypernodeID HypernodeID;
typedef Hypergraph::HyperedgeID HyperedgeID;
typedef Hypergraph::PartitionID PartitionID;
typedef Hypergraph::HypernodeWeight HypernodeWeight;
typedef Hypergraph::HyperedgeWeight HyperedgeWeight;
typedef Hypergraph::Type HypergraphType;
typedef Hypergraph::HypernodeIterator HypernodeIterator;
typedef Hypergraph::HyperedgeIterator HyperedgeIterator;
typedef Hypergraph::IncidenceIterator IncidenceIterator;
typedef Hypergraph::HyperedgeIndexVector HyperedgeIndexVector;
typedef Hypergraph::HyperedgeVector HyperedgeVector;
typedef Hypergraph::HyperedgeWeightVector HyperedgeWeightVector;
typedef Hypergraph::HypernodeWeightVector HypernodeWeightVector;

} // namespace defs
#endif  // LIB_DEFINITIONS_H_
