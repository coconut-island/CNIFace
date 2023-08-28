package cn.abellee.cniface.platform.domain.common;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * @author abel
 * @date 2022/6/5 5:43 PM
 */
@Getter
@AllArgsConstructor
public enum CNIFaceResponseCode {

    OK(0, "OK"),
    NOT_SUPPORT(-2, "功能未支持"),
    ADMIN_ACCOUNT_ALREADY_EXIST(-5, "admin 用户已经存在!"),
    CNIFACE_GRPC_RETURN_ERROR(-100, "CNIFace grpc 返回值不为0"),
    CNIFACE_GRPC_ERROR(-500, "CNIFace grpc 链接错误"),
    VALID_ERROR(-22000, "请求参数不符合规范"),

    READ_IMAGE_ERROR(-30100, "读取图片失败"),

    NO_FACE_DETECTED(-50100, "未检测到人脸"),
    MULTIPLE_FACES_DETECTED(-50101, "检测到多张人脸"),

    ;


    private final int code;

    private final String description;
}
