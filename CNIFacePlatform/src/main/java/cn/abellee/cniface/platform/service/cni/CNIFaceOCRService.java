package cn.abellee.cniface.platform.service.cni;

import cn.abellee.cniface.grpc.*;
import io.grpc.StatusRuntimeException;
import net.devh.boot.grpc.client.inject.GrpcClient;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/11/6 9:15 PM
 */
@Service
public class CNIFaceOCRService {

    @GrpcClient("cniface")
    private OCRServiceGrpc.OCRServiceBlockingStub ocrServiceStub;

    public OCRResponse ocr(OCRRequest ocrRequest) throws StatusRuntimeException {
        return ocrServiceStub.ocr(ocrRequest);
    }
}
