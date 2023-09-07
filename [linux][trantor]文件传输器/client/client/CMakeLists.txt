# CMakeList.txt: client 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

include_directories(trantor)

# 将源代码添加到此项目的可执行文件。
add_executable (client "client.cpp" "client.h" "protocol.cpp" "protocol.h" "protocolBuilder.cpp" "protocolBuilder.h" "protocolConverter.cpp" "protocolConverter.h" "protocolOption.cpp" "protocolOption.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET client PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries(client trantor)

# TODO: 如有需要，请添加测试并安装目标。
