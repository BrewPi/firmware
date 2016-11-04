pushd $BOOST_ROOT
./bootstrap.sh
./b2  link=static --with-thread  --with-system --with-program_options --with-random 
popd
