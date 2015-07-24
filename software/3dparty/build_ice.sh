
ROOT_DIR=$(pwd)
echo $ROOT_DIR

MCPP_DIR=$ROOT_DIR/../.build/mcpp
mkdir -p $MCPP_DIR
cd $MCPP_DIR
$ROOT_DIR/mcpp-2.7.2/configure --prefix=$MCPP_DIR CFLAGS=-fPIC --enable-mcpplib --disable-shared
chmod -R 744 ./
make
mkdir -p $MCPP_DIR/lib
cp -r $MCPP_DIR/src/.libs/* $MCPP_DIR/lib

#exit 0

ICEE_DIR=$ROOT_DIR/../.build/icee
mkdir -p $ICEE_DIR
cd $ICEE_DIR
cp -r $ROOT_DIR/IceE-1.3.0/* $ICEE_DIR
chmod -R 744 $ICEE_DIR
cd $ICEE_DIR/cpp/
make

# Build IceE
cd $ICEE_DIR
make configure
make
