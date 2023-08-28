package cn.abellee.cniface.platform.service.impl;

import cn.abellee.cniface.grpc.*;
import cn.abellee.cniface.platform.domain.common.CNIFaceResponseCode;
import cn.abellee.cniface.platform.domain.dto.*;
import cn.abellee.cniface.platform.domain.dto.DetectResult;
import cn.abellee.cniface.platform.domain.entity.FaceEntity;
import cn.abellee.cniface.platform.domain.entity.FaceEntityRowMapper;
import cn.abellee.cniface.platform.domain.entity.RepositoryEntity;
import cn.abellee.cniface.platform.exception.CNIFaceException;
import cn.abellee.cniface.platform.repository.RepositoryRepository;
import cn.abellee.cniface.platform.service.IDetectService;
import cn.abellee.cniface.platform.service.IRecognitionService;
import cn.abellee.cniface.platform.service.IRepositoryService;
import cn.abellee.cniface.platform.service.cni.CNIFaceRepositoryService;
import cn.abellee.cniface.platform.util.Base64Util;
import cn.abellee.cniface.platform.util.TimeUtil;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.domain.Example;
import org.springframework.jdbc.core.*;
import org.springframework.jdbc.core.namedparam.MapSqlParameterSource;
import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.jdbc.support.KeyHolder;
import org.springframework.stereotype.Service;

import javax.imageio.ImageIO;
import javax.sql.DataSource;
import javax.transaction.Transactional;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

/**
 * @author abel
 * @date 2022/8/20 9:35 PM
 */
@Service
@Slf4j
public class RepositoryServiceImpl implements IRepositoryService {

    private final CNIFaceRepositoryService cniFaceRepositoryService;

    private final IDetectService detectService;

    private final IRecognitionService recognitionService;

    private final JdbcTemplate jdbcTemplate;

    private final NamedParameterJdbcTemplate namedParameterJdbcTemplate;

    private final RepositoryRepository repositoryRepository;

    private final Map<Long, RepositoryEntity> repoId2RepositoryInDBMap;

    public RepositoryServiceImpl(CNIFaceRepositoryService cniFaceRepositoryService, IDetectService detectService,
                                 IRecognitionService recognitionService, JdbcTemplate jdbcTemplate,
                                 RepositoryRepository repositoryRepository) {
        this.cniFaceRepositoryService = cniFaceRepositoryService;
        this.detectService = detectService;
        this.recognitionService = recognitionService;
        this.jdbcTemplate = jdbcTemplate;
        this.repositoryRepository = repositoryRepository;

        assert jdbcTemplate.getDataSource() != null;
        namedParameterJdbcTemplate = new NamedParameterJdbcTemplate(jdbcTemplate.getDataSource());

        repoId2RepositoryInDBMap = new ConcurrentHashMap<>();
    }

    private RepositoryEntity getRepositoryById(Long repoId) {
        if (repoId2RepositoryInDBMap.containsKey(repoId)) {
            return repoId2RepositoryInDBMap.get(repoId);
        }

        Optional<RepositoryEntity> repositoryEntityOptional = repositoryRepository.findById(repoId);
        if (repositoryEntityOptional.isEmpty()) {
            throw new CNIFaceException(CNIFaceResponseCode.VALID_ERROR, "人像库不存在");
        }
        RepositoryEntity repositoryEntity = repositoryEntityOptional.get();
        repoId2RepositoryInDBMap.put(repoId, repositoryEntity);
        return repositoryEntity;
    }

