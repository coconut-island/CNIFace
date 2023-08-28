package cn.abellee.cniface.platform.service.impl;

import cn.abellee.cniface.grpc.*;
import cn.abellee.cniface.platform.domain.dto.*;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.service.IRecognitionService;
import cn.abellee.cniface.platform.service.cni.CNIFaceRecognitionService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/6/6 7:42 PM
 */
@Service
@Slf4j
public class RecognitionServiceImpl implements IRecognitionService {

    private final CNIFaceRecognitionService cniFaceRecognitionService;

    public RecognitionServiceImpl(CNIFaceRecognitionService cniFaceRecognitionService) {
        this.cniFaceRecognitionService = cniFaceRecognitionService;
    }

    private float similarityCal(float similarity) {
        similarity = similarity < 0 ? 0 : similarity;
        similarity = similarity > 1 ? 1 : similarity;
        similarity = similarity * 100;
        return similarity;
    }

    @Override
    public ExtractFeatureResponseDTO extractFeature(ExtractFeatureRequestDTO extractFeatureRequestDTO) {
        ExtractFeatureRequest extractFeatureRequest = ExtractFeatureRequest.newBuilder()
                .addAllKps(extractFeatureRequestDTO.getKps())
                .setFaceImageBase64(extractFeatureRequestDTO.getFaceImageBase64())
                .setIsDoNormalizeL2(extractFeatureRequestDTO.getIsDoNormalizeL2())
                .build();

        ExtractFeatureResponse extractFeatureResponse = cniFaceRecognitionService.extractFeature(extractFeatureRequest);
        if (extractFeatureResponse.getCode() != 0) throw new CNIFaceException(extractFeatureResponse.getCode(), extractFeatureResponse.getMessage());
        ExtractFeatureResponseDTO extractFeatureResponseDTO = new ExtractFeatureResponseDTO();
        extractFeatureResponseDTO.setFeature(extractFeatureResponse.getFeatureList());

        return extractFeatureResponseDTO;
    }

    @Override
    public SimilarityResponseDTO similarity(SimilarityRequestDTO similarityRequestDTO) {
        SimilarityRequest similarityRequest = SimilarityRequest.newBuilder()
                .addAllFeature1(similarityRequestDTO.getFeature1())
                .addAllFeature2(similarityRequestDTO.getFeature2())
                .build();
        SimilarityResponse similarityResponse = cniFaceRecognitionService.similarity(similarityRequest);
        if (similarityResponse.getCode() != 0) throw new CNIFaceException(similarityResponse.getCode(), similarityResponse.getMessage());
        float similarity = similarityResponse.getSimilarity();
        similarity = similarityCal(similarity);
        SimilarityResponseDTO similarityResponseDTO = new SimilarityResponseDTO();
        similarityResponseDTO.setSimilarity(similarity);
        return similarityResponseDTO;
    }

}
