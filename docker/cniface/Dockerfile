FROM abelleeye/cniface_base:v0.0.2

RUN git clone https://github.com/coconut-island/CNIFace.git

COPY deps/models/* /CNIFace/models/
RUN cd /CNIFace/docker/cniface
RUN wget https://public.dm.files.1drv.com/y4mdPogZDMwNZy512iQ2CjKVIr7pR7eGhmcUTeCUE9X2AzcuY3_woRIX5dRdxfQ9QL-8q6bcnXVunM5_01PND5OAPhBM_0pf65nxtior6N_RvLXLOYJL8hIymssCkzwbJuP6ep4MDIvuO4HavYVDnl947g8t2SYLG8-q2g17Y5pa1yn_59NlfgEtiWTkf05gQtFtX0si0FY5sCeVH-DyoWve3BPiZrobtclDUQC1ZaB41A -O cniface_deps.tar.gz
RUN tar zxvf ./cniface_deps.tar.gz

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



