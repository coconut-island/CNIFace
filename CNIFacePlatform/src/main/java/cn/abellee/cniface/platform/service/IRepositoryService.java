package cn.abellee.cniface.platform.service;

import cn.abellee.cniface.platform.domain.dto.*;

import java.util.List;

/**
 * @author abel
 * @date 2022/8/20 9:17 PM
 */
public interface IRepositoryService {

    void createRepository(String repoName);

    List<RepositoryResultDTO> listRepositories();

    boolean existRepository(Long repoId);

    void deleteRepository(Long repoId);

    void addOrUpdateItem(RepositoryAddOrUpdateItemRequestDTO repositoryAddOrUpdateItemRequestDTO);

    void deleteItem(Long repoId, Long itemId);

    RepositorySearchResponseDTO search(RepositorySearchRequestDTO repositorySearchRequestDTO);

    RepositoryQueryResponseDTO query(RepositoryQueryRequestDTO repositoryQueryRequestDTO);

    byte[] getFaceImage(Long repoId, Long id);

    byte[] getSceneImage(Long repoId, Long id);
}
