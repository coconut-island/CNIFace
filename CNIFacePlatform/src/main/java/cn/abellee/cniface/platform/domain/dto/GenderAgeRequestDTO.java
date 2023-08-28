package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/8/28 11:26 AM
 */
@Data
public class GenderAgeRequestDTO {

    private String faceImageBase64;

    private List<Float> kps;
}