    @Override
    @Transactional(rollbackOn = Exception.class)
    public void createRepository(String repoName) {
        RepositoryEntity repositoryEntity = new RepositoryEntity();
        repositoryEntity.setName(repoName);
        Optional<RepositoryEntity> repositoryRepositoryOne = repositoryRepository.findOne(Example.of(repositoryEntity));
        if (repositoryRepositoryOne.isPresent()) {
            repositoryEntity = repositoryRepositoryOne.get();
            ExistRepositoryRequest existRepositoryRequest = ExistRepositoryRequest.newBuilder()
                    .setRepoName(repositoryEntity.getRepoNameInDB())
                    .build();
            ExistRepositoryResponse existRepositoryResponse = cniFaceRepositoryService.existRepository(existRepositoryRequest);
            if (existRepositoryResponse.getCode() != 0) throw new CNIFaceException(existRepositoryResponse.getCode(), "cniface 人像库已存在");
            if (existRepositoryResponse.getExist()) {
                throw new CNIFaceException(CNIFaceResponseCode.VALID_ERROR, "人像库已存在");
            }
            repositoryRepository.deleteById(repositoryEntity.getId());
        }

        repositoryEntity = new RepositoryEntity();
        repositoryEntity.setName(repoName);
        repositoryEntity.setTotal(0);
        repositoryEntity = repositoryRepository.save(repositoryEntity);

        String repoNameInDB = repositoryEntity.getRepoNameInDB();

        CreateRepositoryRequest createRepositoryRequest = CreateRepositoryRequest.newBuilder()
                .setRepoName(repoNameInDB)
                .build();
        CreateRepositoryResponse createRepositoryResponse = cniFaceRepositoryService.createRepository(createRepositoryRequest);
        if (createRepositoryResponse.getCode() != 0) {
            repositoryRepository.deleteById(repositoryEntity.getId());
            throw new CNIFaceException(createRepositoryResponse.getCode(), "cniface 创建人像库失败");
        }


        DataSource dataSource = jdbcTemplate.getDataSource();
        assert dataSource != null;
        try (Connection connection = dataSource.getConnection(); Statement statement = connection.createStatement()) {
            DatabaseMetaData meta = connection.getMetaData();
            ResultSet tables = meta.getTables (null, null, repoNameInDB, null);
            if (tables.next()) {
                statement.executeUpdate("TRUNCATE " + repoNameInDB);
            }
            statement.execute("CREATE TABLE " + repoNameInDB + " LIKE repo");
        } catch (Exception e) {
            e.printStackTrace ();
        }
    }

    @Override
    @Transactional(rollbackOn = Exception.class)
    public void deleteRepository(Long repoId) {
        RepositoryEntity repositoryEntity = getRepositoryById(repoId);
        repositoryRepository.deleteById(repositoryEntity.getId());
        repoId2RepositoryInDBMap.remove(repoId);

        DeleteRepositoryRequest deleteRepositoryRequest = DeleteRepositoryRequest.newBuilder()
                .setRepoName(repositoryEntity.getRepoNameInDB())
                .build();
        DeleteRepositoryResponse deleteRepositoryResponse = cniFaceRepositoryService.deleteRepository(deleteRepositoryRequest);
        if (deleteRepositoryResponse.getCode() != 0) throw new CNIFaceException(deleteRepositoryResponse.getCode(), deleteRepositoryResponse.getMessage());

        String repoNameInDB = repositoryEntity.getRepoNameInDB();
        DataSource dataSource = jdbcTemplate.getDataSource();
        assert dataSource != null;
        try (Connection connection = dataSource.getConnection(); Statement statement = connection.createStatement()) {
            DatabaseMetaData meta = connection.getMetaData();
            ResultSet tables = meta.getTables (null, null, repoNameInDB, null);
            if (tables.next()) {
                statement.executeUpdate("TRUNCATE " + repoNameInDB);
            }
        } catch (Exception e) {
            e.printStackTrace ();
        }
    }

