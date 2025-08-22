#' @title Hashset classes and methods
#' @description Constructors and methods for rstruct hashsets.
#' @name hashset

#' @exportClass rstruct_hashset
setClass("rstruct_hashset", contains = "VIRTUAL")

#' @export
setGeneric("hashset", function(x) standardGeneric("hashset"))
#' @export
setMethod("hashset", "numeric", function(x) new(rstruct_hashset_num, x))
#' @export
setMethod("hashset", "character", function(x) new(rstruct_hashset_chr, x))
#' @export
setMethod("hashset", "list", function(x) new(rstruct_hashset_generic, x))
#' @export
setMethod("hashset", "missing", function(x) {
  message(paste0(
    "Empty hashsets are initialized to store arbitrary R objects by default.\n",
    "This might not always be optimal in terms of performance."
  ))
  new(rstruct_hashset_generic)
})

#' @export
setMethod(
  "[", 
  signature(x = "rstruct_hashset", i = "ANY"),
  function(x, i, ...) {
    x$lookup(i)
  }
)

#' @export
setMethod(
  "[[", 
  signature(x = "rstruct_hashset", i = "ANY"),
  function(x, i, ...) {
    x$contains(i)
  }
)
