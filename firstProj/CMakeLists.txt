# CMakeList.txt : CMake project for cppTest, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (firstProj "firstProj.cpp"  "ServerSocket.cpp" "FirstThread.cpp"  "Synchro.h" "IServer.h" "SecondThread.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET firstProj PROPERTY CXX_STANDARD 20)
endif()
# TODO: Add tests and install targets if needed.

