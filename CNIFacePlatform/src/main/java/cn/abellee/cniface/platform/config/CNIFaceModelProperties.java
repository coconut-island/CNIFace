package cn.abellee.cniface.platform.config;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;

/**
 * @author abel
 * @date 2022/11/1 10:21 PM
 */
@Data
@Configuration
@ConfigurationProperties("cniface.model")
public class CNIFaceModelProperties {

    public enum DetectModel {
        DET_10G,
        DET_500M,
        MNET_COV2
    }

    public enum RecognitionModel {
        W600K_R50,
        W600K_MBF
    }

    private DetectModel detect;

    private RecognitionModel recognition;

}
