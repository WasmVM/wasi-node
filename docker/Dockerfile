FROM ubuntu:cosmic

RUN apt update \
    && apt install -y gcc g++ git vim curl build-essential

RUN locale-gen "en_US.UTF-8"
RUN sh -c "$(curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -)"

RUN apt install -y nodejs
RUN sh -c "$(npm install -g node-gyp)"