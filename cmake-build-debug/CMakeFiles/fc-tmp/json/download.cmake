cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

message(VERBOSE "Executing download step for json")

block(SCOPE_FOR VARIABLES)

include("X:/Code/C++/Statistics/cmake-build-debug/CMakeFiles/fc-stamp/json/download-json.cmake")
include("X:/Code/C++/Statistics/cmake-build-debug/CMakeFiles/fc-stamp/json/verify-json.cmake")
file(COPY_FILE
  "X:/Code/C++/Statistics/cmake-build-debug/_deps/json-tmp/json.hpp"
  "X:/Code/C++/Statistics/cmake-build-debug/_deps/json-src/json.hpp"
  ONLY_IF_DIFFERENT
  INPUT_MAY_BE_RECENT
)

endblock()
