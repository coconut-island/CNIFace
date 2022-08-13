FROM abelleeye/cniface_base:v0.0.2

RUN git clone https://github.com/coconut-island/CNIFace.git

COPY deps/models/* /CNIFace/models/

RUN cd /CNIFace && python3 ./convert_model/from_onnx_arcface.py
RUN cd /CNIFace && python3 ./convert_model/from_onnx_retinaface.py
RUN cd /CNIFace && python3 ./convert_model/from_mxnet_mnet_cov2.py

RUN cp -r /CNIFace/models/relay/ /models/

RUN cd /CNIFace \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j $(nproc) \
    && make install

RUN ldconfig

RUN rm -rf /CNIFace

WORKDIR /

CMD ["cniface", "-m", "/models/"]



