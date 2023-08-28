package cn.abellee.cniface.platform.security.rest.dto;

import cn.abellee.cniface.platform.security.entity.Authority;
import cn.abellee.cniface.platform.security.entity.User;
import lombok.Data;

import java.util.List;
import java.util.stream.Collectors;

/**
 * @author abel
 * @date 2022/8/17 12:03 AM
 */
@Data
public class CurrentUserResultDTO {

    public CurrentUserResultDTO(User user) {
        this.username = user.getUsername();
        this.authorities = user.getAuthorities().stream().map(
                authority -> authority.getName().replace("ROLE_", "").toLowerCase())
                .collect(Collectors.toList());
    }

    private String username;

    private List<String> authorities;
}
