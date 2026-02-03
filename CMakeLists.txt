cmake_minimum_required(VERSION 3.10)
project(ParallelPrefixSum VERSION 1.0)

# 1. 設置 C++ 標準 (面試官愛看 C++17 或更高)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 開啟編譯優化 (-O3)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 3. 尋找 Thread 庫 (這會自動處理 -pthread 旗標)
find_package(Threads REQUIRED)

# 4. 指定標頭檔目錄
include_directories(include)

# 5. 定義可執行檔
add_executable(parallel_sum src/main.cpp)

# 6. 連結執行緒庫
target_link_libraries(parallel_sum PRIVATE Threads::Threads)
