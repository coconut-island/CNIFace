
#include <string>
#include <grpcpp/server_builder.h>

#include "./service/DetectService.h"
#include "./service/DetectWithMaskService.h"
#include "./service/RecognitionService.h"


#define DEFAULT_MODEL_DIR_PATH "../models/relay/"
#define DEFAULT_GRPC_PORT "22506"

void showUsage() {
    std::cout << "Usage: cniface [m] [p]" << std::endl;
    std::cout << "    m: 模型文件文件夹路径，默认: " << DEFAULT_MODEL_DIR_PATH << std::endl;
    std::cout << "    b: grpc端口号，默认: " << DEFAULT_GRPC_PORT << std::endl;
    std::cout << "    h: help" << std::endl;
    exit(0);
}

void getCustomOpt(int argc, char *argv[], std::string &model_dir, std::string &port) {
    int opt;
    const char* opt_string = "m:p:h";
    while(-1 != (opt = getopt(argc, argv, opt_string))) {
        switch (opt) {
            case 'm':
                model_dir = optarg;
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
    std::cout << "Starting All Servers!" << std::endl;

    std::string model_dir = DEFAULT_MODEL_DIR_PATH;
    std::string port = DEFAULT_GRPC_PORT;

    getCustomOpt(argc, argv, model_dir, port);

    std::string server_address("0.0.0.0:" + port);
    DetectService detectService(model_dir);
    DetectWithMaskService detectWithMaskService(model_dir);
    RecognitionService recognitionService(model_dir);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&detectService);
    builder.RegisterService(&detectWithMaskService);
    builder.RegisterService(&recognitionService);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}