    @Override
    @Transactional(rollbackOn = Exception.class)
    public void addOrUpdateItem(RepositoryAddOrUpdateItemRequestDTO repositoryAddOrUpdateItemRequestDTO) {
        RepositoryEntity repositoryEntity = getRepositoryById(repositoryAddOrUpdateItemRequestDTO.getRepoId());

        DetectRequestDTO detectRequestDTO = new DetectRequestDTO();
        detectRequestDTO.setFaceImageBase64(repositoryAddOrUpdateItemRequestDTO.getImageBase64());
        detectRequestDTO.setScore(repositoryAddOrUpdateItemRequestDTO.getDetectScore());
        List<DetectResult> detectResults = detectService.detect(detectRequestDTO);
        if (detectResults.isEmpty()) {
            throw new CNIFaceException(CNIFaceResponseCode.NO_FACE_DETECTED);
        }

        if (detectResults.size() > 1) {
            throw new CNIFaceException(CNIFaceResponseCode.MULTIPLE_FACES_DETECTED);
        }

        DetectResult detectResult = detectResults.get(0);

        ExtractFeatureRequestDTO extractFeatureRequestDTO = new ExtractFeatureRequestDTO();
        extractFeatureRequestDTO.setFaceImageBase64(repositoryAddOrUpdateItemRequestDTO.getImageBase64());
        extractFeatureRequestDTO.setKps(detectResult.getKps());
        extractFeatureRequestDTO.setModel("");
        ExtractFeatureResponseDTO extractFeatureResponseDTO = recognitionService.extractFeature(extractFeatureRequestDTO);
        List<Float> feature = extractFeatureResponseDTO.getFeature();

        byte[] imageBytes = Base64Util.decodeFromString(repositoryAddOrUpdateItemRequestDTO.getImageBase64());

        byte[] subImage;
        try (ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            BufferedImage bufferedImage = ImageIO.read(new ByteArrayInputStream(imageBytes));
            BufferedImage subBufferedImage = bufferedImage.getSubimage(
                    (int)detectResult.getX(), (int)detectResult.getY(),
                    (int)detectResult.getW(), (int)detectResult.getH());
            ImageIO.write(subBufferedImage, "jpeg", out);
            subImage = out.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
            throw new CNIFaceException(CNIFaceResponseCode.READ_IMAGE_ERROR, "裁剪图片过程出错，可能图片不合法。");
        }

        String repoNameInDB = repositoryEntity.getRepoNameInDB();

        int id;
        if (repositoryAddOrUpdateItemRequestDTO.getId() != null) {
            throw new CNIFaceException(CNIFaceResponseCode.NOT_SUPPORT);
        } else {
            String sql = "INSERT INTO `" + repoNameInDB + "` (`name`, `person_id`, `feature`, `face_image`, `scene_image`, `timestamp`) VALUES (?, ?, ?, ?, ?, ?);";
            KeyHolder keyHolder = new GeneratedKeyHolder();
            jdbcTemplate.update(connection -> {
                // 指定主键
                PreparedStatement preparedStatement = connection.prepareStatement(sql, new String[]{"id"});
                preparedStatement.setString(1, repositoryAddOrUpdateItemRequestDTO.getName());
                preparedStatement.setString(2, repositoryAddOrUpdateItemRequestDTO.getPersonId());
                preparedStatement.setString(3, FaceEntity.featureListToString(feature));
                preparedStatement.setBytes(4, subImage);
                preparedStatement.setBytes(5, imageBytes);
                preparedStatement.setLong(6, TimeUtil.now());
                return preparedStatement;
            }, keyHolder);
            assert keyHolder.getKey() != null;
            id = keyHolder.getKey().intValue();
            String setTotalSql = "update repositories set total=total+1 where id='" + repositoryEntity.getId() + "';";
            jdbcTemplate.update(setTotalSql);
        }

        assert id > 0;
        AddOrUpdateItemRequest addOrUpdateItemRequest = AddOrUpdateItemRequest.newBuilder()
                .setId(id)
                .setRepoName(repoNameInDB)
                .addAllFeature(feature)
                .build();
        AddOrUpdateItemResponse addOrUpdateItemResponse = cniFaceRepositoryService.addOrUpdateItem(addOrUpdateItemRequest);
        if (addOrUpdateItemResponse.getCode() != 0) {
            throw new CNIFaceException(addOrUpdateItemResponse.getCode(), addOrUpdateItemResponse.getMessage());
        }
    }

    @Override
    @Transactional
    public void deleteItem(Long repoId, Long itemId) {
        RepositoryEntity repositoryEntity = getRepositoryById(repoId);

        jdbcTemplate.update("DELETE FROM " + repositoryEntity.getRepoNameInDB() + " WHERE id=?", itemId);

        String setTotalSql = "update repositories set total=total-1 where id='" + repositoryEntity.getId() + "';";
        jdbcTemplate.update(setTotalSql);

        DeleteItemRequest deleteItemRequest = DeleteItemRequest.newBuilder()
                .setRepoName(repositoryEntity.getRepoNameInDB())
                .setId(itemId)
                .build();
        DeleteItemResponse deleteItemResponse = cniFaceRepositoryService.deleteItem(deleteItemRequest);
        if (deleteItemResponse.getCode() != 0) throw new CNIFaceException(deleteItemResponse.getCode(), "cniface 删除元素失败");
    }

    private float similarityCal(float similarity) {
        similarity = similarity < 0 ? 0 : similarity;
        similarity = similarity > 1 ? 1 : similarity;
        similarity = similarity * 100;
        return similarity;
    }

