file=cmake-3.4.3-Linux-x86_64.tar.gz
dl=.cache/cmake/$file
mkdir -p .cache/cmake

[[  -f $dl ]] || (
    rm .cache/cmake/*
    wget --no-check-certificate http://cmake.org/files/v3.4/$file -O $dl
)
tar -xzf $dl                        
if [ "$CXX" = "g++" ]; then export CXX="g++-4.9" CC="gcc-4.9"; fi
export cmake=./cmake-3.4.3-Linux-x86_64/bin/cmake



