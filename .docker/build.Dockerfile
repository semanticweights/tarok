FROM debian:buster-slim

RUN \
  apt-get update && apt-get install -y --no-install-recommends \
    git \
    virtualenv \
    clang \
    cmake \
    make \
    python3 \
    python3-dev \
    python3-pip \
    python3-setuptools \
    python3-wheel \
    ca-certificates \
    curl \
    gnupg2 \
    software-properties-common \
    apt-transport-https && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt

# install docker
RUN \
  curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add && \
  add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable" && \
  apt-get update && apt-get install -y docker-ce-cli && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt
