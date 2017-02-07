#pragma once
#include "individuum.h"
namespace kahypar {

  class ICombine {
  public: 
  Individuum combine(const Individuum &parent_1,const Individuum &parent_2) {
   return combineImpl(parent_1, parent_2);
  }
  virtual ~ICombine() {
  }
  private:
  virtual Individuum combineImpl(const Individuum &parent_1,const Individuum &parent_2) = 0;
  };

}
