package cn.abellee.cniface.platform.security.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;

/**
 * @author abel
 * @date 2022/8/16 9:11 PM
 */
@Data
@Entity
@Table(name = "authority")
public class Authority {

    @Id
    @Column(name = "name", length = 50)
    @NotNull
    private String name;

}
