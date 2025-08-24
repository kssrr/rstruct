#include <Rcpp.h>
#include "hashset.h"

// The `HashSet`-template is defined in `hashset.h`

using HashSetNum     = HashSet<Rcpp::NumericVector,   double>;
using HashSetChr     = HashSet<Rcpp::CharacterVector, std::string>;
using HashSetGeneric = HashSet<Rcpp::List,            uint32_t>;

// Modules
RCPP_MODULE(HashSetNumModule) {
  using namespace Rcpp;
  
  class_<HashSetNum>("rstruct_hashset_num")
    .constructor()
    .constructor<NumericVector>()
    .method("contains", &HashSetNum::contains)
    .method("insert",   &HashSetNum::insert)
    .method("remove",   &HashSetNum::remove)
    .method("lookup",   &HashSetNum::lookup)
    .method("update",   &HashSetNum::update)
    .method("print",    &HashSetNum::print);
}

RCPP_MODULE(HashSetChrModule) {
  using namespace Rcpp;
  
  class_<HashSetChr>("rstruct_hashset_chr")
    .constructor()
    .constructor<CharacterVector>()
    .method("contains", &HashSetChr::contains)
    .method("insert",   &HashSetChr::insert)
    .method("remove",   &HashSetChr::remove)
    .method("lookup",   &HashSetChr::lookup)
    .method("update",   &HashSetChr::update)
    .method("print",    &HashSetChr::print);
}

RCPP_MODULE(HashSetGenericModule) {
  using namespace Rcpp;
  
  class_<HashSetGeneric>("rstruct_hashset_generic")
    .constructor()
    .constructor<List>()
    .method("contains", &HashSetGeneric::contains)
    .method("insert",   &HashSetGeneric::insert)
    .method("remove",   &HashSetGeneric::remove)
    .method("lookup",   &HashSetGeneric::lookup)
    .method("update",   &HashSetGeneric::update)
    .method("print",    &HashSetGeneric::print);
}
