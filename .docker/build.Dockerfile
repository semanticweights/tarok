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
    python3-wheel && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt
