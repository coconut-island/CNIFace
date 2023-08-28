package cn.abellee.cniface.platform.domain.entity;

import lombok.Data;
import lombok.ToString;

import javax.persistence.*;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

/**
 * @author abel
 * @date 2022/8/20 9:27 PM
 */
@Data
@Entity
@Table(name = "repo", indexes = {
        @Index(name = "name_timestamp", columnList = "name,timestamp"),
        @Index(name = "personId_timestamp", columnList = "person_id,timestamp")
})
@ToString
public class FaceEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "name", nullable = false)
    private String name;

    @Column(name = "person_id", nullable = false)
    private String personId;

    @Basic(fetch = FetchType.LAZY)
    @Lob
    @Column(name = "feature", nullable = false)
    private String feature;

    @Basic(fetch = FetchType.LAZY)
    @Lob
    @Column(name = "face_image", nullable = false)
    private byte[] faceImage;

    @Basic(fetch = FetchType.LAZY)
    @Lob
    @Column(name = "scene_image", nullable = false)
    private byte[] sceneImage;

    @Column(name = "timestamp")
    private Long timestamp;

    public List<Float> getFeature() {
        return featureStringToList(this.feature);
    }

    public void setFeature(List<Float> feature) {
        this.feature = featureListToString(feature);
    }

    public void setFeature(String feature) {
        this.feature = feature;
    }

    public static String featureListToString(List<Float> feature) {
        return feature.stream().map(String::valueOf).collect(Collectors.joining(","));
    }

    public static List<Float> featureStringToList(String feature) {
        return Arrays.stream(feature.split(",")).map(Float::parseFloat).collect(Collectors.toList());
    }

}
