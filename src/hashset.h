#pragma once

// [[Rcpp::depends(RApiSerialize)]]

/*
 * Shallow wrapper around `std::unordered_set` to build hash sets for use in R. The idea
 * is to provide a data structure with faster lookup than base R vectors for problems
 * where you care mostly about set membership, so you are fine with trading the flexibility
 * of vectors for the speed of a hash set.
 */

#include <Rcpp.h>
#include <RApiSerializeAPI.h>

#include <unordered_set>
#include <string>

#include "MurmurHash3.h"

// For generic R objects, we serialize them & then compute
// a 32 bit hash digest to store in a `std::unordered_set<uint32_t>`
// (as serialized objects can get rather large). Using a 32bit hash 
// digest is faster/easier than direct hashing of potentially very 
// large binary objects. Collision probability should be negligible 
// for hash sets of a reasonable size (if you need a hash set of 
// arbitrary R objects you should probably ask yourself wtf you are 
// doing anyways imo).

inline Rcpp::RawVector serializeRcpp(const Rcpp::RObject& obj) {
  SEXP res = R::serializeToRaw(obj, R_NilValue, R_NilValue);
  return Rcpp::RawVector(res);
}

uint32_t digestRObject(const Rcpp::RObject &obj) {
    Rcpp::RawVector bytes = serializeRcpp(obj);
    uint32_t out;
    MurmurHash3_x86_32(RAW(bytes), bytes.size(), 0, &out);
    return out;
}

// We need to define an "API" that controls how R types
// are mapped to C++ types. This is mostly because sometimes
// scalars of Rcpp types do not map directly to a C++ type.
// The API also controls the argument type for some methods; 
// this is neccessary for "generic" hash sets that can store 
// arbitrary R objects where we might get arbitrary R objects
// that do not correspond/are not mapped directly to a C++
// type.

template <typename RType, typename CppType>
struct Api {
  using ArgType = const CppType&;
  static CppType fromR(const typename RType::stored_type& x) { return x; }
  static CppType toKey(const CppType& x) { return x; }
};

// API Rcpp::CharacterVector -> std::string
template <>
struct Api<Rcpp::CharacterVector, std::string> {
  using ArgType = const std::string&;
  
  static std::string fromR(SEXP x) { return Rcpp::as<std::string>(x); }
  static std::string toKey(const std::string& x) { return x; }
};

// API Rcpp::List (holding arbitrary R objects) -> 32bit hash digest
template <>
struct Api<Rcpp::List, uint32_t> {
  using ArgType = const Rcpp::RObject&;
  
  static uint32_t fromR(SEXP x) { return digestRObject(x); }
  static uint32_t toKey(const Rcpp::RObject& x) { return digestRObject(x); }
};

// Hash Set template
//
// This implements all neccessary methods drawing on the 
// API we layed out above. 
template <typename RType, typename CppType>
class HashSet {
  using ElemApi = Api<RType, CppType>;
  
protected:
  std::unordered_set<CppType> data_;
  
public:
  HashSet() = default;
  
  HashSet(const RType &input) {
    R_xlen_t n = input.size();
    data_.reserve(n);
    for (R_xlen_t i = 0; i < n; i++)
      data_.insert(ElemApi::fromR(input[i]));
  }
  
  bool contains(typename ElemApi::ArgType q) const {
    return data_.find(ElemApi::toKey(q)) != data_.end();
  }
  
  void insert(typename ElemApi::ArgType v) {
    data_.insert(ElemApi::toKey(v));
  }
  
  void remove(const RType &V) {
    for (R_xlen_t i = 0; i < V.size(); i++)
      data_.erase(ElemApi::fromR(V[i]));
  }
  
  Rcpp::LogicalVector lookup(const RType &Q) const {
    R_xlen_t n = Q.size();
    Rcpp::LogicalVector res(n);
    for (R_xlen_t i = 0; i < n; i++)
      res[i] = data_.find(ElemApi::fromR(Q[i])) != data_.end();
    return res;
  }
  
  void update(const RType &V) {
    for (R_xlen_t i = 0; i < V.size(); i++)
      data_.insert(ElemApi::fromR(V[i]));
  }
  
  void print() const {
    Rcpp::Rcout << "C++ hash set with " << data_.size() << " elements.\n";
  }
};
