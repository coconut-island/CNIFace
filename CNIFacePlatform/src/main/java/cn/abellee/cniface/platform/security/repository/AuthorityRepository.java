package cn.abellee.cniface.platform.security.repository;

/**
 * @author abel
 * @date 2022/8/16 9:15 PM
 */

import cn.abellee.cniface.platform.security.entity.Authority;
import org.springframework.data.jpa.repository.JpaRepository;

/**
 * Spring Data JPA repository for the {@link Authority} entity.
 */
public interface AuthorityRepository extends JpaRepository<Authority, String> {
}
