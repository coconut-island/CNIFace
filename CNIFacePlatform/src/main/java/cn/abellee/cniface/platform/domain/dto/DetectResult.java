package cn.abellee.cniface.platform.domain.dto;

import lombok.Data;

import java.util.List;

/**
 * @author abel
 * @date 2022/6/5 5:38 PM
 */
@Data
public class DetectResult {

    public DetectResult(cn.abellee.cniface.grpc.DetectResult detectResult) {
        this.maskScore = detectResult.getMaskScore();
        this.score = detectResult.getScore();
        this.x = detectResult.getX();
        this.y = detectResult.getY();
        this.w = detectResult.getW();
        this.h = detectResult.getH();
        this.kps = detectResult.getKpsList();
        this.subImageBase64 = detectResult.getSubImageBase64();
    }

    private float maskScore;

    private float score;

    private float x;

    private float y;

    private float w;

    private float h;

    private List<Float> kps;

    private String subImageBase64;
}
