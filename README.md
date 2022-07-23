# CNIFace

GRPC 服务

现有功能
1. 人脸检测
2. 人脸特征提取

一个学习实验型项目。

此Repo的code和model均来自于开源社区

启动项目的解决方案主要来自于 insightface，非常感谢

## Docker 启动

### docker
`docker run abelleeye/cniface:v0.0.1`

### docker-compose
```
version: "3.9"
services:

  cniface:
    image: abelleeye/cniface:v0.0.1
    container_name: cniface
    restart: always
    command: cniface -m /models/
    ports:
      - 22506:22506
```


## 环境搭建

环境搭建请参考 docker/base 下的 Dockerfile

## samples

目录：samples/

