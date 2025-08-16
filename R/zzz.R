#' @useDynLib rstruct, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

.onLoad <- function(libname, pkgname) {
  Rcpp::loadModule("hashmap_module", what = TRUE, env = parent.env(environment()))
  Rcpp::loadModule("hashset_module", what = TRUE, env = parent.env(environment()))
}
