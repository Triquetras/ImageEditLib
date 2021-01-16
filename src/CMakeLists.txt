set(src_sources_spec "")
string( TOLOWER "${CMAKE_SYSTEM_NAME}" system_name_lower)
message("system_name_lower " ${system_name_lower})

file(GLOB_RECURSE project_src
  "*.cpp"
)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h *.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)

target_include_directories(${PROJECT_NAME} PUBLIC ${header_dir_list})

target_sources(${PROJECT_NAME} PUBLIC ${project_src} ${src_sources_spec})
