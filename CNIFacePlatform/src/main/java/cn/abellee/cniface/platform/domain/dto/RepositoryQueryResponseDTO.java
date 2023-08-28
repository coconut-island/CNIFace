package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/10/3 11:51 PM
 */
@Data
public class RepositoryQueryResponseDTO {

    private List<RepositoryQueryResultDTO> results;

    private Integer total;
}
