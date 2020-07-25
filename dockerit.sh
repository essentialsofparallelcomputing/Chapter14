#!/bin/sh
docker build -f Dockerfile.Ubuntu20.04 -t chapter14 .
docker run -it --entrypoint /bin/bash chapter14
