package cn.abellee.cniface.platform.domain.entity;

import org.springframework.jdbc.core.RowMapper;
import org.springframework.lang.NonNull;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * @author abel
 * @date 2022/10/7 11:00 PM
 */
public class FaceEntityRowMapper implements RowMapper<FaceEntity> {

    public boolean isExistColumn(ResultSet rs, String columnName) {
        try {
            if (rs.findColumn(columnName) > 0 ) {
                return true;
            }
        } catch (SQLException e) {
            return false;
        }

        return false;
    }

    @Override
    public FaceEntity mapRow(@NonNull ResultSet rs, int rowNum) throws SQLException {
        FaceEntity faceEntity = new FaceEntity();
        if (isExistColumn(rs, "id")) {
            faceEntity.setId((long)rs.getInt("id"));
        }
        if (isExistColumn(rs, "name")) {
            faceEntity.setName(rs.getString("name"));
        }
        if (isExistColumn(rs, "person_id")) {
            faceEntity.setPersonId(rs.getString("person_id"));
        }
        if (isExistColumn(rs, "feature")) {
            faceEntity.setFeature(rs.getString("feature"));
        }
        if (isExistColumn(rs, "timestamp")) {
            faceEntity.setTimestamp(rs.getLong("timestamp"));
        }
        if (isExistColumn(rs, "face_image")) {
            faceEntity.setFaceImage(rs.getBytes("face_image"));
        }
        if (isExistColumn(rs, "scene_image")) {
            faceEntity.setSceneImage(rs.getBytes("scene_image"));
        }

        return faceEntity;
    }
}
