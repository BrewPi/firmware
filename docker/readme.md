To make it easier to build the firmware on various platforms, a docker container is used to run the build process.

To build simply run these commands in the docker directory:

 docker-compose build
 docker-compose run compiler make PLATFORM=p1
 
 # Windows
 If you run docker with docker-toolbox on Windows, you need an extra step to give the docker-machine access to the firmware repo files.
 
 1. Stop the docker-machine VM (docker-machine stop)
 2. In virtualbox, add a shared folder to the VM so it has access to the firmware repo
 3. Restart the docker-machine VM (docker-machine start)
 
 Or follow this guide: http://support.divio.com/local-development/docker/how-to-use-a-directory-outside-cusers-with-docker-toolbox-on-windowsdocker-for-windows
 
 And change the volume in the docker-compose.yml file to use the mount path in the docker-machine VM.
 
 Also you might want to change the settings of the VM to:
 - Increase available memory
 - Add more CPUs
 - Enable USB 2.0 and give access to the photon/p1. Add it in normal mode, then put it in DFU mode and also add that device.