package cn.abellee.cniface.platform.contoller;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.domain.dto.*;
import cn.abellee.cniface.platform.service.IOCRService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author abel
 * @date 2022/11/6 8:46 PM
 */
@RestController
@RequestMapping("/ocr")
public class OCRController {

    private final IOCRService ocrService;

    public OCRController(IOCRService ocrService) {
        this.ocrService = ocrService;
    }

    @PostMapping("/ocr")
    public CNIFaceResponse<OCRResponseDTO> ocr(@RequestBody OCRRequestDTO ocrRequestDTO){
        OCRResponseDTO ocrResponseDTO = ocrService.ocr(ocrRequestDTO);
        return CNIFaceResponse.ok(ocrResponseDTO);
    }
}
