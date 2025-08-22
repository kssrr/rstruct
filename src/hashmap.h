#pragma once
#include <Rcpp.h>
#include <unordered_map>

// Hash and equality for CHARSXP keys
struct SEXPHash {
    std::size_t operator()(SEXP s) const noexcept {
        return std::hash<const void*>()(reinterpret_cast<const void*>(s));
    }
};

struct SEXPEqual {
    bool operator()(SEXP a, SEXP b) const noexcept {
        return a == b; // pointer equality is valid for CHARSXP
    }
};

class HashMap {
    std::unordered_map<SEXP, SEXP, SEXPHash, SEXPEqual> data_;
  
  public:
    HashMap() {}

    HashMap(const Rcpp::List &input) {
        Rcpp::CharacterVector names = input.names();
        if (names.size() != input.size()) {
            Rcpp::stop("All elements must be named");
        }
        data_.reserve(input.size());
        for (R_xlen_t i = 0; i < input.size(); ++i) {
            SEXP key = names[i]; // already a CHARSXP
            data_[key] = input[i];
        }
    }
    
    inline bool contains(SEXP key) const {
        return data_.find(key) != data_.end();
    }
    
    inline void insert(SEXP key, SEXP value) {
        data_[key] = value;
    }
    
    inline SEXP get(SEXP key) const {
        // chr vectors are STRSEXP, a vector like VECSXP
        // where each element is a CHARSXP, which is
        // what we are expecting as key. Since R has no concept
        // of scalars, we get a lenght-1 character vector.
        // Getting the first (only) element as CHARSXP:
        key = STRING_ELT(key, 0);
        auto it = data_.find(key);
        if (it != data_.end()) return it->second;
        return R_NilValue;
    }
    
    void print() const { Rcpp::Rcout << "*Rcpp Hash Map*\n\n"; }
    
    // --- bulk insertion ---
    void update(const Rcpp::List &input) {
        Rcpp::CharacterVector names = input.names();
        size_t n = input.size();
        data_.reserve(data_.size() + n);
    
        for (size_t i = 0; i < n; i++) {
            SEXP key = names[i]; 
            data_[key] = input[i];
        }
    }
    
    // --- bulk lookup ---
    Rcpp::LogicalVector lookup(const Rcpp::CharacterVector &Q) const {
        size_t n = Q.size();
        Rcpp::LogicalVector res(n);
    
        for (size_t i = 0; i < n; i++) {
            SEXP key = Q[i];
            res[i] = contains(key);
        }
    
        res.attr("names") = Q; 
        return res;
    }
    
    // --- bulk retrieval ---
    Rcpp::List retrieve(const Rcpp::CharacterVector &Q) const {
        size_t n = Q.size();
        Rcpp::List res(n);
    
        for (size_t i = 0; i < n; i++) {
            // cannot use `get` here because of the difference
            // between STRSXP & CHARSXP.
            SEXP key = Q[i];
            auto it = data_.find(key);
            res[i] = it != data_.end() ? it->second : R_NilValue;
        }
    
        res.attr("names") = Q;
        return res;
    }
};

