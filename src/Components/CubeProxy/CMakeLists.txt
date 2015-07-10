# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

USE_ROS(roscpp std_msgs)

# Find OpenCV library files
FIND_PACKAGE( OpenCV REQUIRED )

# Create an executable file from sources:
ADD_LIBRARY(CubeProxy SHARED ${files})

MESSAGE(STATUS "${ROS_LIBRARIES}")

# Link external libraries
TARGET_LINK_LIBRARIES(CubeProxy ${OpenCV_LIBS} ${DisCODe_LIBRARIES} ${ROS_LIBRARIES} )

INSTALL_COMPONENT(CubeProxy)
