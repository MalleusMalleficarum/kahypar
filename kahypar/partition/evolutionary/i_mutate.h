#pragma once
#include "individuum.h"
#include "kahypar/definitions.h"
namespace kahypar{
  class IMutate {
  public:
    Individuum mutate(Individuum &target) {
      return mutateImpl(target);
    }
    virtual ~IMutate(){
    }
  private:
    virtual Individuum mutateImpl(Individuum &target) = 0;
  };
}
