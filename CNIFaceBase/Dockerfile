FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

RUN apt update
RUN apt install -y libopencv-dev

RUN apt install -y wget git build-essential autoconf libtool pkg-config libgflags-dev libssl-dev openssl zstd

RUN cd / && git clone --recursive -b v3.24.2 https://github.com/Kitware/CMake.git CMake
RUN cd CMake \
    && ./bootstrap \
    && make -j $(nproc) \
    && make install

RUN cd / && rm -rf /CMake


RUN apt install unzip libzstd-dev libopenblas-dev liblapack-dev -y
# RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/third_party/gamma-87e90349e385e5392089b5f5ed88861d55558781.zip -O /gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && unzip ./gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && rm -rf ./gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && mv ./gamma-87e90349e385e5392089b5f5ed88861d55558781 ./gamma

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/third_party/gamma-87e90349e385e5392089b5f5ed88861d55558781_without_avx2.zip -O /gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && unzip ./gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && rm -rf ./gamma-87e90349e385e5392089b5f5ed88861d55558781.zip && mv ./gamma-87e90349e385e5392089b5f5ed88861d55558781 ./gamma

RUN cd /gamma/third_party && bash build.sh && cd faiss && cp -r ./include/* /usr/local/include/ && cp ./lib/* /lib/x86_64-linux-gnu/

RUN cd /gamma \
    && mkdir -p build \
    && cd build    \
    && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_WITH_SCANN=OFF ..

RUN cd /gamma/idl && bash build.sh && bash build.sh
RUN cd /gamma/build \
    && make -j $(nproc) \
    && make install

RUN rm -rf /gamma

RUN cd / && git clone --recurse-submodules -b v1.47.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc 
RUN cd grpc \
    && mkdir -p build \
    && cd build    \
    && cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=ON .. \
    && make -j $(nproc) \
    && make install

RUN cd / && rm -rf /grpc

RUN apt install -y python3 python3-dev python3-setuptools python3-pip gcc libtinfo-dev zlib1g-dev build-essential libedit-dev libxml2-dev
RUN apt install -y llvm clang

RUN cd / && git clone --recursive -b v0.9.0 https://github.com/apache/tvm tvm
RUN cd /tvm/3rdparty/dlpack/ \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j $(nproc) \
    && make install

RUN cd /tvm/3rdparty/dmlc-core/ \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j $(nproc) \
    && make install

RUN cd /tvm \
    && mkdir -p build \
    && cd build \
    && cmake -DUSE_LLVM=`which llvm-config` .. \
    && make -j $(nproc) \
    && make install

RUN cd /tvm/python \
    && python3 setup.py install

RUN cd / && rm -rf /tvm
RUN pip3 install onnx
RUN pip3 install mxnet
