package cn.abellee.cniface.platform.security.rest.dto;

import lombok.Data;

import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

/**
 * @author abel
 * @date 2022/8/16 9:16 PM
 */
@Data
public class LoginDTO {

    @NotNull
    @Size(min = 1, max = 50)
    private String username;

    @NotNull
    @Size(min = 4, max = 100)
    private String password;

    private Boolean rememberMe;

    public Boolean isRememberMe() {
        return rememberMe;
    }
}
