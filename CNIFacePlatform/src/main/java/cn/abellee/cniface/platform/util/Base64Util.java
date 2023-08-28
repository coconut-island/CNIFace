package cn.abellee.cniface.platform.util;

import java.nio.charset.Charset;
import java.util.Base64;

import static java.nio.charset.StandardCharsets.UTF_8;

/**
 * @author abel
 * @date 2022/6/5 5:49 PM
 */
public class Base64Util {
    private static final Base64.Decoder DECODER = Base64.getDecoder();
    private static final Base64.Encoder ENCODER = Base64.getEncoder();
    private static final Base64.Decoder URL_DECODER = Base64.getUrlDecoder();
    private static final Base64.Encoder URL_ENCODER = Base64.getUrlEncoder();
    private static final Charset DEFAULT_CHARSET;

    public static byte[] encode(byte[] src) {
        return src.length == 0 ? src : ENCODER.encode(src);
    }

    public static String encodeToString(String src) {
        return encodeToString(src.getBytes());
    }

    public static byte[] decode(byte[] src) {
        return src.length == 0 ? src : DECODER.decode(src);
    }

    public static byte[] encodeUrlSafe(byte[] src) {
        return src.length == 0 ? src : URL_ENCODER.encode(src);
    }

    public static byte[] decodeUrlSafe(byte[] src) {
        return src.length == 0 ? src : URL_DECODER.decode(src);
    }

    public static String encodeToString(byte[] src) {
        return src.length == 0 ? "" : new String(encode(src), DEFAULT_CHARSET);
    }

    public static byte[] decodeFromString(String src) {
        return src.isEmpty() ? new byte[0] : decode(src.getBytes(DEFAULT_CHARSET));
    }

    public static String encodeToUrlSafeString(byte[] src) {
        return new String(encodeUrlSafe(src), DEFAULT_CHARSET);
    }

    public static byte[] decodeFromUrlSafeString(String src) {
        return decodeUrlSafe(src.getBytes(DEFAULT_CHARSET));
    }

    public static String decodeStringFromString(String src) {
        return src.isEmpty() ? "" : new String(decode(src.getBytes(DEFAULT_CHARSET)), DEFAULT_CHARSET);
    }

    static {
        DEFAULT_CHARSET = UTF_8;
    }
}

