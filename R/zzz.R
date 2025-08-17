#' @useDynLib rstruct, .registration = TRUE
#' @importFrom Rcpp sourceCpp
#' @import RApiSerialize
NULL

.onLoad <- function(libname, pkgname) {
  requireNamespace("RApiSerialize")
  Rcpp::loadModule("hashmap_module", what = TRUE, env = parent.env(environment()))
  Rcpp::loadModule("hashset_module", what = TRUE, env = parent.env(environment()))
}
