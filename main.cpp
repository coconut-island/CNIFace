
#include <string>
#include <grpcpp/server_builder.h>

#include "./service/RepositoryService.h"
#include "./service/DetectService.h"
#include "./service/RecognitionService.h"


int main(int argc, char *argv[]) {
    std::cout << "Starting All Servers!" << std::endl;

    std::string server_address("0.0.0.0:50051");
    RepositoryService repositoryService;
    DetectService detectService;
    RecognitionService recognitionService;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&repositoryService);
    builder.RegisterService(&detectService);
    builder.RegisterService(&recognitionService);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}