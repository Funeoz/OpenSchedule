cmake_minimum_required(VERSION 3.26)
project(OpenSchedule)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(partie1)
include_directories(partie2)
include_directories(partie3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(OpenSchedule_part2
        partie1/list.c
        partie1/list.h
        partie1/test.c
        partie1/test.h
        partie2/search.c
        partie2/search.h
        demo.c
        timer.c
        timer.h
        utils.c
        utils.h)

add_executable(OpenSchedule_part3
        partie3/agenda.c
        partie3/agenda.h
        partie3/complexity.c
        partie3/complexity.h
        partie3/menu.c
        partie3/menu.h
        partie3/noms.txt
        partie3/prenoms.csv
        main.c
        timer.c
        timer.h
        utils.c
        utils.h)
