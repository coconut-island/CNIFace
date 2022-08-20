FROM abelleeye/cniface_base:v0.0.2

RUN git clone https://github.com/coconut-island/CNIFace.git

RUN cd /CNIFace/models
RUN wget https://public.dm.files.1drv.com/y4mVBYPo1YdLuzawYjUPP97n0bGQK_0n9YAo1H1M3IQ5H2ucDMxSqx0VAP5JP2acIH1zTDgpMdNPNTI6p3A5BiftmeJYvkvWRTRWT5gks08iriVd9JGpnt1u9JW8vOegSWFbLeL7FXVMqY6aNmlpwG9ma8jxRtFjRLbvXF7d6UmwtBDvcGp83ixeEBOLGYbr8UNNBpQQKXPM-LW1jvuzA3mHtZH9aJ---RsL-iHiZE0x2I -O cniface_models.tar.gz
RUN tar zxvf ./cniface_models.tar.gz && mv ./cniface_models/* ./models/

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



