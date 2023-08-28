package cn.abellee.cniface.platform.security.configuration;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;

/**
 * @author abel
 * @date 2022/8/17 1:01 AM
 */
@Data
@Configuration
@ConfigurationProperties("jwt")
public class JWTProperties {

    private String base64Secret;

    private Integer tokenValidityInSeconds;

    private Integer tokenValidityInSecondsForRememberMe;
}
