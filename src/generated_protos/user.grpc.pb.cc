// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: user.proto

#include "user.pb.h"
#include "user.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace user {

static const char* UserService_method_names[] = {
  "/user.UserService/CreateUser",
  "/user.UserService/ReadUser",
  "/user.UserService/UpdateUser",
  "/user.UserService/DeleteUser",
  "/user.UserService/ListUsers",
};

std::unique_ptr< UserService::Stub> UserService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< UserService::Stub> stub(new UserService::Stub(channel, options));
  return stub;
}

UserService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_CreateUser_(UserService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ReadUser_(UserService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UpdateUser_(UserService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteUser_(UserService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ListUsers_(UserService_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::Status UserService::Stub::CreateUser(::grpc::ClientContext* context, const ::user::User& request, ::user::User* response) {
  return ::grpc::internal::BlockingUnaryCall< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_CreateUser_, context, request, response);
}

void UserService::Stub::async::CreateUser(::grpc::ClientContext* context, const ::user::User* request, ::user::User* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CreateUser_, context, request, response, std::move(f));
}

void UserService::Stub::async::CreateUser(::grpc::ClientContext* context, const ::user::User* request, ::user::User* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_CreateUser_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::PrepareAsyncCreateUserRaw(::grpc::ClientContext* context, const ::user::User& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_CreateUser_, context, request);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::AsyncCreateUserRaw(::grpc::ClientContext* context, const ::user::User& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncCreateUserRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status UserService::Stub::ReadUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::user::User* response) {
  return ::grpc::internal::BlockingUnaryCall< ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ReadUser_, context, request, response);
}

void UserService::Stub::async::ReadUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReadUser_, context, request, response, std::move(f));
}

void UserService::Stub::async::ReadUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReadUser_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::PrepareAsyncReadUserRaw(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::user::User, ::google::protobuf::Int64Value, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ReadUser_, context, request);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::AsyncReadUserRaw(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReadUserRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status UserService::Stub::UpdateUser(::grpc::ClientContext* context, const ::user::User& request, ::user::User* response) {
  return ::grpc::internal::BlockingUnaryCall< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UpdateUser_, context, request, response);
}

void UserService::Stub::async::UpdateUser(::grpc::ClientContext* context, const ::user::User* request, ::user::User* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUser_, context, request, response, std::move(f));
}

void UserService::Stub::async::UpdateUser(::grpc::ClientContext* context, const ::user::User* request, ::user::User* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUser_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::PrepareAsyncUpdateUserRaw(::grpc::ClientContext* context, const ::user::User& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UpdateUser_, context, request);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::AsyncUpdateUserRaw(::grpc::ClientContext* context, const ::user::User& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUpdateUserRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status UserService::Stub::DeleteUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::user::User* response) {
  return ::grpc::internal::BlockingUnaryCall< ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_DeleteUser_, context, request, response);
}

void UserService::Stub::async::DeleteUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeleteUser_, context, request, response, std::move(f));
}

void UserService::Stub::async::DeleteUser(::grpc::ClientContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_DeleteUser_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::PrepareAsyncDeleteUserRaw(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::user::User, ::google::protobuf::Int64Value, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DeleteUser_, context, request);
}

::grpc::ClientAsyncResponseReader< ::user::User>* UserService::Stub::AsyncDeleteUserRaw(::grpc::ClientContext* context, const ::google::protobuf::Int64Value& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncDeleteUserRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReader< ::user::User>* UserService::Stub::ListUsersRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request) {
  return ::grpc::internal::ClientReaderFactory< ::user::User>::Create(channel_.get(), rpcmethod_ListUsers_, context, request);
}

void UserService::Stub::async::ListUsers(::grpc::ClientContext* context, const ::google::protobuf::Empty* request, ::grpc::ClientReadReactor< ::user::User>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::user::User>::Create(stub_->channel_.get(), stub_->rpcmethod_ListUsers_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::user::User>* UserService::Stub::AsyncListUsersRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::user::User>::Create(channel_.get(), cq, rpcmethod_ListUsers_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::user::User>* UserService::Stub::PrepareAsyncListUsersRaw(::grpc::ClientContext* context, const ::google::protobuf::Empty& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::user::User>::Create(channel_.get(), cq, rpcmethod_ListUsers_, context, request, false, nullptr);
}

UserService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      UserService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< UserService::Service, ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](UserService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::user::User* req,
             ::user::User* resp) {
               return service->CreateUser(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      UserService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< UserService::Service, ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](UserService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Int64Value* req,
             ::user::User* resp) {
               return service->ReadUser(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      UserService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< UserService::Service, ::user::User, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](UserService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::user::User* req,
             ::user::User* resp) {
               return service->UpdateUser(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      UserService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< UserService::Service, ::google::protobuf::Int64Value, ::user::User, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](UserService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Int64Value* req,
             ::user::User* resp) {
               return service->DeleteUser(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      UserService_method_names[4],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< UserService::Service, ::google::protobuf::Empty, ::user::User>(
          [](UserService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::google::protobuf::Empty* req,
             ::grpc::ServerWriter<::user::User>* writer) {
               return service->ListUsers(ctx, req, writer);
             }, this)));
}

UserService::Service::~Service() {
}

::grpc::Status UserService::Service::CreateUser(::grpc::ServerContext* context, const ::user::User* request, ::user::User* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UserService::Service::ReadUser(::grpc::ServerContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UserService::Service::UpdateUser(::grpc::ServerContext* context, const ::user::User* request, ::user::User* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UserService::Service::DeleteUser(::grpc::ServerContext* context, const ::google::protobuf::Int64Value* request, ::user::User* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status UserService::Service::ListUsers(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::grpc::ServerWriter< ::user::User>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace user
