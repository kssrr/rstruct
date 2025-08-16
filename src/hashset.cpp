#include <Rcpp.h>
#include <unordered_set>

#include "hashset.h"

// Constructors

HashSet::HashSet() {}

HashSet::HashSet(const Rcpp::List &input) {
    R_xlen_t n = input.size();
    data_.reserve(n);

    for (R_xlen_t i = 0; i < n; i++) {
        data_.insert(Rcpp::RObject(input[i]));
    }
}

// scalar operations

bool HashSet::contains(const Rcpp::RObject &q) const {
    return data_.find(q) != data_.end();
}

void HashSet::insert(const Rcpp::RObject v) { data_.insert(v); }

void HashSet::remove(const Rcpp::RObject &v) { data_.erase(v); }

// bulk ops

Rcpp::LogicalVector HashSet::lookup(const Rcpp::List &Q) const {
    R_xlen_t n = Q.size();
    Rcpp::LogicalVector res(n);

    for (R_xlen_t i = 0; i < n; i++) {
        res[i] = data_.find(Q[i]) != data_.end();
    }

    // not returning a named vector here since the entries
    // can be arbitrary R-objects (cannot simply be coerced)
    // to character/names
    return res;
}

void HashSet::update(const Rcpp::List &V) {
    for (const auto &v : V) {
        data_.insert(Rcpp::RObject(v));
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
