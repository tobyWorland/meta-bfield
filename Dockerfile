FROM debian:latest
RUN apt-get update && \
apt-get install -y --no-install-recommends \
ca-certificates build-essential cmake git wget && \
rm -rf /var/cache/apt/{archives,lists/*}
RUN update-ca-certificates
RUN wget https://archives.boost.io/release/1.91.0/source/boost_1_91_0.tar.bz2 && \
tar -xf boost_*.tar.bz2 && \
rm *.tar.bz2 && \
cd boost* && \
./bootstrap.sh --prefix=/opt/boost --with-libraries=json && \
./b2 && ./b2 install && cd .. && \
rm -r boost*
ENV BOOST_ROOT=/opt/boost
