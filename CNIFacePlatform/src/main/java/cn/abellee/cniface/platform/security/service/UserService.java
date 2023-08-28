package cn.abellee.cniface.platform.security.service;

import cn.abellee.cniface.platform.security.SecurityUtils;
import cn.abellee.cniface.platform.security.entity.User;
import cn.abellee.cniface.platform.security.repository.UserRepository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Optional;

/**
 * @author abel
 * @date 2022/8/16 9:20 PM
 */
@Service
@Transactional
public class UserService {

    private final UserRepository userRepository;

    public UserService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    public boolean existUserByUsername(String username) {
        return userRepository.existsUserByUsername(username);
    }

    @Transactional(readOnly = true)
    public Optional<User> getUserWithAuthorities() {
        return SecurityUtils.getCurrentUsername().flatMap(userRepository::findOneWithAuthoritiesByUsername);
    }

    public User add(User user) {
        return userRepository.save(user);
    }

}
