package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author abel
 * @date 2022/6/5 5:36 PM
 */
@Data
@NoArgsConstructor
public class DetectRequestDTO {

    public DetectRequestDTO(DetectRequestSimpleDTO detectRequestSimpleDTO) {
        this.faceImageBase64 = detectRequestSimpleDTO.getFaceImageBase64();
        this.score = detectRequestSimpleDTO.getScore();
        this.isReturnRecognitionSubImage = detectRequestSimpleDTO.getIsReturnRecognitionSubImage();
    }

    private String model;

    private String faceImageBase64;

    private float score;

    private Boolean isReturnRecognitionSubImage;
}
