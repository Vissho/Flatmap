set(target_name flatmap)

add_library(${target_name} INTERFACE
    flatmap.hpp
  )


target_include_directories(
  ${target_name}
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(
#  ${target_name}
#  PRIVATE
#    nlohmann_json
#    picosha2
#)
