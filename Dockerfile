FROM abelleeye/cniface-base:latest

RUN git clone https://github.com/coconut-island/CNIFace.git

RUN cd /CNIFace/models
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/det_10g.onnx -O /CNIFace/models/det_10g.onnx
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/mnet_cov2-0000.params -O /CNIFace/models/mnet_cov2-0000.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/mnet_cov2-symbol.json -O /CNIFace/models/mnet_cov2-symbol.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/w600k_r50.onnx -O /CNIFace/models/w600k_r50.onnx
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/det_500m.onnx -O /CNIFace/models/det_500m.onnx
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/models/w600k_mbf.onnx -O /CNIFace/models/w600k_mbf.onnx

RUN cd /CNIFace && python3 ./convert_model/from_onnx_arcface_w600k_r50.py
RUN cd /CNIFace && python3 ./convert_model/from_onnx_retinaface_det_10g.py
RUN cd /CNIFace && python3 ./convert_model/from_mxnet_mnet_cov2.py
RUN cd /CNIFace && python3 ./convert_model/from_onnx_arcface_w600k_mbf.py
RUN cd /CNIFace && python3 ./convert_model/from_onnx_retinaface_det_500m.py

RUN cp -r /CNIFace/models/relay/ /models/
RUN rm -rf /CNIFace/models

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



