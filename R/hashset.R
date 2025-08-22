#' @export
hashset <- function(input = NULL) {
  if (is.null(input) || is.list(input)) {
    # default to generic hash set if no input
    return(new(rstruct_hashset_generic))
  }
  
  if (is.numeric(input)) {
    return(new(rstruct_hashset_num, input))
  }
  
  if (is.character(input)) {
    return(new(rstruct_hashset_chr, input))
  }
  
  stop("Unsupported input type for hashset()")
}
