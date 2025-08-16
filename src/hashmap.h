#pragma once

#include <Rcpp.h>
#include <unordered_map>
#include <string>

class HashMap {
    std::unordered_map<std::string, Rcpp::RObject> data_;
  
  public:
    HashMap();

    HashMap(const Rcpp::List &input);

    bool contains(const std::string &q) const;
    
    void insert(const std::string &k, Rcpp::RObject v);

    Rcpp::RObject get(const std::string &q) const;

    void print() const;

    // bulk operations

    void update(const Rcpp::List &input);

    Rcpp::LogicalVector lookup(const Rcpp::CharacterVector &Q) const;

    // bulk retrieval
    Rcpp::List retrieve(const Rcpp::CharacterVector &Q) const;
};
