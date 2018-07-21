# Firmware integration tests

## Context

Integration tests for the combination of Spark firmware and Spark Python service have three purposes:
* Improved feedback loop for firmware development
* Verification of compatibility firmware / service
* Automated regression tests

To increase portability and reliability, these tests are containerized.

## Building

To build the Docker image:
```
docker build -t brewblox/firmware-integration .
```

To push it to Docker hub:
```
docker push brewblox/firmware-integration
```

## Running

The recommended command is:
```
docker run --rm -t --network host brewblox/firmware-integration
```

This will run the tests as they were when the image was built, and tries to find the Spark service at `http://localhost:5000`.
`--rm` will remove the container after it stops and `-t` gives pretty colors in your terminal.

## Special commands

**Removing containers**

It is strongly recommended to use the `--rm` switch during development. This avoids hundreds of single-use containers remaining on disk.
```
docker run --rm -t brewblox/firmware-integration
```

**Spark service address**

If the Spark service is currently running on the host, the integration test container must share a network.

The simplest approach is to publish the Spark service's port, and run the integration tests on the host network:
```
docker run --network host brewblox/firmware-integration
```

An alternative approach is to explicitly set the Spark service host name:
```
docker run -e SPARK_ADDRESS=spark brewblox/firmware-integration
```

**Mounting tests**

To run newly changed tests without rebuilding the image, you can mount the tests as volume:
```
docker run --rm -t --network host -v $(pwd)/test:/app/test brewblox/firmware-integration
```

**Pytest arguments**

Any arguments passed to the docker container are used as arguments to the `pytest` executable. For a list of available commands, you can use `--help`.

**Terminal**

Pytest adds pretty colors and symbols when run from a tty, and also supports the `--pdb` command to start debugging when an error is found.

To run the container in tty mode:
```
docker run -t brewblox/firmware-integration
```

To start debugging on error:
```
docker run -it brewblox/firmware-integration --pdb
```
