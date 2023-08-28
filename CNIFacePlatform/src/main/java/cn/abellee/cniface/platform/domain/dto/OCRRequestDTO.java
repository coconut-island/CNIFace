package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

/**
 * @author abel
 * @date 2022/11/6 9:20 PM
 */
@Data
public class OCRRequestDTO {

    private String imageBase64;

    private Boolean isCls = true;

    private Boolean isRec = true;
}
