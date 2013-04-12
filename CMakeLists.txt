
cmake_minimum_required(VERSION 2.8)

PROJECT(VesselViewer)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(VesselViewer VesselViewer)

if(VTK_LIBRARIES)
  target_link_libraries(VesselViewer ${VTK_LIBRARIES})
else()
  target_link_libraries(VesselViewer vtkHybrid)
endif()
