setwd( "~/projects/digitizer.git/data-analysis" )

wd <- read.table( './work_dac.dat' )
names( wd ) <- c( 'dac1', 'dac2', 'volt' )
wd$d1 <- wd$dac1 / 32767 - 1
wd$d2 <- wd$dac2 / 32767 - 1
wd$v <- wd$volt / 10000
cov( wd )
wobj <- lm( wd$v ~ wd$d1 + wd$d2 )
wobj$coefficients
plot( wd$dac1,  wd$volt )




pd <- read.table( './probe_dac.dat' )
names( pd ) <- c( 'dac1', 'dac2', 'volt' )
pd$d1 <- pd$dac1 / 32767 - 1
pd$d2 <- pd$dac2 / 32767 - 1
pd$v <- pd$volt / 10000
cov( pd )
pobj <- lm( pd$v ~ pd$d1 + pd$d2 )
pobj$coefficients
plot( pd$dac1,  pd$volt )

predict( pobj )
