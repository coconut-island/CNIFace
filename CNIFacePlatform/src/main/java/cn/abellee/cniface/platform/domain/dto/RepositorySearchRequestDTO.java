package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/8/29 11:09 PM
 */
@Data
public class RepositorySearchRequestDTO {

    private Long repoId;

    private Integer topk;

    private String imageBase64;

    private Float detectScore;

    private List<Float> feature;
}
