FROM abelleeye/cniface-base:202211200337

RUN mkdir -p /cniface

RUN git clone https://github.com/coconut-island/CNIFace.git

RUN cd /CNIFace \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j $(nproc) \
    && make install

RUN ldconfig

RUN rm -rf /CNIFace

RUN mkdir -p /cniface/models/

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.json -O /cniface/models/det_10g.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.params -O /cniface/models/det_10g.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.so -O /cniface/models/det_10g.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.json -O /cniface/models/det_500m.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.params -O /cniface/models/det_500m.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.so -O /cniface/models/det_500m.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.json -O /cniface/models/mnet_cov2.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.params -O /cniface/models/mnet_cov2.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.so -O /cniface/models/mnet_cov2.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.json -O /cniface/models/genderage.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.params -O /cniface/models/genderage.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.so -O /cniface/models/genderage.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.json -O /cniface/models/w600k_r50.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.params -O /cniface/models/w600k_r50.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.so -O /cniface/models/w600k_r50.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.json -O /cniface/models/w600k_mbf.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.params -O /cniface/models/w600k_mbf.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.so -O /cniface/models/w600k_mbf.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_det_infer.json -O /cniface/models/ch_PP-OCRv3_det_infer.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_det_infer.params -O /cniface/models/ch_PP-OCRv3_det_infer.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_det_infer.so -O /cniface/models/ch_PP-OCRv3_det_infer.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_ppocr_mobile_v2.0_cls_infer.json -O /cniface/models/ch_ppocr_mobile_v2.0_cls_infer.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_ppocr_mobile_v2.0_cls_infer.params -O /cniface/models/ch_ppocr_mobile_v2.0_cls_infer.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_ppocr_mobile_v2.0_cls_infer.so -O /cniface/models/ch_ppocr_mobile_v2.0_cls_infer.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_rec_infer.json -O /cniface/models/ch_PP-OCRv3_rec_infer.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_rec_infer.params -O /cniface/models/ch_PP-OCRv3_rec_infer.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/ch_PP-OCRv3_rec_infer.so -O /cniface/models/ch_PP-OCRv3_rec_infer.so

WORKDIR /

CMD ["cniface", "-m", "/models/"]



