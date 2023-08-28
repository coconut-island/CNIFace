package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/11/6 9:20 PM
 */
@Data
public class OCRResultDTO {

    private List<Float> position;

    private float score;

    private String text;

    private Float clsScore;

    private Integer clsLabel;
}
