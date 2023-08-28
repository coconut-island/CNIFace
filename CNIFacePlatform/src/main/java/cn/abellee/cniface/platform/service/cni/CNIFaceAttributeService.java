package cn.abellee.cniface.platform.service.cni;

import cn.abellee.cniface.grpc.*;
import io.grpc.StatusRuntimeException;
import net.devh.boot.grpc.client.inject.GrpcClient;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/8/28 11:22 AM
 */
@Service
public class CNIFaceAttributeService {

    @GrpcClient("cniface")
    private AttributeServiceGrpc.AttributeServiceBlockingStub attributeServiceBlockingStub;

    public GenderAgeResponse genderAge(final GenderAgeRequest genderAgeRequest) throws StatusRuntimeException {
        return attributeServiceBlockingStub.genderAge(genderAgeRequest);
    }
}
