cmake_minimum_required(VERSION 2.8)
project(cJSON)
set (CJSON_SRC  cJSON.c)
add_library(libcjson ${CJSON_SRC})



