package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/6 7:57 PM
 */
@Data
public class SimilarityRequestDTO {

    private List<Float> feature1;

    private List<Float> feature2;
}
