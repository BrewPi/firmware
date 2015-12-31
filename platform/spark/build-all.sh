
function makeit()
{
	echo "building $*"
    make -C .  all $*
if [[ "$?" -eq 0 ]]; then
  echo "✓ SUCCESS"
else
  echo "✗ FAILED"
  exit 1
fi
}

function makeapp()
{
	makeit PLATFORM=core $*
	makeit PLATFORM=photon $*
}

pwd
makeapp APP=controller
makeapp APP=cbox
