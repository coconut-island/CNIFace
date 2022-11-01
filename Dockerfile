FROM abelleeye/cniface-base:202211010248

RUN git clone https://github.com/coconut-island/CNIFace.git

RUN mkdir -p /models/

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.json -O /models/det_10g.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.params -O /models/det_10g.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_10g.so -O /models/det_10g.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.json -O /models/det_500m.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.params -O /models/det_500m.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/det_500m.so -O /models/det_500m.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.json -O /models/mnet_cov2.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.params -O /models/mnet_cov2.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/mnet_cov2.so -O /models/mnet_cov2.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.json -O /models/genderage.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.params -O /models/genderage.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/genderage.so -O /models/genderage.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.json -O /models/w600k_r50.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.params -O /models/w600k_r50.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_r50.so -O /models/w600k_r50.so

RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.json -O /models/w600k_mbf.json
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.params -O /models/w600k_mbf.params
RUN wget https://github.com/coconut-island/CNIFaceResource/raw/main/relay_models/x86_llvm/w600k_mbf.so -O /models/w600k_mbf.so

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



