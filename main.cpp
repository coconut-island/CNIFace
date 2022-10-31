
#include <string>
#include <grpcpp/server_builder.h>

#include "./service/DetectService.h"
#include "./service/RecognitionService.h"
#include "./service/AttributeService.h"
#include "./service/RepositoryService.h"

#include "./utils/Log.h"


#define DEFAULT_MODEL_DIR_PATH "../models/relay/"
#define DEFAULT_REPO_DIR_PATH "./repo_root"
#define DEFAULT_GRPC_PORT "22506"

void showUsage() {
    std::cout << "Usage: cniface [m] [p]" << std::endl;
    std::cout << "    m: 模型文件文件夹路径，默认: " << DEFAULT_MODEL_DIR_PATH << std::endl;
    std::cout << "    r: 对比库文件存放文件夹路径，默认: " << DEFAULT_REPO_DIR_PATH << std::endl;
    std::cout << "    p: grpc端口号，默认: " << DEFAULT_GRPC_PORT << std::endl;
    std::cout << "    h: help" << std::endl;
    exit(0);
}

void getCustomOpt(int argc, char *argv[], std::string &model_dir, std::string &repository_dir, std::string &port) {
    int opt;
    const char* opt_string = "m:p:h";
    while(-1 != (opt = getopt(argc, argv, opt_string))) {
        switch (opt) {
            case 'm':
                model_dir = optarg;
                break;
            case 'r':
                repository_dir = optarg;
                break;
            case 'p':
                port = port;
                break;
            default:
                showUsage();
                break;
        }
    }
}


int main(int argc, char *argv[]) {
    LOG(INFO) << "Starting All Servers!";

    std::string model_dir = DEFAULT_MODEL_DIR_PATH;
    std::string repo_root_dir = DEFAULT_REPO_DIR_PATH;
    std::string port = DEFAULT_GRPC_PORT;

    getCustomOpt(argc, argv, model_dir, repo_root_dir, port);

    std::string server_address("0.0.0.0:" + port);
    DetectService detectService(model_dir);
    RecognitionService recognitionService(model_dir);
    AttributeService attributeService(model_dir);
    RepositoryService repositoryService(repo_root_dir);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&detectService);
    builder.RegisterService(&recognitionService);
    builder.RegisterService(&attributeService);
    builder.RegisterService(&repositoryService);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server listening on " << server_address;
    server->Wait();
    return 0;
}