# Benchmarks

## Setup

```r
library(rstruct)
library(r2r)
library(hash)
library(microbenchmark)

set.seed(42)

# Benchmarks

N <- 1e4

chars <- c(letters, LETTERS, 0:9)
random_keys <- function(n) paste0(
  sample(chars, n, replace = TRUE),
  sample(chars, n, replace = TRUE),
  sample(chars, n, replace = TRUE),
  sample(chars, n, replace = TRUE),
  sample(chars, n, replace = TRUE)
)

set.seed(840)
keys <- random_keys(N)
values <- rnorm(N)

# hash maps:
m_rstruct <- rstruct::hashmap(as.list(setNames(values, keys)))

m_r2r <- r2r::hashmap()
m_r2r[keys] <- values

m_hash <- hash::hash()
m_hash[keys] <- values

# hash sets (only r2r & rstruct):
hs_rstruct <- rstruct::hashset(keys)
hs_r2r <- r2r::hashset(keys)
```

## Hash Maps

### Random individual lookup

Slightly slower but overall similar to `r2r` & `hash` (it's hard to beat base R due to overhead introduced by interfacing between R & C++):

```r
random_lookup <- microbenchmark(
  `rstruct` = {
    m_rstruct[[sample(keys, 1)]]
  },
  `r2r` = {
    m_r2r[[sample(keys, 1)]]
  },
  `hash` = {
    m_hash[[sample(keys, 1)]]
  },
  times = 1e3
)

random_lookup
#> Unit: microseconds
#>     expr    min      lq     mean  median      uq     max neval cld
#>  rstruct 31.873 38.5360 46.46948 41.2805 46.4955 788.382  1000 a  
#>      r2r 27.877 34.5465 41.62944 37.0495 41.7200 242.572  1000  b 
#>     hash 15.369 19.3085 24.89275 22.0195 25.4435 274.339  1000   c
```


### Bulk retrieval

This is what `rstruct` is optimized for; here it outperforms the other packages by orders of magnitude.

```r
bulk_lookup <- microbenchmark(
  `rstruct` = {
    m_rstruct[keys]
  },
  `r2r` = {
    m_r2r[keys]
  },
  `hash` = {
    m_hash[keys]
  },
  times = 50
)

bulk_lookup
#> Unit: microseconds
#>     expr        min         lq        mean     median         uq        max neval cld
#>  rstruct    368.383    463.613    538.8295    529.122    567.298   1567.334    50 a  
#>      r2r 145451.238 162715.731 179429.3425 174064.287 190938.734 327968.390    50  b 
#>     hash 105089.626 124270.546 131683.5944 131638.968 139596.639 177181.197    50   c
```

## Hash sets

`hash` has no implementation, so we only test `rstruct` against `r2r`.

### Random individual lookup

```r
random_lookup_hs <- microbenchmark(
  rstruct = {
    hs_rstruct[[sample(keys, 1)]]
  },
  r2r = {
    hs_r2r[[sample(keys, 1)]]
  },
  times = 1e3
)

random_lookup_hs
#> Unit: microseconds
#>     expr    min     lq     mean  median      uq      max neval cld
#>  rstruct 29.096 32.392 40.80372 33.5655 35.7250 1125.763  1000  a 
#>      r2r 19.364 21.671 26.25696 22.8875 24.5205  122.012  1000   b
```

### Bulk retrieval

```r
bulk_lookup_hs <- microbenchmark(
  rstruct = {
    hs_rstruct[keys]
  },
  r2r = {
    hs_r2r[keys]
  },
  times = 50
)
#> Unit: milliseconds
#>     expr       min        lq      mean    median        uq        max neval cld
#>  rstruct  1.024232  1.133172  1.460557  1.504055  1.644674   2.894455    50  a 
#>      r2r 81.494306 84.356345 93.083116 90.419758 96.557059 140.492647    50   b
```

