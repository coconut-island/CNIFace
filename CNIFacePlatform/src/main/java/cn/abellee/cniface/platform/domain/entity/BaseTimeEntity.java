package cn.abellee.cniface.platform.domain.entity;

import cn.abellee.cniface.platform.util.TimeUtil;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

import javax.persistence.*;

/**
 * @author abel
 * @date 2022/8/20 9:18 PM
 */
@Data
@ToString
@MappedSuperclass
@EqualsAndHashCode
public class BaseTimeEntity {

    @Column(name = "create_time")
    private Long createTime;

    @Column(name = "update_time")
    private Long updateTime;

    @PrePersist
    protected void prePersist() {
        long now = TimeUtil.now();
        if (createTime == null) {
            createTime = now;
        }

        if (updateTime == null) {
            updateTime = now;
        }
    }

    @PreUpdate
    protected void preUpdate() {
        updateTime = TimeUtil.now();
    }

    @PreRemove
    protected void preRemove() {
        updateTime = TimeUtil.now();
    }

}
