#' @export
#' @method print Rcpp_rstruct_hashmap
print.Rcpp_rstruct_hashmap <- function(obj) {
  obj$print()
}

#' @export
#' @method [[ Rcpp_rstruct_hashmap
`[[.Rcpp_rstruct_hashmap` <- function(obj, key) {
  obj$get(key)
}

#' @export
#' @method [ Rcpp_rstruct_hashmap
`[.Rcpp_rstruct_hashmap` <- function(obj, keys) {
  obj$retrieve(keys)
}

#' @export
#' @method [[<- Rcpp_rstruct_hashmap
`[[<-.Rcpp_rstruct_hashmap` <- function(obj, key, value) {
  obj$insert(key, value)
  invisible(obj)
}

#' @export
hashmap <- function(input = NULL) {
  if (is.null(input)) {
      return(new(rstruct_hashmap))
  } 

  if (!is.list(input) || any(is.na(names(input))) || any(names(input) == "")) {
    stop("Input needs to be a named list, or missing.")
  }
  
  new(rstruct_hashmap, input)
}
