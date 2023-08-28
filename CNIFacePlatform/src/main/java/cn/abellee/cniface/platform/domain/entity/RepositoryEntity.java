package cn.abellee.cniface.platform.domain.entity;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

import javax.persistence.*;

/**
 * @author abel
 * @date 2022/10/3 8:31 PM
 */
@Data
@Entity
@Table(name = "repositories", indexes = {
        @Index(name = "name_createTime", columnList = "name,create_time"),
})
@ToString
@EqualsAndHashCode(callSuper = true)
public class RepositoryEntity extends BaseTimeEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "name", nullable = false)
    private String name;

    @Column(name = "total", nullable = false)
    private Integer total;

    public String getRepoNameInDB() {
        return "repo_" + id;
    }
}
