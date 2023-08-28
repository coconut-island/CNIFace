FROM openjdk:11.0.15-jre
WORKDIR /
COPY build/libs/cniface-platform.jar /
CMD ["java", \
        "-Xlog:gc=info:file=gc.log:uptime:filecount=2:filesize=512M", \
        "-XX:+PrintClassHistogram", \
        "-jar", "cniface-platform.jar"]