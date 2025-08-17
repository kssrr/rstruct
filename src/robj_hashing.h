// [[Rcpp::depends(RApiSerialize)]]

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
#include <RApiSerializeAPI.h>

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

