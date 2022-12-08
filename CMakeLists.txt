# Minimum CMake required
cmake_minimum_required(VERSION 3.15)

# Project
project(P2r)

# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED HINTS "/projects/gRPC/install/lib/cmake/protobuf")
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED HINTS "/projects/gRPC/install/lib/cmake/grpc")
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(p2r_proto "protobuf/proto/P2r.proto" ABSOLUTE)
get_filename_component(p2r_proto_path "${p2r_proto}" PATH)

# Generated sources
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

# Include generated *.pb.h files
include_directories("../../protobuf/classes/")

# Targets (client|server)
foreach(_target
  P2r_client P2r_server)
  add_executable(${_target} "${_target}.cc"
    ${p2r_proto_srcs}
    ${p2r_grpc_srcs})
  target_link_libraries(${_target}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
