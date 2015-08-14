setwd( "c:/projects/digitizer.git/data-analysis" )

wd <- read.table( './work_dac.dat' )
names( wd ) <- c( 'dac1', 'dac2', 'dac3', 'dac4', 'volt' )
wd$d1 <- wd$dac1 / 32767 - 1
wd$d2 <- wd$dac2 / 32767 - 1
wd$d3 <- wd$dac3 / 32767 - 1
wd$d4 <- wd$dac4 / 32767 - 1
wd$v <- wd$volt / 10000
wobj <- lm( wd$v ~ wd$d1 + wd$d2 )
wobj <- lm( wd$volt ~ wd$dac1 + wd$dac2 )
wobj$coefficients
plot( wd$dac2,  wd$volt )

m <- cov( wd )
library(corrplot)
M <- cor( wd )
corrplot( M, method = "circle" )




pd <- read.table( './probe_dac.dat' )
names( pd ) <- c( 'dac1', 'dac2', 'dac3', 'dac4', 'volt' )
pd$d1 <- pd$dac1 / 32767 - 1
pd$d2 <- pd$dac2 / 32767 - 1
pd$d3 <- pd$dac3 / 32767 - 1
pd$d4 <- pd$dac4 / 32767 - 1
pd$v <- pd$volt / 10000
cov( pd )
pobj <- lm( pd$v ~ pd$d1 + pd$d2 + pd$d3 + pd$d4 )
pobj$coefficients
plot( pd$dac4,  pd$volt )

predict( pobj )

library(corrplot)
M <- cor( pd )
corrplot( M, method = "circle" )





adc <- read.table( './adc.dat' )
names( adc ) <- c('adc0', 'adc1', 'adc2', 'adc3', 'v0', 'v1', 'v2', 'v3' )
library(corrplot)
adcM <- cor( adc )
corrplot( adcM, method = "circle" )




