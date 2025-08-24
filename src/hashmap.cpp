#include <Rcpp.h>
#include <unordered_map>
#include <string>

#include "hashmap.h"

RCPP_MODULE(hashmap_module) {
    Rcpp::class_<HashMap>("rstruct_hashmap")
        .constructor()
        .constructor<Rcpp::List>()
        .method("contains", &HashMap::contains)
        .method("insert", &HashMap::insert)
        .method("get", &HashMap::get)
        .method("update", &HashMap::update)
        .method("lookup", &HashMap::lookup)
        .method("retrieve", &HashMap::retrieve)
        .method("print", &HashMap::print)
    ;
}
