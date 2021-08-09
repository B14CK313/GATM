enable_testing()
add_executable(GATM_TEST testSimple.cpp)
target_link_libraries(GATM_TEST PRIVATE GATM_STATIC doctest)

add_test(NAME GATM_TEST COMMAND GATM_TEST)