BOOST_VERSION=1_59_0
url=https://github.com/boostorg/boost/archive/boost-1.59.0.tar.gz
[[ -d .cache ]] || mkdir .cache
dl=.cache/boost_$BOOST_VERSION.tar.gz
test -f $dl || wget $url -O $dl 
export BOOST_HOME=$HOME/.ci/boost
export BOOST_ROOT=$BOOST_HOME/boost_$BOOST_VERSION
mkdir -p $BOOST_HOME
test -d $BOOST_ROOT || (
   tar zxf $dl 
   mv boost_$BOOST_VERSION  $BOOST_HOME
) 
test -d $BOOST_ROOT || ( echo "boost root $BOOST_ROOT not created." && exit 1)
