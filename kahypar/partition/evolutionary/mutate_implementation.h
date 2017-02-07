#pragma once
#include "kahypar/definitions.h"
#include "kahypar/partition/partitioner.h"
namespace kahypar {
  class MutateBaseImplementation final : public IMutate{
  public:

  private:
    void mutateImpl(Individuum &target) override final {
      Partitioner::mutate(target);
    };

  };


}
