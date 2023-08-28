package cn.abellee.cniface.platform.domain.common;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author abel
 * @date 2022/6/5 5:41 PM
 */

@JsonInclude(JsonInclude.Include.NON_NULL)
@JsonIgnoreProperties(ignoreUnknown = true)
@Data
@NoArgsConstructor
@AllArgsConstructor
public class CNIFaceResponse<T> {

    private T result;

    private int code;

    private String message;

    public static <R> CNIFaceResponse<R> ok() {
        return new CNIFaceResponse<>(CNIFaceResponseCode.OK);
    }

    public static <R> CNIFaceResponse<R> ok(R r) {
        return new CNIFaceResponse<>(CNIFaceResponseCode.OK, r);
    }

    public static <R> CNIFaceResponse<R> error(int code, String message) {
        return new CNIFaceResponse<>(code, message);
    }

    public static <R> CNIFaceResponse<R> error(CNIFaceResponseCode cnifaceResponseCode, String message) {
        return new CNIFaceResponse<>(cnifaceResponseCode, message);
    }

    public static <R> CNIFaceResponse<R> error(CNIFaceResponseCode cnifaceResponseCode, String message, R r) {
        return new CNIFaceResponse<>(cnifaceResponseCode, message, r);
    }

    private CNIFaceResponse(int code, String message) {
        this.code = code;
        this.message = message;
    }

    private CNIFaceResponse(CNIFaceResponseCode cnifaceResponseCode, T t) {
        this.code = cnifaceResponseCode.getCode();
        this.message = cnifaceResponseCode.getDescription();
        this.result = t;
    }

    private CNIFaceResponse(CNIFaceResponseCode cnifaceResponseCode) {
        this.code = cnifaceResponseCode.getCode();
        this.message = cnifaceResponseCode.getDescription();
    }

    private CNIFaceResponse(CNIFaceResponseCode cnifaceResponseCode, String message) {
        this.code = cnifaceResponseCode.getCode();
        this.message = message;
    }

    private CNIFaceResponse(CNIFaceResponseCode cnifaceResponseCode, String massage, T t) {
        this.code = cnifaceResponseCode.getCode();
        this.message = massage;
        this.result = t;
    }
}
