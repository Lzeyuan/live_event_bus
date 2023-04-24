set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(live_event_bus STATIC live_event_bus.cpp)

target_include_directories(live_event_bus PUBLIC "/")