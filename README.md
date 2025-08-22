# rstruct

Fast lookup data structures for R (hash maps/"dictionaries" & hash sets) with average constant-time lookup, optimized for high-performance key-based access at scale.

Instead of being based on R's built-in environment data structure, like the [hash](https://github.com/decisionpatterns/r-hash) & [r2r](https://github.com/vgherard/r2r) packages, it wraps `std::unordered_map` and `std::unordered_set` from the C++ STL. 

Performance on scalar operations (random individual lookup) is similar to `hash` & `r2r`, but `rstruct` is significantly faster on bulk operations, as these are implemented in C++.

## Installation

The package is not on CRAN, but you can install it directly from GitHub using

```
devtools::install_github("kssrr/rstruct")
```

## Examples

### Hash maps

You can create hashmaps from named lists. Keys have to be `character` (some string), but values can be arbitrary R objects. Access with `[` and `[[` works similar as it does in lists. `[[` returns the object at the queried key, `[` returns a named list (where the names are the queried keys).

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

# Arbitrary R objects as values:
hm[["banana"]] <- data.frame(a = 1:5, b = 6:10)
```

### Hash Sets

Hash sets can store arbitrary R objects. There are optimizations for sets which are purely `numeric` or `character` (which `rstruct` will detect & apply automatically).

```
# If you construct from a list, the resulting hash set 
# will allow you to store arbitrary objects:
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

# You can also initialize from character or numeric vectors,
# which has some performance benefits under the hood.
hs_chr <- rstruct::hashset(c("a", "b", "c"))
hs_num <- rstruct::hashset(1:10)
```

## Motivation

There are existing packages (mentioned above), which build on R's `environment`-data structure, which is implemented internally as a hash table (see [here](https://cran.r-project.org/doc/manuals/r-release/R-ints.html#Hash-table)). However, for bulk operations (lookup, retrieval, insertion), those implementations rely on R's built-in methods of iterations (either loops or functionals, like `sapply`) which introduces massive overhead (`rstruct` is orders of magnitude faster on those operations).
