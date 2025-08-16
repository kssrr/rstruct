# rstruct

Fast lookup data structures for R.

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

Performance on scalar operations is similar to [r2r](https://github.com/vgherard/r2r), but rstruct is significantly faster on bulk operations as these are implemented in C++ instead of R.
