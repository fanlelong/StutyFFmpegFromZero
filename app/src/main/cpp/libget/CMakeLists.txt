cmake_minimum_required(VERSION 3.10.2)

file(GLOB SOURCE (*.c))

add_library(
        get
        STATIC
        ${SOURCE}
)