    @Override
    public RepositorySearchResponseDTO search(RepositorySearchRequestDTO repositorySearchRequestDTO) {
        RepositoryEntity repositoryEntity = getRepositoryById(repositorySearchRequestDTO.getRepoId());

        String repoNameInDB = repositoryEntity.getRepoNameInDB();

        List<Float> feature = repositorySearchRequestDTO.getFeature();
        if (repositorySearchRequestDTO.getImageBase64() != null) {
            if (repositorySearchRequestDTO.getDetectScore() == null) {
                repositorySearchRequestDTO.setDetectScore(0.4F);
            }

            DetectRequestDTO detectRequestDTO = new DetectRequestDTO();
            detectRequestDTO.setFaceImageBase64(repositorySearchRequestDTO.getImageBase64());
            detectRequestDTO.setScore(repositorySearchRequestDTO.getDetectScore());
            detectRequestDTO.setIsReturnRecognitionSubImage(false);
            List<DetectResult> detectResults = detectService.detect(detectRequestDTO);
            if (detectResults.isEmpty()) {
                throw new CNIFaceException(CNIFaceResponseCode.NO_FACE_DETECTED);
            }

            if (detectResults.size() > 1) {
                throw new CNIFaceException(CNIFaceResponseCode.MULTIPLE_FACES_DETECTED);
            }

            DetectResult detectResult = detectResults.get(0);

            ExtractFeatureRequestDTO extractFeatureRequestDTO = new ExtractFeatureRequestDTO();
            extractFeatureRequestDTO.setFaceImageBase64(repositorySearchRequestDTO.getImageBase64());
            extractFeatureRequestDTO.setKps(detectResult.getKps());
            extractFeatureRequestDTO.setModel("w600k_mbf");
            ExtractFeatureResponseDTO extractFeatureResponseDTO = recognitionService.extractFeature(extractFeatureRequestDTO);
            feature = extractFeatureResponseDTO.getFeature();
        }

        SearchRequest searchRequest = SearchRequest.newBuilder()
                .addAllFeature(feature)
                .setRepoName(repoNameInDB)
                .setTopk(repositorySearchRequestDTO.getTopk())
                .build();
        SearchResponse searchResponse = cniFaceRepositoryService.search(searchRequest);
        if (searchResponse.getCode() != 0) throw new CNIFaceException(searchResponse.getCode(), "cniface 检索失败");

        List<Long> ids = searchResponse.getResultsList().stream().map(SearchResponseItem::getId).collect(Collectors.toList());

        RepositorySearchResponseDTO repositorySearchResponseDTO = new RepositorySearchResponseDTO();
        if (ids.isEmpty()) return repositorySearchResponseDTO;
        
        MapSqlParameterSource parameters = new MapSqlParameterSource();
        parameters.addValue("ids", ids);

        List<FaceEntity> faceEntities = namedParameterJdbcTemplate.query(
                "SELECT id,name,person_id,timestamp FROM " + repoNameInDB + " WHERE id IN (:ids)",
                parameters,
                new FaceEntityRowMapper());
        Map<Long, FaceEntity> faceEntityMap = faceEntities.stream().collect(
                Collectors.toMap(FaceEntity::getId, faceEntity -> faceEntity));

        List<RepositorySearchResultDTO> repositorySearchResultDTOS = new ArrayList<>();
        for (SearchResponseItem searchResponseItem : searchResponse.getResultsList()) {
            long id = searchResponseItem.getId();
            float score = searchResponseItem.getScore();
            FaceEntity faceEntity = faceEntityMap.get(id);

            RepositorySearchResultDTO repositorySearchResultDTO = new RepositorySearchResultDTO();
            repositorySearchResultDTO.setId(id);
            repositorySearchResultDTO.setName(faceEntity.getName());
            repositorySearchResultDTO.setPersonId(faceEntity.getPersonId());
            repositorySearchResultDTO.setScore(similarityCal(score));
            repositorySearchResultDTOS.add(repositorySearchResultDTO);
        }


        repositorySearchResponseDTO.setResults(repositorySearchResultDTOS);
        return repositorySearchResponseDTO;
    }

