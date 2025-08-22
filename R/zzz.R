#' @useDynLib rstruct, .registration = TRUE
#' @importFrom Rcpp sourceCpp
#' @import RApiSerialize
NULL

.onLoad <- function(libname, pkgname) {
  requireNamespace("RApiSerialize")
  Rcpp::loadModule("hashmap_module", what = TRUE, env = parent.env(environment()))
  Rcpp::loadModule("HashSetNumModule", what = TRUE, env = parent.env(environment()))
  Rcpp::loadModule("HashSetChrModule", what = TRUE, env = parent.env(environment()))
  Rcpp::loadModule("HashSetGenericModule", what = TRUE, env = parent.env(environment()))

  methods::setIs("Rcpp_rstruct_hashset_generic", "rstruct_hashset")
  methods::setIs("Rcpp_rstruct_hashset_num", "rstruct_hashset")
  methods::setIs("Rcpp_rstruct_hashset_chr", "rstruct_hashset")
}
