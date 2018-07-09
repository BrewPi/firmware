url=http://downloads.sourceforge.net/project/boost/boost/1.65.0/boost_$BOOST_VERSION.tar.gz
mkdir -p $BOOST_HOME 
curl -sSL ${url} | tar -xz -C $BOOST_HOME
