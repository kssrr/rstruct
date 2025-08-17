
R version 4.3.3 (2024-02-29) -- "Angel Food Cake"
Copyright (C) 2024 The R Foundation for Statistical Computing
Platform: x86_64-pc-linux-gnu (64-bit)

R ist freie Software und kommt OHNE JEGLICHE GARANTIE.
Sie sind eingeladen, es unter bestimmten Bedingungen weiter zu verbreiten.
Tippen Sie 'license()' or 'licence()' für Details dazu.

R ist ein Gemeinschaftsprojekt mit vielen Beitragenden.
Tippen Sie 'contributors()' für mehr Information und 'citation()',
um zu erfahren, wie R oder R packages in Publikationen zitiert werden können.

Tippen Sie 'demo()' für einige Demos, 'help()' für on-line Hilfe, oder
'help.start()' für eine HTML Browserschnittstelle zur Hilfe.
Tippen Sie 'q()', um R zu verlassen.

> serialize
function (object, connection, ascii = FALSE, xdr = TRUE, version = NULL, 
    refhook = NULL) 
{
    if (!is.null(connection)) {
        if (!inherits(connection, "connection")) 
            stop("'connection' must be a connection")
        if (missing(ascii)) 
            ascii <- summary(connection)$text == "text"
    }
    if (!ascii && inherits(connection, "sockconn")) 
        .Internal(serializeb(object, connection, xdr, version, 
            refhook))
    else {
        type <- if (is.na(ascii)) 
            2L
        else if (ascii) 
            1L
        else if (!xdr) 
            3L
        else 0L
        .Internal(serialize(object, connection, type, version, 
            refhook))
    }
}
<bytecode: 0x613f89ab17e8>
<environment: namespace:base>
> 
> 
