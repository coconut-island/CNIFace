package cn.abellee.cniface.platform.contoller;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.domain.dto.*;
import cn.abellee.cniface.platform.service.IAttributeService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author abel
 * @date 2022/8/28 11:30 AM
 */
@RestController
@RequestMapping("/attribute")
public class AttributeController {

    private final IAttributeService attributeService;

    public AttributeController(IAttributeService attributeService) {
        this.attributeService = attributeService;
    }

    @PostMapping("/gender_age")
    public CNIFaceResponse<GenderAgeResult> detect(@RequestBody GenderAgeRequestDTO genderAgeRequestDTO){
        GenderAgeResult genderAgeResult = attributeService.genderAge(genderAgeRequestDTO);
        return CNIFaceResponse.ok(genderAgeResult);
    }
}
