#!/bin/bash

CURRENT_DIR=$(realpath $(dirname "${BASH_SOURCE[0]}"))
ROOT_DIR=$CURRENT_DIR/../../

yum install -y dpkg

(cd $ROOT_DIR/build/packages && chmod +x ./generate_package.sh && ./generate_package.sh deb && cp -r ./*.deb $CURRENT_DIR)

docker build -t cochain/eoscochain.cdt:dev -f Dockerfile.local.custom --build-arg HTTP_PROXY=http://localhost:1080 --build-arg HTTPS_PROXY=http://localhost:1080 --network=host $CURRENT_DIR

