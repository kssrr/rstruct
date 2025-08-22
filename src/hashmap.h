#pragma once

#include <Rcpp.h>
#include <unordered_map>
#include <string>

class HashMap {
    std::unordered_map<std::string, SEXP> data_;
  
  public:
    HashMap() {}

    HashMap(const Rcpp::List &input) {
        Rcpp::CharacterVector names = input.names();
        if (names.size() != input.size()) {
            Rcpp::stop("All elements must be named");
        }
        data_.reserve(input.size());
        for (R_xlen_t i = 0; i < input.size(); ++i) {
            data_[Rcpp::as<std::string>(names[i])] = input[i];
        }
    }
    
    inline bool contains(const std::string &q) const { return data_.find(q) != data_.end(); }
    
    inline void insert(const std::string &k, SEXP v) { data_[k] = v; }
    
    inline SEXP get(const std::string &q) const {
        auto it = data_.find(q);
        if (it != data_.end()) return it->second;
        return R_NilValue;
    }
    
    void print() const { Rcpp::Rcout << "*Rcpp Hash Map*\n\n"; }
    
    // bulk operations
    
    // bulk insertion
    void update(const Rcpp::List &input) {
        Rcpp::CharacterVector names = input.names();
        size_t n = input.size();
        data_.reserve(data_.size() + n);
    
        for (size_t i = 0; i < n; i++) {
            data_[Rcpp::as<std::string>(names[i])] = input[i];
        }
    }
    
    // bulk lookup
    Rcpp::LogicalVector lookup(const Rcpp::CharacterVector &Q) const {
        size_t n = Q.size();
        Rcpp::LogicalVector res(n);
    
        for (size_t i = 0; i < n; i++) {
            res[i] = data_.find(Rcpp::as<std::string>(Q[i])) != data_.end();
        }
    
        res.attr("names") = Q; // return named vector
        return res;
    }
    
    // bulk retrieval
    Rcpp::List retrieve(const Rcpp::CharacterVector &Q) const {
        size_t n = Q.size();
        Rcpp::List res(n);
    
        for (size_t i = 0; i < n; i++) {
            res[i] = get(Rcpp::as<std::string>(Q[i]));
        }
    
        res.attr("names") = Q; // return named list
        return res;
    }
    
};
