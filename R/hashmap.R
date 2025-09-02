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

#' Hashmap
#' 
#' Create a new hashmap, either empty or initialized from a named list.
#' A hashmap is a data structure that stores key–value pairs, with
#' fast lookup, insertion, and deletion based on keys.
#'
#' @param input A named list used to initialize the hashmap. If \code{NULL}
#'   (the default), an empty hashmap is created. All elements of the list
#'   must be named, and names must be non-empty and non-\code{NA}.
#' @return An object of class \code{Rcpp_rstruct_hashmap}.
#'
#' @examples
#' hm <- rstruct::hashmap(
#'   list(
#'     "apple" = 1:3,
#'     "cherry" = "red"
#'   )
#' )
#' 
#' hm[["apple"]]
#' #> [1] 1 2 3
#' 
#' hm[c("apple", "cherry", "does not exist")]
#' #> $apple
#' #> [1] 1 2 3
#' #>
#' #> $cherry
#' #> [1] "red"
#' #>
#' #> $`does not exist`
#' #> NULL
#' 
#' # Arbitrary R objects as values:
#' hm[["banana"]] <- data.frame(a = 1:5, b = 6:10)
#'
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