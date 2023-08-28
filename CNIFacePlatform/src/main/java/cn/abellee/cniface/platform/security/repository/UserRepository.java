package cn.abellee.cniface.platform.security.repository;

import cn.abellee.cniface.platform.security.entity.User;
import org.springframework.data.jpa.repository.EntityGraph;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.Optional;

/**
 * @author abel
 * @date 2022/8/16 9:14 PM
 */
public interface UserRepository extends JpaRepository<User, Long> {

    @EntityGraph(attributePaths = "authorities")
    Optional<User> findOneWithAuthoritiesByUsername(String username);

    Boolean existsUserByUsername(String username);
}
