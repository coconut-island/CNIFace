package cn.abellee.cniface.platform.service.impl;

import cn.abellee.cniface.grpc.OCRRequest;
import cn.abellee.cniface.grpc.OCRResponse;
import cn.abellee.cniface.grpc.OCRResponseItem;
import cn.abellee.cniface.platform.domain.dto.OCRRequestDTO;
import cn.abellee.cniface.platform.domain.dto.OCRResponseDTO;
import cn.abellee.cniface.platform.domain.dto.OCRResultDTO;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.service.IOCRService;
import cn.abellee.cniface.platform.service.cni.CNIFaceOCRService;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * @author abel
 * @date 2022/11/6 9:23 PM
 */
@Service
public class OCRServiceImpl implements IOCRService {

    private final CNIFaceOCRService cniFaceOCRService;

    public OCRServiceImpl(CNIFaceOCRService cniFaceOCRService) {
        this.cniFaceOCRService = cniFaceOCRService;
    }

    @Override
    public OCRResponseDTO ocr(OCRRequestDTO requestDTO) {
        OCRRequest ocrRequest = OCRRequest.newBuilder()
                .setImageBase64(requestDTO.getImageBase64())
                .setIsCls(requestDTO.getIsCls())
                .setIsRec(requestDTO.getIsRec())
                .build();
        OCRResponse ocrResponse = cniFaceOCRService.ocr(ocrRequest);
        if (ocrResponse.getCode() != 0) throw new CNIFaceException(ocrResponse.getCode(), ocrResponse.getMessage());

        OCRResponseDTO ocrResponseDTO = new OCRResponseDTO();

        List<OCRResultDTO> ocrResultDTOList = new ArrayList<>();

        for (OCRResponseItem ocrResponseItem : ocrResponse.getResultsList()) {
            OCRResultDTO ocrResultDTO = new OCRResultDTO();
            ocrResultDTO.setPosition(ocrResponseItem.getPositionList());
            ocrResultDTO.setText(ocrResponseItem.getText());
            ocrResultDTO.setScore(ocrResponseItem.getScore());
            ocrResultDTO.setClsLabel(ocrResponseItem.getClsLabel());
            ocrResultDTO.setClsScore(ocrResponseItem.getClsScore());
            ocrResultDTOList.add(ocrResultDTO);
        }

        ocrResponseDTO.setResults(ocrResultDTOList);

        return ocrResponseDTO;
    }
}
