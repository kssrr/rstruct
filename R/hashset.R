#' @export
#' @method print Rcpp_rstruct_hashset
print.Rcpp_rstruct_hashset <- function(obj) {
  obj$print()
}

#' @export
#' @method [[ Rcpp_rstruct_hashset
`[[.Rcpp_rstruct_hashset` <- function(obj, key) {
  obj$contains(key)
}

#' @export
#' @method [ Rcpp_rstruct_hashset
`[.Rcpp_rstruct_hashset` <- function(obj, keys) {
  obj$lookup(keys)
}

#' @export
#' @method [[<- Rcpp_rstruct_hashset
`[[<-.Rcpp_rstruct_hashset` <- function(obj, old, new) {
  obj$remove(old)
  obj$insert(new)
  invisible(obj)
}

#' @export
hashset <- function(input = NULL) {
  if (is.null(input)) {
    return(new(rstruct_hashset))
  } 
  
  if (!is.list(input)) {
    stop("Input needs to be a list.")
  }
  
  new(rstruct_hashset, input)
}
