# Benchmarks

## Setup

```r
library(rstruct)
library(r2r)
library(hash)
library(microbenchmark)
library(fastmap)

library(ggplot2)

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

# Hash maps

m_rstruct <- rstruct::hashmap(as.list(setNames(values, keys)))

m_r2r <- r2r::hashmap()
m_r2r[keys] <- values

m_hash <- hash::hash()
m_hash[keys] <- values
m_fastmap <- fastmap::fastmap()

for (i in seq_along(keys)) {
  m_fastmap$set(keys[i], values[i])
}

# Hash sets (only rstruct & r2r)

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
  `fastmap` = {
    m_fastmap$get(sample(keys, 1))
  },
  times = 1e3
)

random_lookup
#> Unit: microseconds
#>     expr    min      lq     mean  median      uq     max neval cld
#>  rstruct 30.315 36.0995 41.66674 38.7310 41.6125 856.104  1000 a  
#>      r2r 27.720 32.8780 37.53605 35.1035 37.5250 307.261  1000  b 
#>     hash 14.748 18.4100 22.27964 20.7575 22.5665 328.557  1000   c
#>  fastmap 13.818 16.8320 20.49452 19.3845 21.4940 140.817  1000   c
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
  `fastmap` = {
    m_fastmap$mget(keys)
  },
  times = 50
)

bulk_lookup
#> Unit: microseconds
#>     expr        min         lq        mean      median         uq        max neval  cld
#>  rstruct    519.882    670.962    777.4081    711.8475    774.145   1616.488    50 a   
#>      r2r 151023.863 186794.500 211503.9563 204688.5570 222825.869 341436.095    50  b  
#>     hash 105025.494 137638.709 156059.0240 152346.3065 168229.644 232997.247    50   c 
#>  fastmap  23327.795  28004.905  34033.9940  30370.3995  36015.858  78538.938    50    d
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

