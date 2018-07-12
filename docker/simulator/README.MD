# How to build and use this container 

This container is just a small wrapper around the cross compiled brewblox simulator.

To build the executable to be copied into the container, run:

```
docker-compose build simulator-compiler
docker-compose run simulator-compiler
```

This will place the cbox executable into this directory

To build the simulator container run:
```
docker-compose build simulator
```

To use the container run:
```
docker-compose run --service-ports simulator
```

It will expose the brewblox protocol on TCP port 8332. 
