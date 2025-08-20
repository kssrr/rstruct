#pragma once

#include <Rcpp.h>
#include <unordered_set>

class HashSet {
    std::unordered_set<uint32_t> data_;
  public:
    HashSet();
    HashSet(const Rcpp::List &input);
    bool contains(const Rcpp::RObject &q) const;
    void insert(const Rcpp::RObject v);
    void remove(const Rcpp::RObject &v);
    Rcpp::LogicalVector lookup(const Rcpp::List &Q) const;
    void update(const Rcpp::List &V);
    void print() const;
};
