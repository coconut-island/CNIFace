#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>

#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/registry.h>
#include <tvm/runtime/packed_func.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "anchor_generator.h"
#include "config.h"
#include "ulsMatF.h"

using namespace std;
using namespace tvm::runtime;

void nms_cpu(std::vector<Anchor>& boxes, float threshold, std::vector<Anchor>& filterOutBoxes) {
    filterOutBoxes.clear();
    if(boxes.empty())
        return;
    std::vector<size_t> idx(boxes.size());

    for(unsigned i = 0; i < idx.size(); i++)
    {
        idx[i] = i;
    }

    //descending sort
    sort(boxes.begin(), boxes.end(), std::greater<Anchor>());

    while(!idx.empty())
    {
        int good_idx = idx[0];
        filterOutBoxes.push_back(boxes[good_idx]);

        std::vector<size_t> tmp = idx;
        idx.clear();
        for(unsigned i = 1; i < tmp.size(); i++)
        {
            int tmp_i = tmp[i];
            float inter_x1 = std::max( boxes[good_idx][0], boxes[tmp_i][0] );
            float inter_y1 = std::max( boxes[good_idx][1], boxes[tmp_i][1] );
            float inter_x2 = std::min( boxes[good_idx][2], boxes[tmp_i][2] );
            float inter_y2 = std::min( boxes[good_idx][3], boxes[tmp_i][3] );

            float w = std::max((inter_x2 - inter_x1 + 1), 0.0F);
            float h = std::max((inter_y2 - inter_y1 + 1), 0.0F);

            float inter_area = w * h;
            float area_1 = (boxes[good_idx][2] - boxes[good_idx][0] + 1) * (boxes[good_idx][3] - boxes[good_idx][1] + 1);
            float area_2 = (boxes[tmp_i][2] - boxes[tmp_i][0] + 1) * (boxes[tmp_i][3] - boxes[tmp_i][1] + 1);
            float o = inter_area / (area_1 + area_2 - inter_area);
            if( o <= threshold )
                idx.push_back(tmp_i);
        }
    }
}


