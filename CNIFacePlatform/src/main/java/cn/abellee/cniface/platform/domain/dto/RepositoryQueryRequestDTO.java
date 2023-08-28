package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

/**
 * @author abel
 * @date 2022/8/29 11:09 PM
 */
@Data
public class RepositoryQueryRequestDTO {

    private Long repoId;

    private String name;

    private String personId;

    private Long startTimestamp;

    private Long endTimestamp;

    private Integer page;

    private Integer size;
}
