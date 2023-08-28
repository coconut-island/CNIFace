package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/8/20 10:03 PM
 */
@Data
public class DeleteRepositoryDTO {

    private List<Long> ids;
}
