package cn.abellee.cniface.platform.security.rest;


import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.security.annotaion.AnonymousAccess;
import cn.abellee.cniface.platform.security.jwt.JWTFilter;
import cn.abellee.cniface.platform.security.jwt.TokenProvider;
import cn.abellee.cniface.platform.security.rest.dto.LoginDTO;
import cn.abellee.cniface.platform.security.rest.dto.LoginResultDTO;
import cn.abellee.cniface.platform.security.service.UserService;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.validation.Valid;


/**
 * Controller to authenticate users.
 * @author abel
 * @date 2022/8/16 9:17 PM
 */
@RestController
@RequestMapping
public class LoginController {

    private final TokenProvider tokenProvider;

    private final AuthenticationManagerBuilder authenticationManagerBuilder;

    private final UserService userService;

    public LoginController(TokenProvider tokenProvider, AuthenticationManagerBuilder authenticationManagerBuilder, UserService userService) {
        this.tokenProvider = tokenProvider;
        this.authenticationManagerBuilder = authenticationManagerBuilder;
        this.userService = userService;
    }

    @AnonymousAccess
    @PostMapping("/login")
    public ResponseEntity<CNIFaceResponse<LoginResultDTO>> login(@Valid @RequestBody LoginDTO loginDto) {

        if ("admin".equals(loginDto.getUsername())) {
            if (!userService.existUserByUsername(loginDto.getUsername())) {
                return ResponseEntity.ok(CNIFaceResponse.error(-10, "admin user not found!"));
            }
        }

        UsernamePasswordAuthenticationToken authenticationToken =
                new UsernamePasswordAuthenticationToken(loginDto.getUsername(), loginDto.getPassword());

        Authentication authentication = authenticationManagerBuilder.getObject().authenticate(authenticationToken);
        SecurityContextHolder.getContext().setAuthentication(authentication);

        boolean rememberMe = (loginDto.isRememberMe() == null) ? false : loginDto.isRememberMe();
        String jwt = tokenProvider.createToken(authentication, rememberMe);

        HttpHeaders httpHeaders = new HttpHeaders();
        httpHeaders.add(JWTFilter.AUTHORIZATION_HEADER, jwt);

        return new ResponseEntity<>(CNIFaceResponse.ok(LoginResultDTO.builder().token(jwt).build()), httpHeaders, HttpStatus.OK);
    }

}
