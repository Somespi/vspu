# CMakeList.txt : CMake project for vspu, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (vspu "vspu.cpp" "vspu.h" "cpu/cpu.cpp" "cpu/cpu.h" "assembler/assembler.cpp" "assembler/assembler.h" "assembler/lexer/lexer.cpp" "assembler/lexer/lexer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET vspu PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
