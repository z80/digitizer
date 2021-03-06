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
        testDf     <- shuffledDf[1:testSz, ]            #get test set
        trainDf    <- shuffledDf[(testSz+1):nrow(ds), ] #get training set

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
    t0 <- round( as.numeric( Sys.time() ) )
    lastT <- t0

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
        t1 <- round( as.numeric( Sys.time() ) )
        if ( t1 - lastT >= 10 )
        {
            lastT <- t1
            elapsed <- as.character( (t1-t0)/60.0 )
            #print( elapsed )
            print( paste( "Elapsed ", elapsed, " min", sep="" ) )
            left <- as.character( (t1-t0)*(w-mask)/mask/60.0 )
            #print( left )
            print( paste( "Left ", left, " min", sep="" ) )
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



