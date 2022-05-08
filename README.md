# CNIFace


这个project 是想 做一个 人脸平台的

1. 做个retinaface tvm infer demo 【完成】

python 的tvm 自己安装
opencv4 自己安装
CmakeLists.txt lib路径自己改下

python3 ./convert_model/from_mxnet_mnet.25.py 
mkdir build && cd build && cmake .. && make && ./main