include_directories(../include)
include_directories(../protocal)
#link_directories(/usr/local/lib)

aux_source_directory(. DIR_SRC)
aux_source_directory(../Network NETWORK)
aux_source_directory(../protocal PROTO)

list(APPEND DIR_SRC ${NETWORK})
list(APPEND DIR_SRC ${PROTO})

add_executable(client ${DIR_SRC})
target_link_libraries(client 
            protobuf
            pthread
            )  
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)