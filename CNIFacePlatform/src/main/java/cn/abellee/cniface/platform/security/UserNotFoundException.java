package cn.abellee.cniface.platform.security;

import lombok.NoArgsConstructor;
import org.springframework.security.core.AuthenticationException;

/**
 * @author abel
 * @date 2022/8/17 12:05 AM
 */
public class UserNotFoundException extends AuthenticationException {

    private static final long serialVersionUID = -1126699074574529145L;

    public UserNotFoundException() {
        super("User not found!");
    };

    public UserNotFoundException(String message) {
        super(message);
    }
}
