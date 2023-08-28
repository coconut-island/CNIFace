package cn.abellee.cniface.platform.security.service;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponseCode;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.security.entity.Authority;
import cn.abellee.cniface.platform.security.entity.User;
import cn.abellee.cniface.platform.security.rest.dto.RegisterAdminDTO;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Set;

/**
 * @author abel
 * @date 2022/8/24 10:13 PM
 */
@Service
public class InitService {

    private final UserService userService;

    private final AuthorityService authorityService;

    private final BCryptPasswordEncoder bCryptPasswordEncoder;

    public InitService(UserService userService, AuthorityService authorityService, BCryptPasswordEncoder bCryptPasswordEncoder) {
        this.userService = userService;
        this.authorityService = authorityService;
        this.bCryptPasswordEncoder = bCryptPasswordEncoder;
    }

    @Transactional
    public void initAndRegisterAdmin(RegisterAdminDTO registerAdminDTO) {
        if (userService.existUserByUsername("admin")) {
            throw new CNIFaceException(CNIFaceResponseCode.ADMIN_ACCOUNT_ALREADY_EXIST);
        }

        Authority adminAuthority = new Authority();
        adminAuthority.setName("ROLE_ADMIN");
        authorityService.add(adminAuthority);

        Authority userAuthority = new Authority();
        userAuthority.setName("ROLE_USER");
        authorityService.add(userAuthority);


        User adminUser = new User();
        adminUser.setUsername("admin");
        adminUser.setPassword(bCryptPasswordEncoder.encode(registerAdminDTO.getPassword()));
        adminUser.setActivated(true);
        adminUser.setAuthorities(Set.of(adminAuthority));
        userService.add(adminUser);
    }
}
