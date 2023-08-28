package cn.abellee.cniface.platform.security.configuration;

import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.Collection;
import java.util.Set;
import java.util.stream.Collectors;

@Service(value = "auth")
public class AuthPermissionConfig {

    public boolean check(String... permissions) {
        final Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        if (authentication.isAuthenticated()) {
            final Collection<? extends GrantedAuthority> authorities = authentication.getAuthorities();
            final Set<String> curPermissions = authorities.stream().map(GrantedAuthority::getAuthority).collect(Collectors.toSet());
            return curPermissions.contains("admin") || Arrays.stream(permissions).anyMatch(curPermissions::contains);
        }
        return false;
    }

}
