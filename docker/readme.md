To make it easier to build the firmware on various platforms, a docker container is used to run the build process.

To build simply run these commands in the docker directory:

 docker-compose build
 docker-compose run compiler make PLATFORM=p1