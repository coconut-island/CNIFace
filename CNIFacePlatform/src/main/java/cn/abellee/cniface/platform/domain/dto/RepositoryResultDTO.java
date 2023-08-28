package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

/**
 * @author abel
 * @date 2022/8/20 9:42 PM
 */
@Data
public class RepositoryResultDTO {

    private Long id;

    private String name;

    private Integer total;

    private Long createTime;

    private Long updateTime;
}
