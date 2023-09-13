cmake_minimum_required(VERSION 3.0.2)
project(mlx90640-thermal-camera)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
)

catkin_package(
 INCLUDE_DIRS
  include
 CATKIN_DEPENDS
  cv_bridge
  roscpp
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(mlx90640
  ./src/main.cpp
  ./src/i2c_driver.cpp
  ./src/mlx90640.cpp
)

target_link_libraries(mlx90640
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

target_include_directories(mlx90640
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_dependencies(mlx90640
  ${catkin_EXPORTED_TARGETS}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(TARGETS mlx90640
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
