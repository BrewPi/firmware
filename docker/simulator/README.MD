# How to build and use this container

This container is just a small wrapper around the cross compiled brewblox simulator.

To build the executable to be copied into the container, run build-simulator.sh in the parent directory.

To use the container run:
```
docker-compose run --service-ports simulator
```

It will expose the brewblox protocol on TCP port 8332.
