BOOST_VERSION=1_59_0
url=https://github.com/boostorg/boost/archive/boost-1.59.0.tar.gz
[[ -d .cache ]] || mkdir .cache
test -f .cache/boost_$BOOST_VERSION.tar.gz || wget $url -O .cache/boost_$BOOST_VERSION.tar.gz 
export BOOST_HOME=$HOME/.ci/boost
export BOOST_ROOT=$BOOST_HOME/boost_$BOOST_VERSION
mkdir -p $BOOST_HOME
test -d $BOOST_ROOT || (
   tar zxf boost_$BOOST_VERSION.tar.gz
   mv boost_$BOOST_VERSION  $BOOST_HOME
) 
test -d $BOOST_ROOT || ( echo "boost root $BOOST_ROOT not created." && exit 1)
