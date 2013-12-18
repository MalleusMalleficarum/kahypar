#ifndef PARTITION_METRICS_H_
#define PARTITION_METRICS_H_

#include <cmath>

#include "../lib/datastructure/Hypergraph.h"
#include "../lib/definitions.h"

namespace metrics {
using datastructure::HyperedgeWeight;
using datastructure::IncidenceIterator;
using defs::PartitionID;

template <class Hypergraph>
HyperedgeWeight hyperedgeCut(const Hypergraph& hg) {
  HyperedgeWeight cut = 0;
  forall_hyperedges(he, hg) {
    IncidenceIterator begin, end;
    std::tie(begin, end) = hg.pins(*he);
    if (begin == end) {
      continue;
    }
    ASSERT(begin != end, "Accessing empty hyperedge");

    PartitionID partition = hg.partitionIndex(*begin);
    ++begin;

    for (IncidenceIterator pin_it = begin; pin_it != end; ++pin_it) {
      if (partition != hg.partitionIndex(*pin_it)) {
        //PRINT("** Hyperedge " << *he << " is cut-edge");
        cut += hg.edgeWeight(*he);
        break;
      }
    }    
  } endfor
  return cut;
}

template <class Hypergraph>
double imbalance(const Hypergraph& hypergraph) {
  typedef typename Hypergraph::HypernodeWeight HypernodeWeight;
  std::vector<HypernodeWeight> partition_sizes(2, 0);
  HypernodeWeight total_weight = 0;
  forall_hypernodes(hn, hypergraph) {
    ASSERT(hypergraph.partitionIndex(*hn) < 2, "Invalid partition index for hypernode " << *hn);
    partition_sizes[hypergraph.partitionIndex(*hn)] += hypergraph.nodeWeight(*hn);
    total_weight += hypergraph.nodeWeight(*hn);
  } endfor

  ASSERT(total_weight == hypergraph.numNodes(),
         "Size of both partitions does not match number of nodes");
  HypernodeWeight max_weight = std::max(partition_sizes[0], partition_sizes[1]);
  return 2.0 * max_weight / total_weight - 1.0;
}

} // namespace metrics

#endif  // PARTITION_METRICS_H_