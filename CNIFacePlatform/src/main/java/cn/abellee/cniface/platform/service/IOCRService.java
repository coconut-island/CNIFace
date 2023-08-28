package cn.abellee.cniface.platform.service;

import cn.abellee.cniface.platform.domain.dto.OCRRequestDTO;
import cn.abellee.cniface.platform.domain.dto.OCRResponseDTO;

/**
 * @author abel
 * @date 2022/11/6 9:19 PM
 */
public interface IOCRService {

    OCRResponseDTO ocr(OCRRequestDTO requestDTO);
}
