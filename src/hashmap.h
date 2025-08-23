#pragma once
#include <Rcpp.h>

#include <unordered_map>

// Hash and equality for CHARSXP keys

// SEXPs are just SEXPREC*, essentially pointers to R objects,
// so we can use the standard pointer hash
struct SEXPHash {
  std::size_t operator()(SEXP s) const noexcept { return std::hash<const void *>()(reinterpret_cast<const void *>(s)); }
};

// Pointer equality is valid for CHARSXP: R maintains
// a global string pool, so if two CHARSXPs are the same
// string they will point to the same memory.
struct SEXPEqual {
  bool operator()(SEXP a, SEXP b) const noexcept { return a == b; }
};

// Hash Map

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
      SEXP key = names[i];
      data_[key] = input[i];
    }
  }

  inline bool contains(SEXP key) const {
    // chr vectors are STRSEXP, a vector like VECSXP
    // where each element is a CHARSXP, which is
    // what we are expecting as key. Since R has no concept
    // of scalars, we get a length-1 character vector (i.e.
    // the SEXP we get is a STRSEXP, but we want a CHARSXP).
    // Getting the first (only) element as CHARSXP:
    key = STRING_ELT(key, 0);
    return data_.find(key) != data_.end();
  }

  inline void insert(SEXP key, SEXP value) {
    key = STRING_ELT(key, 0);
    data_[key] = value;
  }

  inline SEXP get(SEXP key) const {
    key = STRING_ELT(key, 0);
    auto it = data_.find(key);
    if (it != data_.end()) return it->second;
    return R_NilValue;
  }

  void print() const {
    Rcpp::Rcout << "C++ Hash Map\n\n";
    for (const auto &kv : data_) {
      Rcpp::Rcout << "[[\"" << Rcpp::as<std::string>(kv.first) << "\"]]\n";
      Rf_PrintValue(kv.second);
      Rcpp::Rcout << "\n";
    }
  }

  void update(const Rcpp::List &input) {
    Rcpp::CharacterVector names = input.names();
    size_t n = input.size();
    data_.reserve(data_.size() + n);

    for (size_t i = 0; i < n; i++) {
      SEXP key = names[i];
      data_[key] = input[i];
    }
  }

  Rcpp::LogicalVector lookup(const Rcpp::CharacterVector &Q) const {
    size_t n = Q.size();
    Rcpp::LogicalVector res(n);

    for (size_t i = 0; i < n; i++) {
      SEXP key = Q[i];
      res[i] = data_.find(key) != data_.end();
    }

    res.attr("names") = Q;
    return res;
  }

  Rcpp::List retrieve(const Rcpp::CharacterVector &Q) const {
    size_t n = Q.size();
    Rcpp::List res(n);

    for (size_t i = 0; i < n; i++) {
      // cannot use `get` here because of the difference
      // between STRSXP & CHARSXP (if we access elements of
      // a CharacterVector, the result is already CHARSXP)
      SEXP key = Q[i];
      auto it = data_.find(key);
      res[i] = it != data_.end() ? it->second : R_NilValue;
    }

    res.attr("names") = Q;
    return res;
  }
};
