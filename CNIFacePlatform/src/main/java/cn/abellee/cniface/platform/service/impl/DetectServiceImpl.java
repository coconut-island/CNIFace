package cn.abellee.cniface.platform.service.impl;

import cn.abellee.cniface.grpc.DetectRequest;
import cn.abellee.cniface.grpc.DetectResponse;
import cn.abellee.cniface.platform.domain.dto.DetectRequestDTO;
import cn.abellee.cniface.platform.domain.dto.DetectResult;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.service.IDetectService;
import cn.abellee.cniface.platform.service.cni.CNIFaceDetectService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * @author abel
 * @date 2022/6/5 3:28 PM
 */
@Service
@Slf4j
public class DetectServiceImpl implements IDetectService {

    private final CNIFaceDetectService cniFaceDetectService;


    public DetectServiceImpl(CNIFaceDetectService cniFaceDetectService) {
        this.cniFaceDetectService = cniFaceDetectService;
    }

    @Override
    public List<DetectResult> detect(final DetectRequestDTO detectRequest) {
        DetectResponse detectResponse = cniFaceDetectService.detect(
                DetectRequest.newBuilder()
                        .setFaceImageBase64(detectRequest.getFaceImageBase64())
                        .setScore(detectRequest.getScore())
                        .setIsReturnRecognitionSubImage(detectRequest.getIsReturnRecognitionSubImage())
                        .build()
        );


        if (detectResponse.getCode() != 0) throw new CNIFaceException(detectResponse.getCode(), detectResponse.getMessage());
        List<DetectResult> detectResults = new ArrayList<>();
        for (cn.abellee.cniface.grpc.DetectResult grpcDetectResult : detectResponse.getResultsList()) {
            DetectResult detectResult = new DetectResult(grpcDetectResult);
            detectResults.add(detectResult);
        }
        return detectResults;
    }

}
