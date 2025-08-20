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

/*
 * For generic R objects, we serialize them & then compute a 32 bit hash digest to
 * store in a `std::unordered_set<uint32_t>` (as serialized objects can get rather large).
 * Using a 32bit hash digest is faster/easier than direct hashing of potentially very large
 * binary objects. Collision probability should be negligible for hash sets of a reasonable
 * size (if you need a hash set of arbitrary R objects you should probably ask yourself wtf
 * you are doing anyways imo).
 *
 */

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

// generic base class template for a hash set

template <typename RType, typename CppType>
class HashSet {
protected:
    std::unordered_set<CppType> data_;

public:
    HashSet() = default;

    HashSet(const RType &input) {
        R_xlen_t n = input.size();
        data_.reserve(n);
        for (R_xlen_t i = 0; i < n; i++)
            data_.insert(Rcpp::as<CppType>(input[i]));
    }

    bool contains(const CppType &q) const { return data_.find(q) != data_.end(); }
    void insert(const CppType &v) { data_.insert(v); }
    void remove(const CppType &v) { data_.erase(v); }

    Rcpp::LogicalVector lookup(const RType &Q) const {
        R_xlen_t n = Q.size();
        Rcpp::LogicalVector res(n);
        for (R_xlen_t i = 0; i < n; i++)
            res[i] = data_.find(Rcpp::as<CppType>(Q[i])) != data_.end();
        return res;
    }

    void update(const RType &V) {
        for (R_xlen_t i = 0; i < V.size(); i++)
            data_.insert(Rcpp::as<CppType>(V[i]));
    }

    void print() const {
        Rcpp::Rcout << "C++ hash set with " << data_.size() << " elements.\n";
    }
};

/*
 * Derived classes: hash sets for numeric & character vectors, and 
 * hash set for generic R objects.
 *
 */

// numeric vector maps to std::unordered_set<double>
class HashSetNum : public HashSet<Rcpp::NumericVector, double> {
public:
    using HashSet<Rcpp::NumericVector, double>::HashSet;
};

// character vector maps to std::unordered_set<std::string>
class HashSetChr : public HashSet<Rcpp::CharacterVector, std::string> {
public:
    using HashSet<Rcpp::CharacterVector, std::string>::HashSet;
};

class HashSetGeneric : public HashSet<Rcpp::List, uint32_t> {
public:
    HashSetGeneric() = default;

    HashSetGeneric(const Rcpp::List &input) {
        data_.reserve(input.size());
        for (R_xlen_t i = 0; i < input.size(); ++i)
            data_.insert(digestRObject(input[i]));
    }

    bool contains(const Rcpp::RObject &q) const {
        return data_.find(digestRObject(q)) != data_.end();
    }

    void insert(const Rcpp::RObject &v) {
        data_.insert(digestRObject(v));
    }

    void remove(const Rcpp::RObject &v) {
        data_.erase(digestRObject(v));
    }

    Rcpp::LogicalVector lookup(const Rcpp::List &Q) const {
        Rcpp::LogicalVector res(Q.size());
        for (R_xlen_t i = 0; i < Q.size(); i++)
            res[i] = data_.find(digestRObject(Q[i])) != data_.end();
        return res;
    }

    void update(const Rcpp::List &V) {
        for (R_xlen_t i = 0; i < V.size(); ++i)
            data_.insert(digestRObject(V[i]));
    }
};

