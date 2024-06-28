cmake_minimum_required(VERSION 3.10)

project(v4l2view)

find_package(OpenCV REQUIRED)

add_executable(
    ${PROJECT_NAME} 
    main.cpp
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
