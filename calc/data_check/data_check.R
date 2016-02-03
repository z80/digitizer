setwd( 'c:/projects/digitizer.git/calc/data_check' );
d <- read.csv( "data.txt", header=T, sep=";" )
plot( d$a1 );
plot( d$b2 );
plot( d$c3 );
plot( d$d4 );
plot( d$e5 );
plot( d$g6 );
plot( d$h7 );
plot( d$i8 );
points( orig$day, orig$balance, col="dark red" );
grid( col = "gray", lty = "dotted" );
