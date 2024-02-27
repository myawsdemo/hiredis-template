cmake_minimum_required(VERSION 3.10)
project(RedisTLSExample)

# 添加可执行文件
add_executable(redis_tls_example redis-instance-tls.cpp)

# 寻找 hiredis 库
find_package(hiredis REQUIRED)
if (hiredis_FOUND)
    target_include_directories(redis_tls_example PRIVATE ${hiredis_INCLUDE_DIRS})
    target_link_libraries(redis_tls_example PRIVATE ${hiredis_LIBRARIES})
else ()
    message(FATAL_ERROR "hiredis not found")
endif ()

# 寻找 OpenSSL 库
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    target_include_directories(redis_tls_example PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(redis_tls_example PRIVATE ${OPENSSL_LIBRARIES})
else ()
    message(FATAL_ERROR "OpenSSL not found")
endif ()

# 链接其他可能需要的库（例如 pthread）
target_link_libraries(redis_tls_example PRIVATE dl pthread)