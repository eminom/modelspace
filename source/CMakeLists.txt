cmake_minimum_required(VERSION 2.8)
project(ModelTest)
aux_source_directory(. MODELTEST_SRC)
add_executable(ModelTest ${MODELTEST_SRC})


