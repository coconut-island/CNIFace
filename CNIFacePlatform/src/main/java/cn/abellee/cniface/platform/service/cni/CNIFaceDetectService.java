package cn.abellee.cniface.platform.service.cni;

import cn.abellee.cniface.grpc.DetectRequest;
import cn.abellee.cniface.grpc.DetectResponse;
import cn.abellee.cniface.grpc.DetectServiceGrpc;
import cn.abellee.cniface.platform.config.CNIFaceModelProperties;
import io.grpc.StatusRuntimeException;
import lombok.extern.slf4j.Slf4j;
import net.devh.boot.grpc.client.inject.GrpcClient;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/6/5 3:27 PM
 */
@Service
@Slf4j
public class CNIFaceDetectService {

    @GrpcClient("cniface")
    private DetectServiceGrpc.DetectServiceBlockingStub detectServiceStub;

    private final CNIFaceModelProperties cniFaceModelProperties;

    public CNIFaceDetectService(CNIFaceModelProperties cniFaceModelProperties) {
        this.cniFaceModelProperties = cniFaceModelProperties;
    }

    public DetectResponse detect(DetectRequest detectRequest) throws StatusRuntimeException {
        if (detectRequest.getModel() == null) {
            detectRequest = detectRequest.toBuilder()
                    .setModel(cniFaceModelProperties.getDetect().toString().toLowerCase())
                    .build();
        }
        return detectServiceStub.detect(detectRequest);
    }
}
