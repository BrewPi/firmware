MY_DIR=$(dirname $(readlink -f $0))

function makeit()
{
	echo "building $*"
    make -C . -s  all $*
if [[ "$?" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  exit 1
fi
}

function makeapp()
{
	makeit PLATFORM=gcc $*
	makeit PLATFORM=P1 $*
	makeit PLATFORM=photon $*
}

pushd "$MYDIR" > /dev/null
# makeapp APP=controller
makeapp APP=brewblox
popd > /dev/null