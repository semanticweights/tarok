FROM debian:buster-slim

RUN \
  apt-get update && apt-get install -y --no-install-recommends python3 && \
  apt-get clean && apt-get autoclean && rm -rf /var/lib/apt && \
  python3 -m pip install --upgrade pip setuptools && \
  python3 -m pip install -r requirements.txt && \
  addgroup --gid 12345 tarok && useradd -ms /bin/bash --uid 12345 --gid 12345 tarok

USER 12345

COPY pytarok.cpython-37m-x86_64-linux-gnu.so pyspiel.so /opt/
ENV PYTHONPATH $PYTHONPATH:/opt

ENTRYPOINT ["python3"]
