package cn.abellee.cniface.platform.contoller;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.domain.dto.ExtractFeatureRequestDTO;
import cn.abellee.cniface.platform.domain.dto.ExtractFeatureResponseDTO;
import cn.abellee.cniface.platform.domain.dto.SimilarityRequestDTO;
import cn.abellee.cniface.platform.domain.dto.SimilarityResponseDTO;
import cn.abellee.cniface.platform.service.IRecognitionService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author abel
 * @date 2022/6/6 7:50 PM
 */
@RestController
@RequestMapping("/recognition")
public class RecognitionController {

    private final IRecognitionService recognitionService;

    public RecognitionController(IRecognitionService recognitionService) {
        this.recognitionService = recognitionService;
    }

    @PostMapping("/extract_feature")
    public CNIFaceResponse<ExtractFeatureResponseDTO> extractFeature(@RequestBody ExtractFeatureRequestDTO extractFeatureRequest){
        ExtractFeatureResponseDTO extractFeatureResponseDTO = recognitionService.extractFeature(extractFeatureRequest);
        return CNIFaceResponse.ok(extractFeatureResponseDTO);
    }

    @PostMapping("/similarity")
    public CNIFaceResponse<SimilarityResponseDTO> similarity(@RequestBody SimilarityRequestDTO similarityRequest){
        SimilarityResponseDTO similarityResponseDTO = recognitionService.similarity(similarityRequest);
        return CNIFaceResponse.ok(similarityResponseDTO);
    }
}
