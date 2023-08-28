package cn.abellee.cniface.platform.security.rest.dto;

import lombok.Builder;
import lombok.Data;

/**
 * @author abel
 * @date 2022/8/16 10:44 PM
 */
@Data
@Builder
public class LoginResultDTO {

    private String token;
}
