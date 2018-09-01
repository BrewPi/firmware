#!/bin/bash

# generates cpp with google's protobuf implementation. Not used in firmware, but used in unit test code
# do some renames so the names don't cause conflicts when both are used

PROTO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$PROTO_DIR" > /dev/null # .option files are read from execution directory, so have to cd into this dir 
rm -rf test
mkdir -p "test/proto"
mkdir -p "test/cpp"

# copy proto files with .test appended and fix includes 
for file in *.proto 
do
  cp "$file" "test/proto/${file%.proto}.test.proto"
  sed -i 's/brewblox.proto/brewblox.test.proto/g' "test/proto/${file%.proto}.test.proto"
  sed -i 's/BrewbloxFieldOptions/BrewbloxTestFieldOptions/g' "test/proto/${file%.proto}.test.proto"
done

# generate code
cd test/proto
protoc *.proto --cpp_out=../cpp

#rename .cc files to .cpp
cd ../cpp
for file in *.cc 
do
  mv "$file" "${file%.cc}.cpp"
done
popd > /dev/null
