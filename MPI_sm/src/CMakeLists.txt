# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
#Create a variable called linalg_SOURCES containing all .cpp files:

set(secondDerivative_Src 
    src/main.c
    src/secondDerivative.c
    src/setFun.c
    PARENT_SCOPE
    )

