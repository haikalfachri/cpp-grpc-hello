cmake_minimum_required(VERSION 3.5.0)
project(helloworld VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
# SET(CMAKE_EXE_LINKER_FLAGS  "/FORCE")

include(CTest)
enable_testing()

include_directories(
  "D:\\vcpkg\\installed\\x64-windows\\include"
)
link_directories(
  "D:\\vcpkg\\installed\\x64-windows\\lib"
)

file(GLOB_RECURSE sources src/*.cpp src/*.cc src/*.h src/*.hpp)

add_executable(main ${sources})

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(main gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(main protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(main cppzmq cppzmq-static)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main nlohmann_json::nlohmann_json)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(main rapidjson)

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(main libpqxx::pqxx)

find_package(unofficial-restbed CONFIG REQUIRED)
target_link_libraries(main unofficial::restbed::restbed)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
