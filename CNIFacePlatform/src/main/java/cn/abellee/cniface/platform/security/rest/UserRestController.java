package cn.abellee.cniface.platform.security.rest;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.security.UserNotFoundException;
import cn.abellee.cniface.platform.security.entity.User;
import cn.abellee.cniface.platform.security.rest.dto.CurrentUserResultDTO;
import cn.abellee.cniface.platform.security.service.UserService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * @author abel
 * @date 2022/8/16 9:19 PM
 */
@RestController
@RequestMapping
public class UserRestController {

    private final UserService userService;

    public UserRestController(UserService userService) {
        this.userService = userService;
    }

    @GetMapping("/currentUser")
    public ResponseEntity<CNIFaceResponse<CurrentUserResultDTO>> currentUser() {
        User user = userService.getUserWithAuthorities().orElseThrow(UserNotFoundException::new);
        CurrentUserResultDTO currentUserResultDTO = new CurrentUserResultDTO(user);
        return ResponseEntity.ok(CNIFaceResponse.ok(currentUserResultDTO));
    }
}
