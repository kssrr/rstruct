#' @exportClass rstruct_hashset
setClass("rstruct_hashset", contains = "VIRTUAL")

#' Hashset
#' 
#' @description
#' Create a new Hashset from numeric, character, list input, or an 
#' empty hashset if called with no arguments. Hashsets store unique 
#' values with fast lookup and insertion. Purely numeric or 
#' character hashsets are more optimized internally.
#'
#' @param x Input values. Can be a numeric vector, character vector, 
#'   list, or missing.
#' @return An object of class \code{rstruct_hashset}.
#' @examples
#' # If you construct from a list, the resulting hash set 
#' # will allow you to store arbitrary objects:
#' hs <- rstruct::hashset(
#'   list(
#'     "yellow",
#'     1:10,
#'     data.frame(a = 1:5, b = 6:10)
#'   )
#' )
#' 
#' hs[[1:10]]
#' #> [1] TRUE
#' 
#' hs[[data.frame(a = 1:5, b = 6:10)]]
#' #> [1] TRUE
#' 
#' hs[list("yellow", data.frame(a = 1:5, b = 6:10))]
#' #> [1] TRUE TRUE
#' 
#' # You can also initialize from character or numeric vectors,
#' # which has some performance benefits under the hood.
#' hs_chr <- rstruct::hashset(c("a", "b", "c"))
#' hs_num <- rstruct::hashset(1:10)
#' 
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