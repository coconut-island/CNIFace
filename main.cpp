
#include <string>
#include <grpcpp/server_builder.h>

#include "./service/DetectService.h"
#include "./service/RecognitionService.h"
#include "./service/AttributeService.h"
#include "./service/RepositoryService.h"


#define DEFAULT_MODEL_DIR_PATH "../models/relay/"
#define DEFAULT_REPO_DIR_PATH "./repo_root"
#define DEFAULT_GRPC_PORT "22506"
#define DEFAULT_CPU_DEVICES "0"

void showUsage() {
    std::cout << "Usage: cniface [m] [p]" << std::endl;
    std::cout << "    m: 模型文件文件夹路径，默认: " << DEFAULT_MODEL_DIR_PATH << std::endl;
    std::cout << "    r: 对比库文件存放文件夹路径，默认: " << DEFAULT_REPO_DIR_PATH << std::endl;
    std::cout << "    p: grpc端口号，默认: " << DEFAULT_GRPC_PORT << std::endl;
    std::cout << "    c: cpu devices，格式：0,1,2，默认: " << DEFAULT_CPU_DEVICES << std::endl;
    std::cout << "    h: help" << std::endl;
    exit(0);
}

void getCustomOpt(int argc, char *argv[], std::string &model_dir, std::string &repository_dir, std::string &port,
                  std::vector<int> &cpu_devices) {
    int opt;
    const char* opt_string = "m:r:p:c:h";
    while(-1 != (opt = getopt(argc, argv, opt_string))) {
        switch (opt) {
            case 'm':
                model_dir = optarg;
                break;
            case 'r':
                repository_dir = optarg;
                break;
            case 'p':
                port = optarg;
                break;
            case 'c':
                {
                    char *cpu_device = strtok(optarg, ",");
                    while (cpu_device != nullptr) {
                        cpu_devices.emplace_back(stoi(cpu_device));
                        cpu_device = strtok(nullptr, ",");
                    }
                }
                break;
            default:
                showUsage();
                break;
        }
    }

    std::stringstream print_cpu_device_stream;
    std::copy(cpu_devices.begin(), cpu_devices.end(), std::ostream_iterator<int>(print_cpu_device_stream, ","));
    std::string print_cpu_device = print_cpu_device_stream.str().substr(0, print_cpu_device_stream.str().length() - 1);

    LOG(INFO) << "=====CustomOpt=====";
    LOG(INFO) << "Model Dir   = " << model_dir;
    LOG(INFO) << "Repo Dir    = " << repository_dir;
    LOG(INFO) << "Port        = " << port;
    LOG(INFO) << "CPU DEVICES = " << print_cpu_device;
    LOG(INFO) << "===================";
}


int main(int argc, char *argv[]) {

    std::string model_dir = DEFAULT_MODEL_DIR_PATH;
    std::string repo_root_dir = DEFAULT_REPO_DIR_PATH;
    std::string port = DEFAULT_GRPC_PORT;
    std::vector<int> cpu_devices = {0};

    getCustomOpt(argc, argv, model_dir, repo_root_dir, port, cpu_devices);

    LOG(INFO) << "Starting All Servers!";

    std::string server_address("0.0.0.0:" + port);
    DetectService detectService(model_dir, cpu_devices);
    RecognitionService recognitionService(model_dir, cpu_devices);
    AttributeService attributeService(model_dir, cpu_devices);
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