cmake_minimum_required(VERSION 3.20)

# index タスクは SHARED 実務タスクから複数参照されるため。
add_library(index SHARED
    src/index_api.c
    src/index_api_proc.cpp
    src/index_task_impl.cpp
    src/index.cpp
    src/index_task_entry.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/out_inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/common/task_manager/inc
)

# 巨大データを扱うので OS API が必要ならここでリンク
target_link_libraries(index
        engine_core   # 必要なら
)
