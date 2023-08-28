package cn.abellee.cniface.platform.security.service;

import cn.abellee.cniface.platform.security.entity.Authority;
import cn.abellee.cniface.platform.security.repository.AuthorityRepository;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/8/24 9:24 PM
 */
@Service
public class AuthorityService {

    private final AuthorityRepository authorityRepository;

    public AuthorityService(AuthorityRepository authorityRepository) {
        this.authorityRepository = authorityRepository;
    }

    public Authority add(Authority authority) {
        return authorityRepository.save(authority);
    }
}
