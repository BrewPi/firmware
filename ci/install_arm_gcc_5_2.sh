ARM_GCC_VERSION=5.2.1
ext=bz2
mkdir -p $(ci_cache:-.}
target=${ci_cache:-.}/arm-gcc-$ARM_GCC_VERSION.tar.$ext
url=https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.$ext
test -f $target || wget -O $target $url 



mkdir -p $BOOST_HOME
test -d $BOOST_ROOT || (
   tar zxf boost_$BOOST_VERSION.tar.gz
   mv boost_$BOOST_VERSION  $BOOST_HOME
) 
test -d $BOOST_ROOT || ( echo "boost root $BOOST_ROOT not created." && exit 1)
