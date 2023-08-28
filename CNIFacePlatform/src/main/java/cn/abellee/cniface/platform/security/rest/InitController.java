package cn.abellee.cniface.platform.security.rest;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.security.annotaion.AnonymousAccess;
import cn.abellee.cniface.platform.security.rest.dto.RegisterAdminDTO;
import cn.abellee.cniface.platform.security.service.InitService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.validation.Valid;

/**
 * @author abel
 * @date 2022/8/24 7:04 PM
 */
@RestController
@RequestMapping
public class InitController {

    private final InitService initService;

    public InitController(InitService initService) {
        this.initService = initService;
    }


    @AnonymousAccess
    @PostMapping("/init_and_register_admin")
    public ResponseEntity<CNIFaceResponse<?>> initAndRegisterAdmin(@Valid @RequestBody RegisterAdminDTO registerAdminDTO) {
        initService.initAndRegisterAdmin(registerAdminDTO);
        return ResponseEntity.ok(CNIFaceResponse.ok());
    }
}
