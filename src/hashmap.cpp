#include <Rcpp.h>
#include <unordered_map>
#include <string>

#include "hashmap.h"

// Hash Map w/ std::string keys storing arbitrary R objects as values

HashMap::HashMap() {}

HashMap::HashMap(const Rcpp::List &input) {
    Rcpp::CharacterVector names = input.names();
    if (names.size() != input.size()) {
        Rcpp::stop("All elements must be named");
    }
    data_.reserve(input.size());
    for (R_xlen_t i = 0; i < input.size(); ++i) {
        data_[Rcpp::as<std::string>(names[i])] = input[i];
    }
}

bool HashMap::contains(const std::string &q) const { return data_.find(q) != data_.end(); }

void HashMap::insert(const std::string &k, Rcpp::RObject v) { data_[k] = v; }

Rcpp::RObject HashMap::get(const std::string &q) const {
    auto it = data_.find(q);
    if (it == data_.end()) {
        return R_NilValue;
    }
    return it->second;
}

void HashMap::print() const {
    Rcpp::Rcout << "*Rcpp Hash Map*\n\n";
    if (data_.empty()) {
        Rcpp::Rcout << "<empty>\n";
        return;
    }

    Rcpp::CharacterVector keys(data_.size());
    Rcpp::List values(data_.size());
    
    size_t i = 0;
    for (const auto& kv : data_) {
        keys[i] = kv.first;
        values[i] = kv.second;
        ++i;
    }
    
    values.attr("names") = keys;

    // dispatch to R print
    Rcpp::Function print("print");
    print(values);
}

// bulk operations

// bulk insertion
void HashMap::update(const Rcpp::List &input) {
    Rcpp::CharacterVector names = input.names();
    size_t n = input.size();
    data_.reserve(data_.size() + n);

    for (size_t i = 0; i < n; i++) {
        data_[Rcpp::as<std::string>(names[i])] = input[i];
    }
}

// bulk lookup
Rcpp::LogicalVector HashMap::lookup(const Rcpp::CharacterVector &Q) const {
    size_t n = Q.size();
    Rcpp::LogicalVector res(n);

    for (size_t i = 0; i < n; i++) {
        res[i] = data_.find(Rcpp::as<std::string>(Q[i])) != data_.end();
    }

    res.attr("names") = Q; // return named vector
    return res;
}

// bulk retrieval
Rcpp::List HashMap::retrieve(const Rcpp::CharacterVector &Q) const {
    size_t n = Q.size();
    Rcpp::List res(n);

    for (size_t i = 0; i < n; i++) {
        res[i] = get(Rcpp::as<std::string>(Q[i]));
    }

    res.attr("names") = Q; // return named list
    return res;
}

RCPP_MODULE(hashmap_module) {
    Rcpp::class_<HashMap>("rstruct_hashmap")
        .constructor()
        .constructor<Rcpp::List>()
        .method("contains", &HashMap::contains)
        .method("insert", &HashMap::insert)
        .method("get", &HashMap::get)
        .method("update", &HashMap::update)
        .method("lookup", &HashMap::lookup)
        .method("retrieve", &HashMap::retrieve)
        .method("print", &HashMap::print)
    ;
}
