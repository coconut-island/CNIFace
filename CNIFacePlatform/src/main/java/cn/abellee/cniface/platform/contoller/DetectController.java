package cn.abellee.cniface.platform.contoller;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.domain.dto.DetectRequestDTO;
import cn.abellee.cniface.platform.domain.dto.DetectRequestSimpleDTO;
import cn.abellee.cniface.platform.domain.dto.DetectResult;
import cn.abellee.cniface.platform.service.IDetectService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/5 3:21 PM
 */
@RestController
@RequestMapping("/detect")
public class DetectController {

    private final IDetectService detectService;

    public DetectController(IDetectService detectService) {
        this.detectService = detectService;
    }

    @PostMapping("/face")
    public CNIFaceResponse<List<DetectResult>> detect(@RequestBody DetectRequestSimpleDTO detectRequestSimpleDTO){
        DetectRequestDTO detectRequestDTO = new DetectRequestDTO(detectRequestSimpleDTO);
        List<DetectResult> detectResults = detectService.detect(detectRequestDTO);
        return CNIFaceResponse.ok(detectResults);
    }


}
