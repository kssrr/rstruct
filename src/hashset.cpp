// [[Rcpp::depends(RApiSerialize)]]

/*
 * There is no built-in way to hash Rcpp::RObj, but we need one to 
 * built a hashset that can hold arbitrary R objects. Here, we serialize an
 * R object & then compute a SHA256 hash digest of the object. 
 *
 */

#include <Rcpp.h>
#include <unordered_set>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <RApiSerializeAPI.h>

#include "hashset.h"

inline Rcpp::RawVector serializeRcpp(const Rcpp::RObject& obj) {
  SEXP res = R::serializeToRaw(obj, R_NilValue, R_NilValue);
  return Rcpp::RawVector(res);
}

std::string sha256Raw(Rcpp::RawVector data) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(RAW(data), data.size(), hash);
  
  // Convert hash bytes to hex string
  std::stringstream ss;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return ss.str();
}

std::string digestRObject(Rcpp::RObject obj) {
  Rcpp::RawVector serialized = serializeRcpp(obj);
  return sha256Raw(serialized);
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
