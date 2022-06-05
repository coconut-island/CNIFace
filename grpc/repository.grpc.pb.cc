// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: repository.proto

#include "repository.pb.h"
#include "repository.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace cniface {

static const char* RepositoryService_method_names[] = {
  "/cniface.RepositoryService/listRepo",
  "/cniface.RepositoryService/addRepo",
  "/cniface.RepositoryService/deleteRepo",
  "/cniface.RepositoryService/search",
  "/cniface.RepositoryService/addFeature",
  "/cniface.RepositoryService/deleteFeature",
};

std::unique_ptr< RepositoryService::Stub> RepositoryService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RepositoryService::Stub> stub(new RepositoryService::Stub(channel, options));
  return stub;
}

RepositoryService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_listRepo_(RepositoryService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_addRepo_(RepositoryService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_deleteRepo_(RepositoryService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_search_(RepositoryService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_addFeature_(RepositoryService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_deleteFeature_(RepositoryService_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RepositoryService::Stub::listRepo(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::cniface::ListRepositoryResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::google::protobuf::Empty, ::cniface::ListRepositoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_listRepo_, context, request, response);
}

void RepositoryService::Stub::async::listRepo(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::cniface::ListRepositoryResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::google::protobuf::Empty, ::cniface::ListRepositoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_listRepo_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::listRepo(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::cniface::ListRepositoryResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_listRepo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::ListRepositoryResponse>* RepositoryService::Stub::PrepareAsynclistRepoRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::ListRepositoryResponse, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_listRepo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::ListRepositoryResponse>* RepositoryService::Stub::AsynclistRepoRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsynclistRepoRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RepositoryService::Stub::addRepo(::grpc::ClientContext* context, const ::cniface::AddRepoRequest& request, ::cniface::AddRepoResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::AddRepoRequest, ::cniface::AddRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_addRepo_, context, request, response);
}

void RepositoryService::Stub::async::addRepo(::grpc::ClientContext* context, const ::cniface::AddRepoRequest* request, ::cniface::AddRepoResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::AddRepoRequest, ::cniface::AddRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_addRepo_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::addRepo(::grpc::ClientContext* context, const ::cniface::AddRepoRequest* request, ::cniface::AddRepoResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_addRepo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::AddRepoResponse>* RepositoryService::Stub::PrepareAsyncaddRepoRaw(::grpc::ClientContext* context, const ::cniface::AddRepoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::AddRepoResponse, ::cniface::AddRepoRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_addRepo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::AddRepoResponse>* RepositoryService::Stub::AsyncaddRepoRaw(::grpc::ClientContext* context, const ::cniface::AddRepoRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncaddRepoRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RepositoryService::Stub::deleteRepo(::grpc::ClientContext* context, const ::cniface::DeleteRepoRequest& request, ::cniface::DeleteRepoResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::DeleteRepoRequest, ::cniface::DeleteRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_deleteRepo_, context, request, response);
}

void RepositoryService::Stub::async::deleteRepo(::grpc::ClientContext* context, const ::cniface::DeleteRepoRequest* request, ::cniface::DeleteRepoResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::DeleteRepoRequest, ::cniface::DeleteRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_deleteRepo_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::deleteRepo(::grpc::ClientContext* context, const ::cniface::DeleteRepoRequest* request, ::cniface::DeleteRepoResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_deleteRepo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::DeleteRepoResponse>* RepositoryService::Stub::PrepareAsyncdeleteRepoRaw(::grpc::ClientContext* context, const ::cniface::DeleteRepoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::DeleteRepoResponse, ::cniface::DeleteRepoRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_deleteRepo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::DeleteRepoResponse>* RepositoryService::Stub::AsyncdeleteRepoRaw(::grpc::ClientContext* context, const ::cniface::DeleteRepoRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncdeleteRepoRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RepositoryService::Stub::search(::grpc::ClientContext* context, const ::cniface::SearchRequest& request, ::cniface::SearchResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::SearchRequest, ::cniface::SearchResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_search_, context, request, response);
}

void RepositoryService::Stub::async::search(::grpc::ClientContext* context, const ::cniface::SearchRequest* request, ::cniface::SearchResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::SearchRequest, ::cniface::SearchResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::search(::grpc::ClientContext* context, const ::cniface::SearchRequest* request, ::cniface::SearchResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::SearchResponse>* RepositoryService::Stub::PrepareAsyncsearchRaw(::grpc::ClientContext* context, const ::cniface::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::SearchResponse, ::cniface::SearchRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_search_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::SearchResponse>* RepositoryService::Stub::AsyncsearchRaw(::grpc::ClientContext* context, const ::cniface::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncsearchRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RepositoryService::Stub::addFeature(::grpc::ClientContext* context, const ::cniface::AddFeatureRequest& request, ::cniface::AddFeatureResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::AddFeatureRequest, ::cniface::AddFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_addFeature_, context, request, response);
}

void RepositoryService::Stub::async::addFeature(::grpc::ClientContext* context, const ::cniface::AddFeatureRequest* request, ::cniface::AddFeatureResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::AddFeatureRequest, ::cniface::AddFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_addFeature_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::addFeature(::grpc::ClientContext* context, const ::cniface::AddFeatureRequest* request, ::cniface::AddFeatureResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_addFeature_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::AddFeatureResponse>* RepositoryService::Stub::PrepareAsyncaddFeatureRaw(::grpc::ClientContext* context, const ::cniface::AddFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::AddFeatureResponse, ::cniface::AddFeatureRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_addFeature_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::AddFeatureResponse>* RepositoryService::Stub::AsyncaddFeatureRaw(::grpc::ClientContext* context, const ::cniface::AddFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncaddFeatureRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RepositoryService::Stub::deleteFeature(::grpc::ClientContext* context, const ::cniface::deleteFeatureRequest& request, ::cniface::deleteFeatureResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::deleteFeatureRequest, ::cniface::deleteFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_deleteFeature_, context, request, response);
}

void RepositoryService::Stub::async::deleteFeature(::grpc::ClientContext* context, const ::cniface::deleteFeatureRequest* request, ::cniface::deleteFeatureResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::deleteFeatureRequest, ::cniface::deleteFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_deleteFeature_, context, request, response, std::move(f));
}

void RepositoryService::Stub::async::deleteFeature(::grpc::ClientContext* context, const ::cniface::deleteFeatureRequest* request, ::cniface::deleteFeatureResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_deleteFeature_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::deleteFeatureResponse>* RepositoryService::Stub::PrepareAsyncdeleteFeatureRaw(::grpc::ClientContext* context, const ::cniface::deleteFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::deleteFeatureResponse, ::cniface::deleteFeatureRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_deleteFeature_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::deleteFeatureResponse>* RepositoryService::Stub::AsyncdeleteFeatureRaw(::grpc::ClientContext* context, const ::cniface::deleteFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncdeleteFeatureRaw(context, request, cq);
  result->StartCall();
  return result;
}

RepositoryService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::google::protobuf::Empty, ::cniface::ListRepositoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Empty* req,
             ::cniface::ListRepositoryResponse* resp) {
               return service->listRepo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::cniface::AddRepoRequest, ::cniface::AddRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::AddRepoRequest* req,
             ::cniface::AddRepoResponse* resp) {
               return service->addRepo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::cniface::DeleteRepoRequest, ::cniface::DeleteRepoResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::DeleteRepoRequest* req,
             ::cniface::DeleteRepoResponse* resp) {
               return service->deleteRepo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::cniface::SearchRequest, ::cniface::SearchResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::SearchRequest* req,
             ::cniface::SearchResponse* resp) {
               return service->search(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::cniface::AddFeatureRequest, ::cniface::AddFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::AddFeatureRequest* req,
             ::cniface::AddFeatureResponse* resp) {
               return service->addFeature(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RepositoryService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RepositoryService::Service, ::cniface::deleteFeatureRequest, ::cniface::deleteFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RepositoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::deleteFeatureRequest* req,
             ::cniface::deleteFeatureResponse* resp) {
               return service->deleteFeature(ctx, req, resp);
             }, this)));
}

RepositoryService::Service::~Service() {
}

::grpc::Status RepositoryService::Service::listRepo(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::cniface::ListRepositoryResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RepositoryService::Service::addRepo(::grpc::ServerContext* context, const ::cniface::AddRepoRequest* request, ::cniface::AddRepoResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RepositoryService::Service::deleteRepo(::grpc::ServerContext* context, const ::cniface::DeleteRepoRequest* request, ::cniface::DeleteRepoResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RepositoryService::Service::search(::grpc::ServerContext* context, const ::cniface::SearchRequest* request, ::cniface::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RepositoryService::Service::addFeature(::grpc::ServerContext* context, const ::cniface::AddFeatureRequest* request, ::cniface::AddFeatureResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RepositoryService::Service::deleteFeature(::grpc::ServerContext* context, const ::cniface::deleteFeatureRequest* request, ::cniface::deleteFeatureResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace cniface

