function (object, algo = c("md5", "sha1", "crc32", "sha256", 
    "sha512", "xxhash32", "xxhash64", "murmur32", "spookyhash", 
    "blake3", "crc32c"), serialize = TRUE, file = FALSE, length = Inf, 
    skip = "auto", ascii = FALSE, raw = FALSE, seed = 0, errormode = c("stop", 
        "warn", "silent"), serializeVersion = .getSerializeVersion()) 
{
    algo <- match.arg(algo, c("md5", "sha1", "crc32", "sha256", 
        "sha512", "xxhash32", "xxhash64", "murmur32", "spookyhash", 
        "blake3", "crc32c"))
    errormode <- match.arg(errormode, c("stop", "warn", "silent"))
    if (is.infinite(length)) {
        length <- -1
    }
    if (is.character(file) && missing(object)) {
        object <- file
        file <- TRUE
    }
    is_streaming_algo <- algo == "spookyhash"
    if (is_streaming_algo && !serialize) {
        .errorhandler(paste0(algo, " algorithm is not available without serialization."), 
            mode = errormode)
    }
    if (serialize && !file) {
        if (!is_streaming_algo) {
            object <- if (.hasNoSharing()) 
                serialize(object, connection = NULL, ascii = ascii, 
                  nosharing = TRUE, version = serializeVersion)
            else serialize(object, connection = NULL, ascii = ascii, 
                version = serializeVersion)
        }
        if (is.character(skip) && skip == "auto") 
            skip <- set_skip(object, ascii)
    }
    else if (!is.character(object) && !inherits(object, "raw") && 
        !is_streaming_algo) {
        return(.errorhandler(paste("Argument object must be of type character", 
            "or raw vector if serialize is FALSE"), mode = errormode))
    }
    if (file && !is.character(object)) 
        return(.errorhandler("file=TRUE can only be used with a character object", 
            mode = errormode))
    if (file && is_streaming_algo) 
        return(.errorhandler(paste0(algo, " algorithm can not be used with files."), 
            mode = errormode))
    algoint <- algo_int(algo)
    if (file) {
        algoint <- algoint + 100
        object <- path.expand(object)
        if (.isWindows()) 
            object <- enc2utf8(object)
        check_file(object, errormode)
    }
    if (is.character(skip)) 
        skip <- 0
    if (!is_streaming_algo) {
        val <- .Call(digest_impl, object, as.integer(algoint), 
            as.integer(length), as.integer(skip), as.integer(raw), 
            as.integer(seed))
    }
    else if (algo == "spookyhash") {
        val <- paste(.Call(spookydigest_impl, object, skip, 0, 
            0, serializeVersion, NULL), collapse = "")
    }
    if ((algoint == 3 || algoint == 103) && .getCRC32PreferOldOutput()) {
        val <- sub("^0+", "", val)
    }
    return(val)
}
