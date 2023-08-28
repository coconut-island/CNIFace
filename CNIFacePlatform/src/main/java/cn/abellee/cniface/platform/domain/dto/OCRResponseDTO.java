package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/11/6 9:20 PM
 */
@Data
public class OCRResponseDTO {

    private List<OCRResultDTO> results;
}
