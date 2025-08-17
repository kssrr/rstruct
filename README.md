# rstruct

Fast lookup data structures for R. Instead of being based on R's built-in environment data structure, like the [hash](https://github.com/decisionpatterns/r-hash) & [r2r](https://github.com/vgherard/r2r) packages it wraps `std::unordered_map` and `std::unordered_set` from the C++ STL. 

Performance on scalar operations (random individual lookup) is similar to `hash` & `r2r`, but `rstruct` is significantly faster on bulk operations, as these are implemented in C++.

## Installation

The package is not on CRAN, but you can install it directly from GitHub using

```
devtools::install_github("kssrr/rstruct")
```

## Examples

Hash maps:

```
hm <- rstruct::hashmap(
  list(
    "apple" = 1:3,
    "cherry" = "red"
  )
)

hm[["apple"]]
#> [1] 1 2 3

hm[c("apple", "cherry", "does not exist")]
#> $apple
#> [1] 1 2 3
#>
#> $cherry
#> [1] "red"
#>
#> $`does not exist`
#> NULL
```

Keys have to be `character`, but can hold arbitrary R objects as values:

```
hm[["banana"]] <- data.frame(a = 1:5, b = 6:10)
```

Hash sets can store arbitrary R objects:

```
hs <- rstruct::hashset(
  list(
    "yellow",
    1:10,
    data.frame(a = 1:5, b = 6:10)
  )
)

hs[[1:10]]
#> [1] TRUE

hs[[data.frame(a = 1:5, b = 6:10)]]
#> [1] TRUE

hs[list("yellow", data.frame(a = 1:5, b = 6:10))]
#> [1] TRUE TRUE
```
