# A simple C++ client-server application

## Prerequisites
Clang++ version 17, Asio library installed on the host system. Optionally, it is possible to install a Docker container, which will download all the necessary dependencies

## Deploy Docker container
```bash
cd docker
docker compose up -d
```
Docker copose mounts repository as a "/workspace" volume inside a container.
```bash
# exec into a container
docker exec -it container_id /bin/bash
cd /workspace
```

## Build
```bash
mkdir build
cd build
cmake ..
cmkake --build .
```

# Warning! This version of application is unstable, untested and builds with errors
