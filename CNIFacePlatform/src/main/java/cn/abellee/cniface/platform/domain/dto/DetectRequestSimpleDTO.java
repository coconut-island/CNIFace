package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

/**
 * @author abel
 * @date 2022/8/24 5:47 PM
 */
@Data
public class DetectRequestSimpleDTO {

    private String faceImageBase64;

    private float score;

    private Boolean isReturnRecognitionSubImage = true;
}
