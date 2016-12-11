# Try to find the Glm library
# 
# This module defines the following variables:
# 
# GLM_FOUND            - If Glm was found
# GLM_INCLUDE_DIR      - The include directory of Glm
# 
# This module will use the following variables:
# 
# GLM_ROOT - This will be used to find Glm
# 

# Defines potential paths for finding Glm
SET(GLM_FIND_PATHS
  ${GLM_ROOT}
  "C:/Program Files (x86)/glm"
  "C:/Program Files/glm"
  $ENV{GLM_ROOT}
  /usr/local/
  /usr/
  /sw
  /opt/local/
  /opt/csw/
  /opt/
)

if (NOT GLM_FIND_QUIETLY)
  message(STATUS "Looking for Glm...")
endif ()

# Look for include folder
find_path(GLM_INCLUDE_DIR NAMES glm/glm.hpp
  HINTS ${GLM_FIND_PATHS}
  PATH_SUFFIXES include)

# If at least one library was found
if (GLM_INCLUDE_DIR)
  # Mark as found
  SET(GLM_FOUND TRUE)
else()
  # Glm was not found
  SET(GLM_FOUND FALSE)
endif()

# Don't show variables to user
mark_as_advanced(
  GLM_INCLUDE_DIR
)

if (GLM_FOUND)
  message(STATUS "-- Found Glm : ${GLM_INCLUDE_DIR}")
else()
  if(GLM_FIND_REQUIRED)
    # Fatal error
    message(FATAL_ERROR "Could NOT find Glm")
  elseif(NOT GLM_FIND_QUIETLY)
    # Error, but continue
    message("Could NOT find Glm")
  endif()
endif()
