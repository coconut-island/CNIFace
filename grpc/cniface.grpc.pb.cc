// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: cniface.proto

#include "cniface.pb.h"
#include "cniface.grpc.pb.h"

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

static const char* DetectService_method_names[] = {
  "/cniface.DetectService/detect",
};

std::unique_ptr< DetectService::Stub> DetectService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< DetectService::Stub> stub(new DetectService::Stub(channel, options));
  return stub;
}

DetectService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_detect_(DetectService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status DetectService::Stub::detect(::grpc::ClientContext* context, const ::cniface::DetectRequest& request, ::cniface::DetectResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::DetectRequest, ::cniface::DetectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_detect_, context, request, response);
}

void DetectService::Stub::async::detect(::grpc::ClientContext* context, const ::cniface::DetectRequest* request, ::cniface::DetectResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::DetectRequest, ::cniface::DetectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_detect_, context, request, response, std::move(f));
}

void DetectService::Stub::async::detect(::grpc::ClientContext* context, const ::cniface::DetectRequest* request, ::cniface::DetectResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_detect_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::DetectResponse>* DetectService::Stub::PrepareAsyncdetectRaw(::grpc::ClientContext* context, const ::cniface::DetectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::DetectResponse, ::cniface::DetectRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_detect_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::DetectResponse>* DetectService::Stub::AsyncdetectRaw(::grpc::ClientContext* context, const ::cniface::DetectRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncdetectRaw(context, request, cq);
  result->StartCall();
  return result;
}

DetectService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      DetectService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< DetectService::Service, ::cniface::DetectRequest, ::cniface::DetectResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](DetectService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::DetectRequest* req,
             ::cniface::DetectResponse* resp) {
               return service->detect(ctx, req, resp);
             }, this)));
}

DetectService::Service::~Service() {
}

::grpc::Status DetectService::Service::detect(::grpc::ServerContext* context, const ::cniface::DetectRequest* request, ::cniface::DetectResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* RecognitionService_method_names[] = {
  "/cniface.RecognitionService/extractFeature",
  "/cniface.RecognitionService/similarity",
};

std::unique_ptr< RecognitionService::Stub> RecognitionService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RecognitionService::Stub> stub(new RecognitionService::Stub(channel, options));
  return stub;
}

RecognitionService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_extractFeature_(RecognitionService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_similarity_(RecognitionService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RecognitionService::Stub::extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::cniface::ExtractFeatureResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_extractFeature_, context, request, response);
}

void RecognitionService::Stub::async::extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_extractFeature_, context, request, response, std::move(f));
}

void RecognitionService::Stub::async::extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_extractFeature_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>* RecognitionService::Stub::PrepareAsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::ExtractFeatureResponse, ::cniface::ExtractFeatureRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_extractFeature_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>* RecognitionService::Stub::AsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncextractFeatureRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status RecognitionService::Stub::similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::cniface::SimilarityResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::cniface::SimilarityRequest, ::cniface::SimilarityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_similarity_, context, request, response);
}

void RecognitionService::Stub::async::similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::cniface::SimilarityRequest, ::cniface::SimilarityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_similarity_, context, request, response, std::move(f));
}

void RecognitionService::Stub::async::similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_similarity_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::cniface::SimilarityResponse>* RecognitionService::Stub::PrepareAsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::cniface::SimilarityResponse, ::cniface::SimilarityRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_similarity_, context, request);
}

::grpc::ClientAsyncResponseReader< ::cniface::SimilarityResponse>* RecognitionService::Stub::AsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncsimilarityRaw(context, request, cq);
  result->StartCall();
  return result;
}

RecognitionService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RecognitionService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RecognitionService::Service, ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RecognitionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::ExtractFeatureRequest* req,
             ::cniface::ExtractFeatureResponse* resp) {
               return service->extractFeature(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RecognitionService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RecognitionService::Service, ::cniface::SimilarityRequest, ::cniface::SimilarityResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](RecognitionService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::cniface::SimilarityRequest* req,
             ::cniface::SimilarityResponse* resp) {
               return service->similarity(ctx, req, resp);
             }, this)));
}

RecognitionService::Service::~Service() {
}

::grpc::Status RecognitionService::Service::extractFeature(::grpc::ServerContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RecognitionService::Service::similarity(::grpc::ServerContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace cniface

