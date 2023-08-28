//
// Created by Abel Lee on 2022/11/6.
//

#ifndef CNIFACE_OCRSERVICE_H
#define CNIFACE_OCRSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../ppocr/ocr_det.h"
#include "../ppocr/ocr_cls.h"
#include "../ppocr/ocr_rec.h"


class OCRService final : public cniface::OCRService::Service {
public:
    explicit OCRService(const std::string& model_dir, const std::string& resource_dir, const std::vector<int> &cpu_devices);
    ~OCRService() override;

    grpc::Status ocr(::grpc::ServerContext *context, const ::cniface::OCRRequest *request,
                        ::cniface::OCRResponse *response) override;

private:
    PaddleOCR::DBDetector *detector;
    PaddleOCR::Classifier *classifier;
    PaddleOCR::CRNNRecognizer *recognizer;
};


#endif //CNIFACE_OCRSERVICE_H
