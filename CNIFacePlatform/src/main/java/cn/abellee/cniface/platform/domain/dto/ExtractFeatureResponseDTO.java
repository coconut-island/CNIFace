package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/6 7:39 PM
 */
@Data
public class ExtractFeatureResponseDTO {

    private List<Float> feature;

}
