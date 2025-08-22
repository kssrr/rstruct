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
#>  rstruct 29.765 31.5555 37.67056 32.7240 34.6510 537.592  1000 a  
#>      r2r 26.254 28.4800 33.80781 29.5885 31.6145 116.347  1000  b 
#>     hash 14.081 15.6025 19.11052 16.3320 17.4685 376.473  1000   c
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
#>     expr       min         lq        mean     median         uq        max neval cld
#>  rstruct    343.99    444.808    606.0201    538.762    591.548   4013.246    50 a  
#>      r2r 145245.49 161387.589 178020.9954 168853.045 180884.461 309141.976    50  b 
#>     hash 100912.44 117251.384 133593.0540 125494.804 137523.699 271285.365    50   c
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

