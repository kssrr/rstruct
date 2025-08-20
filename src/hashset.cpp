// [[Rcpp::depends(RApiSerialize)]]

/*
 * To store arbitrary R objects in a hash set, we serialize them & then
 * compute a hash digest (larger R objects can become a rather large byte
 * sequence, so a digest is easier to store & hash for `std::unordered_set`).
 */

#include <Rcpp.h>
#include <unordered_set>
#include <RApiSerializeAPI.h>

#include "hashset.h"
#include "MurmurHash3.h"

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

// Constructors

HashSet::HashSet() {}

HashSet::HashSet(const Rcpp::List &input) {
    R_xlen_t n = input.size();
    data_.reserve(n);

    for (R_xlen_t i = 0; i < n; i++) {
        data_.insert(digestRObject(Rcpp::RObject(input[i])));
    }
}

// scalar operations

bool HashSet::contains(const Rcpp::RObject &q) const {
    return data_.find(digestRObject(q)) != data_.end();
}

void HashSet::insert(const Rcpp::RObject v) { data_.insert(digestRObject(v)); }

void HashSet::remove(const Rcpp::RObject &v) { data_.erase(digestRObject(v)); }

// bulk ops

Rcpp::LogicalVector HashSet::lookup(const Rcpp::List &Q) const {
    R_xlen_t n = Q.size();
    Rcpp::LogicalVector res(n);

    for (R_xlen_t i = 0; i < n; i++) {
        res[i] = data_.find(digestRObject(Q[i])) != data_.end();
    }

    // not returning a named vector here since the entries
    // can be arbitrary R-objects (cannot simply be coerced)
    // to character/names
    return res;
}

void HashSet::update(const Rcpp::List &V) {
    for (const auto &v : V) {
        data_.insert(digestRObject(Rcpp::RObject(v)));
    }
}

void HashSet::print() const {
    Rcpp::Rcout << "*Rcpp Hash Set*\n";

    if (data_.empty()) {
        Rcpp::Rcout <<"<empty>\n";
        return;
    }

    for (const auto &elem : data_) {
        Rcpp::Function print("print");
        print(elem);
        Rcpp::Rcout << "\n";
    }
}

// Expose as Rcpp Module:

RCPP_MODULE(hashset_module) {
    Rcpp::class_<HashSet>("rstruct_hashset")
        .constructor()
        .constructor<Rcpp::List>()
        .method("contains", &HashSet::contains)
        .method("insert", &HashSet::insert)
        .method("update", &HashSet::update)
        .method("lookup", &HashSet::lookup)
        .method("print", &HashSet::print)
    ;
}
