#pragma once

#include <Rcpp.h>
#include <unordered_map>
#include <string>

class HashMap {
    std::unordered_map<std::string, Rcpp::RObject> data_;
  
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
    
    bool contains(const std::string &q) const { return data_.find(q) != data_.end(); }
    
    void insert(const std::string &k, Rcpp::RObject v) { data_[k] = v; }
    
    Rcpp::RObject get(const std::string &q) const {
        auto it = data_.find(q);
        if (it == data_.end()) {
            return R_NilValue;
        }
        return it->second;
    }
    
    void print() const {
        Rcpp::Rcout << "*Rcpp Hash Map*\n\n";
        if (data_.empty()) {
            Rcpp::Rcout << "<empty>\n";
            return;
        }
    
        for (const auto& kv : data_) {
            Rcpp::Rcout << "[[\"" << kv.first << "\"]]\n";
    
            // use R's print() to show the value nicely
            Rcpp::Function print("print");
            print(kv.second);
    
            Rcpp::Rcout << "\n";
        }
    }
    
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
