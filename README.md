# CNIFace

![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)

This project is one **grpc server of face**!

- Web front link: [CNIFaceWeb](https://github.com/coconut-island/CNIFaceWeb)

- Web server link: [CNIFacePlatform](https://github.com/coconut-island/CNIFacePlatform)

Supported modules:
- [x] face detect
- [x] face recognition
- [x] face && mask detect, mnet_cov2

# Docker

## docker-compose
You can run all from docker-compose
```
cd docker/cniface/
docker-compose -f milvus-standalone-docker-compose.yml up -d 
docker-compose up -d 
```

# Local env build

## ubuntu:20.04

Refer to link: [Dockerfile](https://github.com/coconut-island/CNIFace/blob/main/docker/base/Dockerfile)

# Samples

link: [Samples](https://github.com/coconut-island/CNIFace/tree/main/samples)


# References

Thanks for these source codes porviding me with knowledges to complete this repository.

- [insightface](https://github.com/deepinsight/insightface)
- [milvus](https://github.com/milvus-io/milvus)
