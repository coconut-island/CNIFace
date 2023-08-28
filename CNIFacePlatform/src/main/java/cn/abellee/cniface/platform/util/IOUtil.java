package cn.abellee.cniface.platform.util;

import java.util.Objects;

/**
 * @author abel
 * @date 2022/6/5 5:50 PM
 */
public class IOUtil {

    public static void streamClose(AutoCloseable... closeables){
        for (AutoCloseable closeable : closeables) {
            if (Objects.nonNull(closeable)) {
                try {
                    closeable.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
