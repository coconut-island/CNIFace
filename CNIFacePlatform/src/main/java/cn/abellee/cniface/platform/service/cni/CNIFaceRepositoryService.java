package cn.abellee.cniface.platform.service.cni;

import cn.abellee.cniface.grpc.*;
import net.devh.boot.grpc.client.inject.GrpcClient;
import org.springframework.stereotype.Service;

/**
 * @author abel
 * @date 2022/10/3 10:42 PM
 */
@Service
public class CNIFaceRepositoryService {

    @GrpcClient("cniface")
    private RepositoryServiceGrpc.RepositoryServiceBlockingStub repositoryServiceBlockingStub;


    public CreateRepositoryResponse createRepository(CreateRepositoryRequest createRepositoryRequest) {
        return repositoryServiceBlockingStub.createRepository(createRepositoryRequest);
    }

    public DeleteRepositoryResponse deleteRepository(DeleteRepositoryRequest deleteRepositoryRequest) {
        return repositoryServiceBlockingStub.deleteRepository(deleteRepositoryRequest);
    }

    public ExistRepositoryResponse existRepository(ExistRepositoryRequest existRepositoryRequest) {
        return repositoryServiceBlockingStub.existRepository(existRepositoryRequest);
    }

    public ListRepositoryResponse listRepository() {
        return repositoryServiceBlockingStub.listRepository(com.google.protobuf.Empty.newBuilder().build());
    }

    public AddOrUpdateItemResponse addOrUpdateItem(AddOrUpdateItemRequest addOrUpdateItemRequest) {
        return repositoryServiceBlockingStub.addOrUpdateItem(addOrUpdateItemRequest);
    }

    public DeleteItemResponse deleteItem(DeleteItemRequest deleteItemRequest) {
        return repositoryServiceBlockingStub.deleteItem(deleteItemRequest);
    }

    public SearchResponse search(SearchRequest searchRequest) {
        return repositoryServiceBlockingStub.search(searchRequest);
    }
}
