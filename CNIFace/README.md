# CNIFace

![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)

This project is one **simple face platform && grpc server**!

- Web front link: [CNIFaceWeb](https://github.com/coconut-island/CNIFaceWeb)

- Web server link: [CNIFacePlatform](https://github.com/coconut-island/CNIFacePlatform)

Supported modules:
- [x] Face Detect
- [x] Face Recognition
- [x] Face && Mask Detect
- [x] Attribute Gender And Age
- [x] Support Multiple CPU
- [x] Support PaddleOCR, only move right now
- [ ] Fuse Preprocess
- [ ] Refine Models Outputs OPs
- [ ] Model Performance Tune

# Docker

## docker-compose
You can run all from docker-compose
```
wget https://github.com/coconut-island/CNIFace/raw/main/docker/docker-compose.yaml
docker-compose up -d
```

# Local env build

## ubuntu:20.04

Refer to link: [Dockerfile](https://github.com/coconut-island/CNIFaceBase/blob/main/Dockerfile)

# Samples

link: [Samples](https://github.com/coconut-island/CNIFace/tree/main/samples)


# References

Thanks for these source codes porviding me with knowledges to complete this repository.

- [insightface](https://github.com/deepinsight/insightface)