int main(int argc, char *argv[]) {
    DLDevice dev{kDLCPU, 0};
    string model_dir_path = "../models/relay/retinaface_mnet025_v1/";
    string so_lib_path = model_dir_path + "mnet.25.x86.cpu.so";
    Module mod_syslib = Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + "mnet.25.x86.cpu.json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + "mnet.25.x86.cpu.params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    // parameters need to be TVMByteArray type to indicate the binary data
    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();

    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;

    // get global function module for graph runtime
    tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, device_type, device_id);

    DLTensor* x;

    int in_ndim = 4;
    int in_c = 3, in_h = 640, in_w = 480;

    int ratio_x = 1, ratio_y = 1;
    int64_t in_shape[4] = {1, in_c, in_h, in_w};
    TVMArrayAlloc(in_shape, in_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &x);

    int64_t w1=ceil(in_w/32.0),w2=ceil(in_w/16.0),w3=ceil(in_w/8.0), h1=ceil(in_h/32.0),h2=ceil(in_h/16.0),h3=ceil(in_h/8.0);
    int out_num = (w1*h1+w2*h2+w3*h3)*(4+8+20);

    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod.GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");

    int total_input = 3*in_w*in_h;
    auto* data_x = (float*)malloc(total_input*sizeof(float));
    //float* y_iter = (float*)malloc(out_num*4);


    cv::Mat image = cv::imread("../images/1.jpg");
    if(!image.data)
        printf("load error");

    cv::Mat resizeImage;
    cv::resize(image, resizeImage, cv::Size(in_w, in_h), cv::INTER_AREA);
    cv::Mat input_mat;

    const int64 start = cv::getTickCount();
    resizeImage.convertTo(input_mat, CV_32FC3);
    //cv::cvtColor(input_mat, input_mat, cv::COLOR_BGR2RGB);

    cv::Mat split_mat[3];
    cv::split(input_mat, split_mat);
    memcpy(data_x,                                 split_mat[2].ptr<float>(), input_mat.cols*input_mat.rows*sizeof(float));
    memcpy(data_x+input_mat.cols*input_mat.rows,   split_mat[1].ptr<float>(), input_mat.cols*input_mat.rows*sizeof(float));
    memcpy(data_x+input_mat.cols*input_mat.rows*2, split_mat[0].ptr<float>(), input_mat.cols*input_mat.rows*sizeof(float));
    TVMArrayCopyFromBytes(x, data_x, total_input*sizeof(float));

    const int64 start_forward= cv::getTickCount();
    double duration_before = (start_forward-start)/ cv::getTickFrequency();
    std::cout << "Pre Forward Time: " << duration_before*1000 << "ms" << std::endl;
    // get the function from the module(set input data)
    set_input("data", x);

    load_params(params_arr);

    // get the function from the module(run it)
    run();

    std::vector<AnchorGenerator> ac(_feat_stride_fpn.size());
    for (int i = 0; i < _feat_stride_fpn.size(); ++i) {
        int stride = _feat_stride_fpn[i];
        ac[i].Init(stride, anchor_cfg[stride], false);
    }

    std::vector<Anchor> proposals;
    proposals.clear();

    int64_t w[3] = {w1, w2, w3};
    int64_t h[3] = {h1, h2, h3};
    int64_t out_size[9] = {w1*h1*4, w1*h1*8, w1*h1*20, w2*h2*4, w2*h2*8, w2*h2*20, w3*h3*4, w3*h3*8, w3*h3*20};

    int out_ndim = 4;
    int64_t out_shape[9][4] = {{1, 4, h1, w1}, {1, 8, h1, w1}, {1, 20, h1, w1},
                               {1, 4, h2, w2}, {1, 8, h2, w2}, {1, 20, h2, w2},
                               {1, 4, h3, w3}, {1, 8, h3, w3}, {1, 20, h3, w3}};

    DLTensor* y[9];
    for (int i = 0 ; i < 9; i++)
        TVMArrayAlloc(out_shape[i], out_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &y[i]);

    for (int i = 0 ; i < 9; i+=3) {
        get_output(i, y[i]);
        get_output(i+1, y[i+1]);
        get_output(i+2, y[i+2]);

        ulsMatF clsMat(w[i/3], h[i/3], 4);
        ulsMatF regMat(w[i/3], h[i/3], 8);
        ulsMatF ptsMat(w[i/3], h[i/3], 20);

        TVMArrayCopyToBytes(y[i], clsMat.m_data, out_size[i]*sizeof(float));
        TVMArrayCopyToBytes(y[i+1], regMat.m_data, out_size[i+1]*sizeof(float));
        TVMArrayCopyToBytes(y[i+2], ptsMat.m_data, out_size[i+2]*sizeof(float));

        ac[i/3].FilterAnchor(clsMat, regMat, ptsMat, proposals);
        std::cout <<"proposals:" << proposals.size() << std::endl;

    }

    const int64 end_forward= cv::getTickCount();
    double duration_forward = (end_forward-start_forward)/ cv::getTickFrequency();
    std::cout << "Forward Time: " << duration_forward*1000 << "ms" << std::endl;

    // nms
    std::vector<Anchor> result;
    nms_cpu(proposals, nms_threshold, result);

    const int64 end_all= cv::getTickCount();
    double duration_after = (end_all-end_forward)/ cv::getTickFrequency();
    std::cout << "Post Forward Time: " << duration_after*1000 << "ms" << std::endl;
    double duration = (end_all-start)/ cv::getTickFrequency();
    std::cout << "All Time: " << duration*1000 << "ms" << std::endl;

    printf("final proposals: %ld\n", result.size());
    for(int i = 0; i < result.size(); i ++)
    {
        printf("score%d: %.6f\n", i, result[i].score);
        cv::rectangle (resizeImage, cv::Point((int)result[i].finalbox.x*ratio_x, (int)result[i].finalbox.y*ratio_y), cv::Point((int)result[i].finalbox.width*ratio_x, (int)result[i].finalbox.height*ratio_y), cv::Scalar(0, 255, 255), 2, 8, 0);
        for (auto & pt : result[i].pts) {
            cv::circle(resizeImage, cv::Point((int)pt.x*ratio_x, (int)pt.y*ratio_y), 1, cv::Scalar(225, 0, 225), 2, 8);
        }
    }
    result[0].print();

    cv::imshow("img", resizeImage);
    cv::waitKey(0);

    free(data_x);
//    free(y_iter);
    data_x = nullptr;
    //y_iter = nullptr;

    TVMArrayFree(x);
    for (auto & i : y)
        TVMArrayFree(i);
    return 0;
}