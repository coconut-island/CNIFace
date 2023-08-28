package cn.abellee.cniface.platform.config;

import cn.abellee.cniface.platform.interceptor.CNIFaceGrpcInterceptor;
import net.devh.boot.grpc.client.interceptor.GrpcGlobalClientInterceptor;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.annotation.Order;

/**
 * @author abel
 * @date 2022/6/5 5:22 PM
 */
@Order()
@Configuration(proxyBeanMethods = false)
public class GlobalClientInterceptorConfiguration {

    @GrpcGlobalClientInterceptor
    CNIFaceGrpcInterceptor cniFaceGrpcInterceptor() {
        return new CNIFaceGrpcInterceptor();
    }

}