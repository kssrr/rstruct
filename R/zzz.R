.onLoad <- function(libname, pkgname) {
  Rcpp::loadModule("hashmap_module", what = TRUE)
}
