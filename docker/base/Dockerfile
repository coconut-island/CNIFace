FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

COPY deps/sources.list /etc/apt/sources.list
RUN apt update
RUN apt update
RUN apt install -y libopencv-dev

RUN apt install -y cmake git build-essential autoconf libtool pkg-config libgflags-dev
# RUN git clone --recurse-submodules -b v1.47.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
COPY deps/grpc /grpc
RUN cd grpc \
    && mkdir -p build \
    && cd build    \
    && cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=ON .. \
    && make -j $(nproc) \
    && make install

RUN rm -rf /grpc

RUN apt install -y python3 python3-dev python3-setuptools python3-pip gcc libtinfo-dev zlib1g-dev build-essential libedit-dev libxml2-dev
RUN apt install -y llvm clang

# RUN cd / && git clone --recursive https://github.com/apache/tvm tvm
COPY deps/tvm /tvm
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

RUN rm -rf /tvm
RUN pip3 install onnx -i https://pypi.tuna.tsinghua.edu.cn/simple
RUN pip3 install mxnet -i https://pypi.tuna.tsinghua.edu.cn/simple
