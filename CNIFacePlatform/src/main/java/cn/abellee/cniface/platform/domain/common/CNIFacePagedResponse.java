package cn.abellee.cniface.platform.domain.common;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/5 5:44 PM
 */
@JsonInclude(JsonInclude.Include.NON_NULL)
@JsonIgnoreProperties(ignoreUnknown = true)
@Data
public class CNIFacePagedResponse<T> {

    private List<T> result;

    private int code;

    private String message;

    private int total;

    public static <R> CNIFacePagedResponse<R> ok() {
        return new CNIFacePagedResponse<>(CNIFaceResponseCode.OK);
    }

    public static <R> CNIFacePagedResponse<R> ok(int total, List<R> r) {
        return new CNIFacePagedResponse<>(CNIFaceResponseCode.OK, total, r);
    }

    public static <R> CNIFacePagedResponse<R> error(CNIFaceResponseCode cnifaceResponseCode, String message) {
        return new CNIFacePagedResponse<>(cnifaceResponseCode, message);
    }

    public static <R> CNIFacePagedResponse<R> error(CNIFaceResponseCode cnifaceResponseCode, String message, int total, List<R> r) {
        return new CNIFacePagedResponse<>(cnifaceResponseCode, message, total, r);
    }

    private CNIFacePagedResponse(CNIFaceResponseCode cnifaceResponseCode) {
        this.code = cnifaceResponseCode.getCode();
        this.message = cnifaceResponseCode.getDescription();
        this.total = 0;
    }

    private CNIFacePagedResponse(CNIFaceResponseCode cnifaceResponseCode, String message) {
        this.code = cnifaceResponseCode.getCode();
        this.message = message;
        this.total = 0;
    }

    private CNIFacePagedResponse(CNIFaceResponseCode cnifaceResponseCode, int total, List<T> t) {
        this.code = cnifaceResponseCode.getCode();
        this.message = cnifaceResponseCode.getDescription();
        this.total = total;
        this.result = t;
    }

    private CNIFacePagedResponse(CNIFaceResponseCode cnifaceResponseCode, String message, int total, List<T> t) {
        this.code = cnifaceResponseCode.getCode();
        this.message = message;
        this.total = total;
        this.result = t;
    }

}

