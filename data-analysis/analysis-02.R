setwd( "~/projects/digitizer.git/data-analysis" )

workOrigData <- read.table( './work_dac.dat' )
names( workOrigData ) <- c( 'd0', 'd1', 'd2', 'd3', 't', 'v' )
ds <- workOrigData[ , 1:5 ]

ds$tpow2  <- ds$t*ds$t
ds$tpow3  <- ds$t*ds$t*ds$t
ds$d0t    <- ds$d0*ds$t
ds$d1t    <- ds$d1*ds$t
ds$logt   <- log( ds$t )
ds$logd0t <- log( ds$d0t )
ds$logd1t <- log( ds$d1t )
ds$d0pow2   <- ds$d0*ds$d0
ds$d0pow3   <- ds$d0*ds$d0*ds$d0
ds$d0pow4   <- ds$d0*ds$d0*ds$d0*ds$d0
ds$d1pow2   <- ds$d1*ds$d1
ds$d1pow3   <- ds$d1*ds$d1*ds$d1
ds$d1pow3   <- ds$d1*ds$d1*ds$d1*ds$d1

ds$workV <- workOrigData$v


M <- cor( ds )
library(corrplot)
corrplot( M, method = "circle" )

maskSd <- function( ds, mask, foldsN, repeatsN )
{
    w <- dim( ds )[2] - 1
    nms <- colnames( ds )[ 1:(w+1) ]
    #print( nms )

    f <- paste( "d$", nms[w+1], "~", sep="" )
    #print( f )

    xAdded <- FALSE
    for ( i in 1:w )
    {
        if ( bitwAnd( mask, 2^(i-1) ) != 0 )
        {
            if ( xAdded )
            {
                f<- paste( f, "+", sep="" )
                xAdded <- FALSE
            }
            f<-paste( f, "d$", nms[i], sep="" )
            xAdded <- TRUE
            #print( f )
            #print( i )
            #print( w )
        }
    }
    #print( f )
    fla <- as.formula( f )

    dataSz <- dim( ds )[1]

    # Subdivide into training and testing dataset.
    testSz  <- dataSz/foldsN
    trainSz <- dataSz - testSz

    meanSd <- 0.0

    for( tries in 1:repeatsN )
    {
        shuffledDf <- ds[sample(nrow(ds)), ]            #sample rows
        testDf     <- shuffledDf[1:testSz, ]            #get training set
        trainDf    <- shuffledDf[(testSz+1):nrow(ds), ] #get test set

        d <- trainDf

        wobj <- lm( fla, d )

        d <- testDf
        pr <- predict( wobj, newdata=d, type="response" )
        curSd <- sd( d[, w+1] - pr )
        meanSd <- meanSd + curSd
        #print( curSd )
    }
    meanSd <- meanSd / repeatsN
    #print( meanSd )
    return( list( sd=meanSd, formula=fla ) )
}

bestFormula <- function( ds, foldsN, repeatsN )
{
    w <- dim( ds )[2] - 1
    w <- 2^w-1

    jobDone <- 0

    bestMask <- 1
    res     <- maskSd( ds, bestMask, foldsN, repeatsN )
    bestSd  <- res$sd[1]
    bestFla <- res$formula
    for ( mask in 2:w )
    {
        res <- maskSd( ds, mask, foldsN, repeatsN )
        #print( res )
        curSd <- res$sd[1]
        curFla <- res$formula

        #print( "returned data: " )
        #print( curSd )
        #print( curFla )

        if ( curSd < bestSd )
        {
            bestSd <- curSd
            bestMask <- mask
            bestFla  <- curFla
        }

        newDone <- round( 100*mask/w )
        if ( newDone != jobDone )
        {
            jobDone <- newDone
            print( paste( "Job done ", newDone, "%", sep="" ) )
            print( paste( "Current best sd=", bestSd, ", fla=", bestFla, sep="" ) )
        }
    }

    d <- ds
    wobj <- lm( bestFla, d )
    pr <- predict( wobj, newdata=d, type="response" )
    w <- dim( ds )[2]
    bestSd <- sd( d[, w] - pr )

    print( "Found best parameters: " )
    print( paste( "Best sd=", bestSd, " and best formula=", bestFla, sep="" ) )

    return( list( regObj=wobj, fla=bestFla, sd=bestSd ) )
}


REPEATS <- 32
FOLDS <- 5

res <- bestFormula( ds, FOLDS, REPEATS )









