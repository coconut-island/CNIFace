// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: recognition.proto
#ifndef GRPC_recognition_2eproto__INCLUDED
#define GRPC_recognition_2eproto__INCLUDED

#include "recognition.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace cniface {

class RecognitionService final {
 public:
  static constexpr char const* service_full_name() {
    return "cniface.RecognitionService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::cniface::ExtractFeatureResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>> AsyncextractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>>(AsyncextractFeatureRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>> PrepareAsyncextractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>>(PrepareAsyncextractFeatureRaw(context, request, cq));
    }
    virtual ::grpc::Status similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::cniface::SimilarityeResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>> Asyncsimilarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>>(AsyncsimilarityRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>> PrepareAsyncsimilarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>>(PrepareAsyncsimilarityRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>* AsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::cniface::ExtractFeatureResponse>* PrepareAsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>* AsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::cniface::SimilarityeResponse>* PrepareAsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::cniface::ExtractFeatureResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>> AsyncextractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>>(AsyncextractFeatureRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>> PrepareAsyncextractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>>(PrepareAsyncextractFeatureRaw(context, request, cq));
    }
    ::grpc::Status similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::cniface::SimilarityeResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>> Asyncsimilarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>>(AsyncsimilarityRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>> PrepareAsyncsimilarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>>(PrepareAsyncsimilarityRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, std::function<void(::grpc::Status)>) override;
      void extractFeature(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response, std::function<void(::grpc::Status)>) override;
      void similarity(::grpc::ClientContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>* AsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::cniface::ExtractFeatureResponse>* PrepareAsyncextractFeatureRaw(::grpc::ClientContext* context, const ::cniface::ExtractFeatureRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>* AsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::cniface::SimilarityeResponse>* PrepareAsyncsimilarityRaw(::grpc::ClientContext* context, const ::cniface::SimilarityRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_extractFeature_;
    const ::grpc::internal::RpcMethod rpcmethod_similarity_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status extractFeature(::grpc::ServerContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response);
    virtual ::grpc::Status similarity(::grpc::ServerContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_extractFeature() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestextractFeature(::grpc::ServerContext* context, ::cniface::ExtractFeatureRequest* request, ::grpc::ServerAsyncResponseWriter< ::cniface::ExtractFeatureResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_similarity() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestsimilarity(::grpc::ServerContext* context, ::cniface::SimilarityRequest* request, ::grpc::ServerAsyncResponseWriter< ::cniface::SimilarityeResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_extractFeature<WithAsyncMethod_similarity<Service > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_extractFeature() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::cniface::ExtractFeatureRequest* request, ::cniface::ExtractFeatureResponse* response) { return this->extractFeature(context, request, response); }));}
    void SetMessageAllocatorFor_extractFeature(
        ::grpc::MessageAllocator< ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* extractFeature(
      ::grpc::CallbackServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_similarity() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::cniface::SimilarityRequest, ::cniface::SimilarityeResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::cniface::SimilarityRequest* request, ::cniface::SimilarityeResponse* response) { return this->similarity(context, request, response); }));}
    void SetMessageAllocatorFor_similarity(
        ::grpc::MessageAllocator< ::cniface::SimilarityRequest, ::cniface::SimilarityeResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::cniface::SimilarityRequest, ::cniface::SimilarityeResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* similarity(
      ::grpc::CallbackServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_extractFeature<WithCallbackMethod_similarity<Service > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_extractFeature() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_similarity() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_extractFeature() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestextractFeature(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_similarity() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestsimilarity(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_extractFeature() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->extractFeature(context, request, response); }));
    }
    ~WithRawCallbackMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* extractFeature(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_similarity() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->similarity(context, request, response); }));
    }
    ~WithRawCallbackMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* similarity(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_extractFeature : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_extractFeature() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::cniface::ExtractFeatureRequest, ::cniface::ExtractFeatureResponse>* streamer) {
                       return this->StreamedextractFeature(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_extractFeature() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status extractFeature(::grpc::ServerContext* /*context*/, const ::cniface::ExtractFeatureRequest* /*request*/, ::cniface::ExtractFeatureResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedextractFeature(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::cniface::ExtractFeatureRequest,::cniface::ExtractFeatureResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_similarity : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_similarity() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::cniface::SimilarityRequest, ::cniface::SimilarityeResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::cniface::SimilarityRequest, ::cniface::SimilarityeResponse>* streamer) {
                       return this->Streamedsimilarity(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_similarity() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status similarity(::grpc::ServerContext* /*context*/, const ::cniface::SimilarityRequest* /*request*/, ::cniface::SimilarityeResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status Streamedsimilarity(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::cniface::SimilarityRequest,::cniface::SimilarityeResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_extractFeature<WithStreamedUnaryMethod_similarity<Service > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_extractFeature<WithStreamedUnaryMethod_similarity<Service > > StreamedService;
};

}  // namespace cniface


#endif  // GRPC_recognition_2eproto__INCLUDED