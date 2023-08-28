package cn.abellee.cniface.platform.repository;

import cn.abellee.cniface.platform.domain.entity.RepositoryEntity;
import org.springframework.data.jpa.repository.JpaRepository;

/**
 * @author abel
 * @date 2022/8/20 9:34 PM
 */
public interface RepositoryRepository extends JpaRepository<RepositoryEntity, Long> {
}
