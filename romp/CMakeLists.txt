# Define resources.
add_custom_command(
  OUTPUT resources_manpage.cc
  COMMAND ../misc/xxd.py doc/romp.1 ${CMAKE_CURRENT_BINARY_DIR}/resources_manpage.cc
  MAIN_DEPENDENCY doc/romp.1
  DEPENDS ../misc/xxd.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(
  OUTPUT resources_c_prefix.cc
  COMMAND ../misc/xxd.py resources/c_prefix.c ${CMAKE_CURRENT_BINARY_DIR}/resources_c_prefix.cc
  MAIN_DEPENDENCY resources/c_prefix.c
  DEPENDS ../misc/xxd.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# add_custom_command(
#   OUTPUT resources_h_prefix.cc
#   COMMAND ../misc/xxd.py resources/h_prefix.h ${CMAKE_CURRENT_BINARY_DIR}/resources_h_prefix.cc
#   MAIN_DEPENDENCY resources/h_prefix.h
#   DEPENDS ../misc/xxd.py
#   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(# our main definition file for rw
  OUTPUT resources_romp-rw.cc
  COMMAND ../misc/xxd.py resources/romp-rw.hpp ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw.ccm.cc
  MAIN_DEPENDENCY resources/romp-rw.hpp
  DEPENDS ../misc/xxd.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(# file with our main function 
  OUTPUT resources_romp-rw-main.cc
  COMMAND ../misc/xxd.py resources/romp-rw-main.hpp ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw-main.cc
  MAIN_DEPENDENCY resources/romp-rw-main.hpp
  DEPENDS ../misc/xxd.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(# file with CLI config options
  OUTPUT resources_romp-rw-options.cc
  COMMAND ../misc/xxd.py resources/romp-rw-options.hpp ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw-options.cc
  MAIN_DEPENDENCY resources/romp-rw-options.hpp
  DEPENDS ../misc/xxd.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(romp
  ${CMAKE_CURRENT_BINARY_DIR}/resources_c_prefix.cc
  # ${CMAKE_CURRENT_BINARY_DIR}/resources_h_prefix.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resources_manpage.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw-main.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resources_romp-rw-options.cc
  ../common/escape.cc
  ../common/help.cc
  src/check.cc
  src/CLikeGenerator.cc
  src/CodeGenerator.cc
  src/compares_complex_values.cc
  src/generate_c.cc
  # src/generate_h.cc
  src/main.cc
  src/options.cc
  src/romp_def.cpp
  src/model/IModel.cpp
  )

target_include_directories(romp
  PRIVATE
  src
  ${CMAKE_CURRENT_BINARY_DIR}/../librumur)

target_link_libraries(romp
  librumur)

# Compress manpage
add_custom_target(man-romp
  ALL
  DEPENDS romp.1.gz)
add_custom_command(
  OUTPUT romp.1.gz
  COMMAND gzip -9 --no-name --to-stdout doc/romp.1 >"${CMAKE_CURRENT_BINARY_DIR}/romp.1.gz"
  MAIN_DEPENDENCY doc/romp.1
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS romp
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/romp.1.gz
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
