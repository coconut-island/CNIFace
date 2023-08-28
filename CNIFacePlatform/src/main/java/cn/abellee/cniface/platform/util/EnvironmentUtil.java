package cn.abellee.cniface.platform.util;

import java.util.Objects;

/**
 * @author abel
 * @date 2022/6/5 5:49 PM
 */
public class EnvironmentUtil {

    public static boolean isStartupFromJar() {
        String protocol = EnvironmentUtil.class.getResource("").getProtocol();
        if (Objects.equals(protocol, "jar")) {
            return true;
        }
        return !Objects.equals(protocol, "file");
    }

    public static boolean isLinux() {
        return System.getProperty("os.name").contains("linux");
    }
}
