cmake_minimum_required(VERSION 3.28)
project(c__Project)

set(CMAKE_CXX_STANDARD 17)
# set(FETCHCONTENT_FULLY_DISCONNECTED ON) #增量拉取

# 3-rd lib
include(FetchContent)
FetchContent_Declare(
        cpp_thread_pool #线程池
        GIT_REPOSITORY http://github.com/mtrebi/thread-pool.git
        GIT_TAG master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/thread-pool
)
# FetchContent_Declare(
#         cJSON #JSON数据解析
#         GIT_REPOSITORY http://github.com/DaveGamble/cJSON.git
#         GIT_TAG master
#         SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/cJSON
# )
FetchContent_Declare(
        aes #加密解密
        GIT_REPOSITORY http://github.com/kokke/tiny-AES-c.git
        GIT_TAG master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/aes
)
#BASE64存在linux依赖
FetchContent_Declare(
        base64
        GIT_REPOSITORY http://github.com/jwerle/b64.c.git
        GIT_TAG master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/b64
)
FetchContent_Declare(
        fltk
        GIT_REPOSITORY https://github.com/fltk/fltk.git
        GIT_TAG master
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/fltk
)

# 拷贝源码来手动构建cJSON
# FetchContent_MakeAvailable(cJSON)
FetchContent_MakeAvailable(cpp_thread_pool)
FetchContent_MakeAvailable(base64)
FetchContent_MakeAvailable(aes)
FetchContent_MakeAvailable(fltk)

# 处理BASE64编译成静态库
set(BASE64_SOURCE_DIR "${PROJECT_SOURCE_DIR}/lib/b64")
file(GLOB BASE64_LIB_SOURCES "${BASE64_SOURCE_DIR}/*.c")
#message(WARNING ${BASE64_LIB_SOURCES})
list(REMOVE_ITEM BASE64_LIB_SOURCES "${BASE64_SOURCE_DIR}/test.c")
#message(WARNING ${BASE64_LIB_SOURCES})
add_library(
        base64
        STATIC
        ${BASE64_LIB_SOURCES}
)

# project
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(c__Project
        src/cJSON.c
        src/main.cpp
        src/dump.cpp
        src/app.cpp
        src/editor.cpp
)

# 引入cpp_thread_pool头文件
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/thread-pool/include)
# target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/cJSON)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/aes)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/b64)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/fltk)

# 引入3-rd lib
target_link_libraries(${PROJECT_NAME} PRIVATE base64)
# target_link_libraries(${PROJECT_NAME} PRIVATE cjson)
target_link_libraries(${PROJECT_NAME} PRIVATE tiny-aes)
target_link_libraries(${PROJECT_NAME} PRIVATE fltk)