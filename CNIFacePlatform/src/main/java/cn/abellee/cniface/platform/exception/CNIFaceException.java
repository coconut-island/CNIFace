package cn.abellee.cniface.platform.exception;

import cn.abellee.cniface.platform.domain.common.CNIFaceResponseCode;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

/**
 * @author abel
 * @date 2022/6/5 5:45 PM
 */
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
@Data
public class CNIFaceException extends RuntimeException {

    private int code;

    private String message;

    public CNIFaceException(CNIFaceResponseCode cniFaceResponseCode) {
        this.code = cniFaceResponseCode.getCode();
        this.message = cniFaceResponseCode.getDescription();
    }

    public CNIFaceException(CNIFaceResponseCode cniFaceResponseCode, String message) {
        super(message);
        this.code = cniFaceResponseCode.getCode();
        this.message = message != null && !message.isBlank() ? message : cniFaceResponseCode.getDescription();
    }

    public CNIFaceException(int code, String message) {
        super(message);
        this.code = code;
        this.message = message;
    }

}