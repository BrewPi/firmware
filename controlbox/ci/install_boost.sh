BOOST_VERSION=1_59_0
url=http://downloads.sourceforge.net/project/boost/boost/1.59.0/boost_$BOOST_VERSION.tar.gz
mkdir -p .cache/boost
dl=.cache/boost/boost_$BOOST_VERSION.tar.gz
test -f $dl || (
   rm .cache/boost/* 
   wget $url -O $dl
) 
export BOOST_HOME=$HOME/.ci/boost
export BOOST_ROOT=$BOOST_HOME/boost_$BOOST_VERSION
mkdir -p $BOOST_HOME
test -d $BOOST_ROOT || (
   tar zxf $dl 
   mv boost_$BOOST_VERSION  $BOOST_HOME
) 
test -d $BOOST_ROOT || ( echo "boost root $BOOST_ROOT not created." && exit 1)
