#!/bin/sh
docker build --no-cache -t chapter14 .
#docker run -it --entrypoint /bin/bash chapter14
docker build --no-cache -f Dockerfile.Ubuntu20.04 -t chapter14 .
#docker run -it --entrypoint /bin/bash chapter14
docker build --no-cache -f Dockerfile.debian -t chapter14 .
#docker run -it --entrypoint /bin/bash chapter14
