package cn.abellee.cniface.platform.service.impl;

import cn.abellee.cniface.grpc.GenderAgeRequest;
import cn.abellee.cniface.grpc.GenderAgeResponse;
import cn.abellee.cniface.platform.domain.dto.GenderAgeRequestDTO;
import cn.abellee.cniface.platform.domain.dto.GenderAgeResult;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.service.IAttributeService;
import cn.abellee.cniface.platform.service.cni.CNIFaceAttributeService;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/8/28 11:31 AM
 */
@Service
public class AttributeServiceImpl implements IAttributeService {

    private final CNIFaceAttributeService cniFaceAttributeService;

    public AttributeServiceImpl(CNIFaceAttributeService cniFaceAttributeService) {
        this.cniFaceAttributeService = cniFaceAttributeService;
    }


    @Override
    public GenderAgeResult genderAge(GenderAgeRequestDTO genderAgeRequestDTO) {
        GenderAgeRequest genderAgeRequest = GenderAgeRequest.newBuilder()
                .addAllKps(genderAgeRequestDTO.getKps())
                .setFaceImageBase64(genderAgeRequestDTO.getFaceImageBase64())
                .build();
        GenderAgeResponse genderAgeResponse = cniFaceAttributeService.genderAge(genderAgeRequest);
        if (genderAgeResponse.getCode() != 0) throw new CNIFaceException(genderAgeResponse.getCode(), genderAgeResponse.getMessage());
        GenderAgeResult genderAgeResult = new GenderAgeResult();
        genderAgeResult.setGender(genderAgeResponse.getGender());
        genderAgeResult.setAge(genderAgeResponse.getAge());
        return genderAgeResult;
    }
}
