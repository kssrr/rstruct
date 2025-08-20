#include <Rcpp.h>

#include "hashset.h"

using HashNum = HashSetNum;

RCPP_MODULE(HashSetNumModule) {
    using namespace Rcpp;

    class_<HashNum>("HashSetNum")
    .constructor()
    .constructor<NumericVector>()
    .method("contains", &HashNum::contains)
    .method("insert", &HashNum::insert)
    .method("remove", &HashNum::remove)
    .method("lookup", &HashNum::lookup)
    .method("update", &HashNum::update)
    .method("print", &HashNum::print)
    ;
}

using HashChr = HashSetChr;

RCPP_MODULE(HashSetChrModule) {
    using namespace Rcpp;

    class_<HashChr>("HashSetChr")
    .constructor()
    .constructor<CharacterVector>()
    .method("contains", &HashChr::contains)
    .method("insert", &HashChr::insert)
    .method("remove", &HashChr::remove)
    .method("lookup", &HashChr::lookup)
    .method("update", &HashChr::update)
    .method("print", &HashChr::print)
    ;
}

using HashGen = HashSetGeneric;

RCPP_MODULE(HashSetGenericModule) {
    using namespace Rcpp;

    class_<HashGen>("HashSetGeneric")
    .constructor()
    .constructor<List>()
    .method("contains", &HashGen::contains)
    .method("insert", &HashGen::insert)
    .method("remove", &HashGen::remove)
    .method("lookup", &HashGen::lookup)
    .method("update", &HashGen::update)
    .method("print", &HashGen::print)
    ;
}
