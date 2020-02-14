FROM debian:buster-slim

RUN \
  apt-get update && apt-get install -y --no-install-recommends python3 && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt
