FROM debian:buster-slim

RUN \
  apt-get update && apt-get install -y --no-install-recommends python3 && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt

COPY pytarok.cpython-37m-x86_64-linux-gnu.so pyspiel.so /opt/
ENV PYTHONPATH $PYTHONPATH:/opt
CMD ["python3"]
