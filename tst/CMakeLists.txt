set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})
# set(CMAKE_CXX_FLAGS "-I/home/exet4718/postgres/include/")

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

# target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest -L/home/exet4718/postgres/lib/ -lpq)