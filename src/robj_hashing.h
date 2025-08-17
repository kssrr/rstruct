/*
 * There is no built-in way to hash Rcpp::RObj, but we need one to 
 * built a hashset that can hold arbitrary R objects. Here, we serialize an
 * R object & then compute a SHA256 hash digest of the object. 
 *
 */

#include <Rcpp.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>


// The serialization is suboptimal because we have to call
// into R; still need to find a lower-level way
// to serialize R objects (this does not seem to be exposed)
Rcpp::RawVector serializeRcpp(Rcpp::RObject obj) {
  Rcpp::Function serialize("serialize");
  return serialize(obj, R_NilValue);
}

Rcpp::RObject unserializeRcpp(Rcpp::RawVector data) {
  Rcpp::Function unserialize("unserialize");
  return unserialize(data);
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