    @Override
    public RepositoryQueryResponseDTO query(RepositoryQueryRequestDTO repositoryQueryRequestDTO) {
        RepositoryEntity repositoryEntity = getRepositoryById(repositoryQueryRequestDTO.getRepoId());

        String repoNameInDB = repositoryEntity.getRepoNameInDB();
        String querySql = "SELECT id,name,person_id,timestamp FROM " + repoNameInDB;
        String countSql = "SELECT count(1) FROM " + repoNameInDB;

        String whereSql = "";
        if (repositoryQueryRequestDTO.getName() != null && !repositoryQueryRequestDTO.getName().isBlank()) {
            whereSql += " name='" + repositoryQueryRequestDTO.getName() + "'";
        }
        if (repositoryQueryRequestDTO.getPersonId() != null && !repositoryQueryRequestDTO.getPersonId().isBlank()) {
            whereSql += " person_id='" + repositoryQueryRequestDTO.getPersonId() + "'";
        }
        if (repositoryQueryRequestDTO.getStartTimestamp() != null && repositoryQueryRequestDTO.getStartTimestamp() > 0) {
            whereSql += " AND timestamp>=" + repositoryQueryRequestDTO.getStartTimestamp();
        }
        if (repositoryQueryRequestDTO.getEndTimestamp() != null&& repositoryQueryRequestDTO.getEndTimestamp() > 0) {
            whereSql += " AND timestamp<=" + repositoryQueryRequestDTO.getEndTimestamp();
        }
        if (!whereSql.isBlank()) {
            querySql += " WHERE" + whereSql;
            countSql += " WHERE" + whereSql;
        }

        querySql += " ORDER BY timestamp DESC";

        Integer page = repositoryQueryRequestDTO.getPage();
        Integer size = repositoryQueryRequestDTO.getSize();

        querySql += " LIMIT " + size;
        querySql += " OFFSET " + (page - 1) * size;

        List<FaceEntity> faceEntities = namedParameterJdbcTemplate.query(querySql, new FaceEntityRowMapper());

        Integer count = jdbcTemplate.queryForObject(countSql, Integer.class);

        RepositoryQueryResponseDTO repositoryQueryResponseDTO = new RepositoryQueryResponseDTO();
        List<RepositoryQueryResultDTO> repositoryQueryResultDTOS = new ArrayList<>();
        for (FaceEntity faceEntity : faceEntities) {
            RepositoryQueryResultDTO repositoryQueryResultDTO = new RepositoryQueryResultDTO();
            repositoryQueryResultDTO.setId(faceEntity.getId());
            repositoryQueryResultDTO.setName(faceEntity.getName());
            repositoryQueryResultDTO.setPersonId(faceEntity.getPersonId());
            repositoryQueryResultDTO.setTimestamp(faceEntity.getTimestamp());
            repositoryQueryResultDTOS.add(repositoryQueryResultDTO);
        }
        repositoryQueryResponseDTO.setResults(repositoryQueryResultDTOS);
        repositoryQueryResponseDTO.setTotal(count);

        return repositoryQueryResponseDTO;
    }

    @Override
    public byte[] getFaceImage(Long repoId, Long id) {
        String sql = "SELECT id,face_image FROM " + "repo_" + repoId + " WHERE id=?";
        return jdbcTemplate.queryForObject(sql, new Object[]{id}, (rs, rowNum) -> rs.getBytes("face_image"));
    }

    @Override
    public byte[] getSceneImage(Long repoId, Long id) {
        String sql = "SELECT id,scene_image FROM " + "repo_" + repoId + " WHERE id=?";
        return jdbcTemplate.queryForObject(sql, new Object[]{id}, (rs, rowNum) -> rs.getBytes("scene_image"));
    }

    @Override
    public boolean existRepository(Long repoId) {
        return repositoryRepository.existsById(repoId);
    }

    @Override
    public List<RepositoryResultDTO> listRepositories() {
        List<RepositoryEntity> repositoryRepositoryAll = repositoryRepository.findAll();
        List<RepositoryResultDTO> repositoryResultDTOS = new ArrayList<>(repositoryRepositoryAll.size());
        for (RepositoryEntity repositoryEntity : repositoryRepositoryAll) {
            RepositoryResultDTO repositoryResultDTO = new RepositoryResultDTO();
            repositoryResultDTO.setId(repositoryEntity.getId());
            repositoryResultDTO.setName(repositoryEntity.getName());
            repositoryResultDTO.setTotal(repositoryEntity.getTotal());
            repositoryResultDTO.setCreateTime(repositoryEntity.getCreateTime());
            repositoryResultDTO.setUpdateTime(repositoryEntity.getUpdateTime());
            repositoryResultDTOS.add(repositoryResultDTO);
        }
        return repositoryResultDTOS;
    }
}
