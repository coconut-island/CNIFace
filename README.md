# CNIFace

![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)

Web server link: [CNIFacePlatform](https://github.com/coconut-island/CNIFacePlatform)

This project is one **grpc server of face**!

Supported modules:
- [x] face detect
- [x] face recognition

# Docker

## docker
`docker run abelleeye/cniface:v0.0.2`

## docker-compose
```
version: "3.9"
services:

  cniface:
    image: abelleeye/cniface:v0.0.2
    container_name: cniface
    restart: always
    command: cniface -m /models/
    ports:
      - 22506:22506
```

# Local env build

## ubuntu:20.04

Refer to link: [Dockerfile](https://github.com/coconut-island/CNIFace/blob/main/docker/base/Dockerfile)

# Samples

link: [Samples](https://github.com/coconut-island/CNIFace/tree/main/samples)


# References

Thanks for these source codes porviding me with knowledges to complete this repository.

- [insightface](https://github.com/deepinsight/insightface)
```
