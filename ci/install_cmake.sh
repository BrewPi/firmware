file=cmake-3.4.3-Linux-x86_64.tar.gz

[[  -f .cache/$file ]] ||  wget --no-check-certificate http://cmake.org/files/v3.4/$file -O .cache/$file
tar -xzf .cache/$file                        
if [ "$CXX" = "g++" ]; then export CXX="g++-4.8" CC="gcc-4.8"; fi
export cmake=./cmake-3.4.3-Linux-x86_64/bin/cmake



