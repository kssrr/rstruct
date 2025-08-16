#pragma once

#include <Rcpp.h>
#include <unordered_set>

// There is no default way to std::hash an Rcpp::RObject,
// so we are doing pointer comparison.
// An Rcpp::Robject is a wrapper around SEXP, which is a C pointer
// to an R object in memory. Reinterpret that pointer as integer
// & use it as hash -> two objects are equal iff they’re the same SEXP in R’s memory.
//
// If I understand correctly this means e.g. 1:3 != c(1, 2, 3), but only if
// the objects are *actually* the same.

namespace std {
    template<>
    struct hash<Rcpp::RObject> {
        std::size_t operator()(const Rcpp::RObject& x) const {
            return reinterpret_cast<std::size_t>(SEXP(x));
        }
    };

    template<>
    struct equal_to<Rcpp::RObject> {
        bool operator()(const Rcpp::RObject& a, const Rcpp::RObject& b) const noexcept {
            return SEXP(a) == SEXP(b);
        }
    };
}

class HashSet {
    std::unordered_set<Rcpp::RObject> data_;
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
