FROM ubuntu:cosmic

RUN apt update \
    && apt install -y gcc g++ git curl build-essential
    
RUN sh -c "$(curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -)"

RUN apt install -y nodejs
RUN sh -c "$(npm install -g node-gyp)"