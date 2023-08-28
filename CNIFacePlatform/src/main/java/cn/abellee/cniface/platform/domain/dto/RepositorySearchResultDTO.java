package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/10/3 11:52 PM
 */
@Data
public class RepositorySearchResultDTO {

    private Long id;

    private String name;

    private String personId;

    private Float score;

    private List<Float> feature;
}
