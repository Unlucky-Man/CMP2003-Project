cmake_minimum_required(VERSION 3.23)
project(CMP2003Project_Final)

set(CMAKE_CXX_STANDARD 14)

add_executable(CMP2003Project_Final main.cpp createAllList.h cosineSimilarity.h ratePredictor.h)
set(CMAKE_CXX_STANDARD 14)

configure_file(train.csv
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(test.csv
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)



