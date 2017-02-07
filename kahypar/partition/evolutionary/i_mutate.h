#pragma once
#include "individuum.h"
namespace kahypar{
  class IMutate {
  public:
    void mutate(Individuum &target) {
      mutateImpl(target);
    }
    virtual ~IMutate(){
    }
  private:
    virtual void mutateImpl(Individuum &target) = 0;
  };
}
