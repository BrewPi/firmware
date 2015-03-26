
3 easy steps:

# Setup

Clone the `firmware` and `spark-firmware` repos to the same parent folder (e.g. 'brewpi').

- In the spark-firmware repo, change to the "feature/hal" branch: `git checkout feature/hal`
- in the firmwarwe repo, change to the "develop" branch: `git checkout develop`

# Build

```
cd brewpi/firmware/spark
make
```

This will build the binary to the file `target/brewpi.bin`.

# Profit!