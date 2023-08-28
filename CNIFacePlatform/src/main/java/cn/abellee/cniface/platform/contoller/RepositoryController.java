package cn.abellee.cniface.platform.contoller;

import cn.abellee.cniface.platform.domain.common.CNIFacePagedResponse;
import cn.abellee.cniface.platform.domain.common.CNIFaceResponse;
import cn.abellee.cniface.platform.domain.dto.*;
import cn.abellee.cniface.platform.security.annotaion.AnonymousAccess;
import cn.abellee.cniface.platform.service.IRepositoryService;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * @author abel
 * @date 2022/8/20 9:59 PM
 */
@RestController
@RequestMapping("/repository")
public class RepositoryController {

    private final IRepositoryService repositoryService;

    public RepositoryController(IRepositoryService repositoryService) {
        this.repositoryService = repositoryService;
    }

    @PostMapping("/create")
    public CNIFaceResponse<?> create(@RequestBody CreateRepositoryDTO createRepositoryDTO){
        repositoryService.createRepository(createRepositoryDTO.getName());
        return CNIFaceResponse.ok();
    }

    @PostMapping("/delete")
    public CNIFaceResponse<?> delete(@RequestBody DeleteRepositoryRequestDTO deleteRepositoryRequestDTO){
        repositoryService.deleteRepository(deleteRepositoryRequestDTO.getRepoId());
        return CNIFaceResponse.ok();
    }

    @GetMapping("/list")
    public CNIFaceResponse<List<RepositoryResultDTO>> list(){
        List<RepositoryResultDTO> repositoryResultDTOS = repositoryService.listRepositories();
        return CNIFaceResponse.ok(repositoryResultDTOS);
    }

    @PostMapping("/item/insert")
    public CNIFaceResponse<?> insertItem(@RequestBody RepositoryAddOrUpdateItemRequestDTO repositoryAddOrUpdateItemRequestDTO){
        repositoryService.addOrUpdateItem(repositoryAddOrUpdateItemRequestDTO);
        return CNIFaceResponse.ok();
    }

    @PostMapping("/item/delete")
    public CNIFaceResponse<?> deleteItem(@RequestBody DeleteRepositoryItemRequestDTO deleteRepositoryItemRequestDTO){
        repositoryService.deleteItem(deleteRepositoryItemRequestDTO.getRepoId(), deleteRepositoryItemRequestDTO.getItemId());
        return CNIFaceResponse.ok();
    }

    @PostMapping("/item/retrieval")
    public CNIFacePagedResponse<RepositorySearchResultDTO> search(@RequestBody RepositorySearchRequestDTO repositorySearchRequestDTO){
        RepositorySearchResponseDTO repositorySearchResponseDTO = repositoryService.search(repositorySearchRequestDTO);
        return CNIFacePagedResponse.ok(repositorySearchResponseDTO.getResults().size(), repositorySearchResponseDTO.getResults());
    }

    @PostMapping("/item/query")
    public CNIFacePagedResponse<RepositoryQueryResultDTO> listItem(@RequestBody RepositoryQueryRequestDTO repositoryQueryRequestDTO){
        RepositoryQueryResponseDTO repositoryQueryResponseDTO = repositoryService.query(repositoryQueryRequestDTO);
        return CNIFacePagedResponse.ok(repositoryQueryResponseDTO.getTotal(), repositoryQueryResponseDTO.getResults());
    }

    @AnonymousAccess
    @GetMapping("/item/face_image/{repoId}/{id}")
    public ResponseEntity<byte[]> faceImage(@PathVariable Long repoId, @PathVariable Long id){
        byte[] faceImage = repositoryService.getFaceImage(repoId, id);
        return ResponseEntity.ok().contentType(MediaType.IMAGE_PNG).body(faceImage);
    }

    @AnonymousAccess
    @GetMapping("/item/scene_image/{repoId}/{id}")
    public ResponseEntity<byte[]> sceneImage(@PathVariable Long repoId, @PathVariable Long id){
        byte[] sceneImage = repositoryService.getSceneImage(repoId, id);
        return ResponseEntity.ok().contentType(MediaType.IMAGE_PNG).body(sceneImage);
    }

}
