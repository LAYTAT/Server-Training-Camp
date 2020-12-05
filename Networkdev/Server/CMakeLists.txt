include_directories(../include)
include_directories(../protocal)
include_directories(../components)
include_directories(../systems)
include_directories(/usr/include/mysql)
include_directories(/usr/local/include/mysql++)



link_directories(
	/usr/lib64/mysql
	)


aux_source_directory(. DIR_SRC)
aux_source_directory(../Network NETWORK)
aux_source_directory(../protocal PROTO)
aux_source_directory(../systems SYSTEM)


list(APPEND DIR_SRC ${NETWORK})
list(APPEND DIR_SRC ${PROTO}) 
list(APPEND DIR_SRC ${SYSTEM}) 

add_definitions("-Wall -g")

add_executable(server ${DIR_SRC})
target_link_libraries(server
    protobuf
    pthread
    mysqlpp
    #libmysqlclient.so
    mysqlclient
    nsl
    z
    m
    redis++
    hiredis
    )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)



