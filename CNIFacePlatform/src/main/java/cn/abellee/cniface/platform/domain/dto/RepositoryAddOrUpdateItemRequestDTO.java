package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

/**
 * @author abel
 * @date 2022/8/29 11:07 PM
 */
@Data
public class RepositoryAddOrUpdateItemRequestDTO {

    private Long repoId;

    private Integer id;

    private String name;

    private String personId;

    private String imageBase64;

    private Float detectScore;
}
