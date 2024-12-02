# CMakeList.txt : CMake project for Statisticka_kalkulacka, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Statisticka_kalkulacka "Statisticka_kalkulacka.cpp" "Statisticka_kalkulacka.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Statisticka_kalkulacka PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
