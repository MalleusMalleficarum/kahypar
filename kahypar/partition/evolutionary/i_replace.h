#pragma once
#include "individuum.h"
#include "replace_information.h"
namespace kahypar {

  class IReplace {
  public: 
  ReplaceInformation replace(const Individuum &in,std::vector<Individuum> &population) {
   return replaceImpl(in, population);
  }
  virtual ~IReplace() {
  }
  private:
  virtual ReplaceInformation replaceImpl(const Individuum &in,std::vector<Individuum> &population) = 0;
  };

}
