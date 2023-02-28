# Minimum CMake required
cmake_minimum_required(VERSION 3.15)

project(P2r)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED HINTS "/projects/gRPC/install/lib/cmake/protobuf")
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED HINTS "/projects/gRPC/install/lib/cmake/grpc")
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

get_filename_component(p2r_proto "protobuf/proto/P2r.proto" ABSOLUTE)
get_filename_component(p2r_proto_path "${p2r_proto}" PATH)

set(p2r_proto_srcs "../../protobuf/classes/P2r.pb.cc")
set(p2r_proto_hdrs "../../protobuf/classes/P2r.pb.h")
set(p2r_grpc_srcs "../../protobuf/classes/P2r.grpc.pb.cc")
set(p2r_grpc_hdrs "../../protobuf/classes/P2r.grpc.pb.h")
add_custom_command(
      OUTPUT "${p2r_proto_srcs}" "${p2r_proto_hdrs}" "${p2r_grpc_srcs}" "${p2r_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "../../protobuf/classes/"
        --cpp_out "../../protobuf/classes/"
        -I "${p2r_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${p2r_proto}"
      DEPENDS "${p2r_proto}")

include_directories("../../protobuf/classes/")

add_library( P2r STATIC
             P2r_client.cc 
             P2r_server.cc 
             ${p2r_proto_srcs} 
             ${p2r_grpc_srcs})
target_link_libraries(P2r 
                      ${_REFLECTION}
                      ${_GRPC_GRPCPP}
                      ${_PROTOBUF_LIBPROTOBUF})
target_include_directories(P2r
    PUBLIC
        api
)

add_executable( P2r_example
 P2r_example.c api/P2r_client_api.c api/P2r_server_api.c) 

target_link_libraries( P2r_example
  P2r )
