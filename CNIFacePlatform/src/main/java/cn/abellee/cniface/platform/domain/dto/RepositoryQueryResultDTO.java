package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/10/3 11:52 PM
 */
@Data
public class RepositoryQueryResultDTO {

    private Long id;

    private String name;

    private String personId;

    private List<Float> feature;

    private Long timestamp;
}
