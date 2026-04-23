cmake_minimum_required(VERSION 3.20)
project(FaturaPro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# ─── DEPENDÊNCIAS ───────────────────────────────────────────
include(FetchContent)

# cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.42.0
)
FetchContent_MakeAvailable(httplib)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# nlohmann_json (via Homebrew)
find_package(nlohmann_json REQUIRED)

# SQLite3 (via sistema)
find_package(SQLite3 REQUIRED)

# ─── EXECUTÁVEL PRINCIPAL ───────────────────────────────────
add_executable(programa src/main.cpp)
target_include_directories(programa PRIVATE src/)
target_link_libraries(programa PRIVATE
    httplib::httplib
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
)

# ─── EXECUTÁVEL DE TESTES ───────────────────────────────────
add_executable(testes tests/test_cliente_service.cpp)
target_include_directories(testes PRIVATE src/)
target_link_libraries(testes PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
)

add_test(NAME todos_os_testes COMMAND testes)