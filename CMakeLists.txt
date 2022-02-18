# LEONID PERLIN 2022 <perlinleo@gmail.com>

#               _-o#&&*''''?d:>b\_
#           _o/"`''  '',, dMF9MMMMMHo_
#        .o&#'        `"MbHMMMMMMMMMMMHo.
#      .o"" '         vodM*$&&HMMMMMMMMMM?.
#     ,'              $M&ood,~'`(&##MMMMMMH\
#    /               ,MMMMMMM#b?#bobMMMMHMMML
#   &              ?MMMMMMMMMMMMMMMMM7MMM$R*Hk
#  ?$.            :MMMMMMMMMMMMMMMMMMM/HMMM|`*L
# |               |MMMMMMMMMMMMMMMMMMMMbMH'   T,
# $H#:            `*MMMMMMMMMMMMMMMMMMMMb#}'  `?
# ]MMH#             ""*""""*#MMMMMMMMMMMMM'    -
# MMMMMb_                   |MMMMMMMMMMMP'     :
# HMMMMMMMHo                 `MMMMMMMMMT       .
# ?MMMMMMMMP                  9MMMMMMMM}       -
# -?MMMMMMM                  |MMMMMMMMM?,d-    '
#  :|MMMMMM-                 `MMMMMMMT .M|.   :
#   .9MMM[                    &MMMMM*' `'    .
#    :9MMk                    `MMM#"        -
#      &M}                     `          .-
#       `&.                             .
#         `~,   .                     ./
#             . _                  .-
#               '`--._,dd###pp=""'

# WORLD WIDE WEB 2022


cmake_minimum_required(VERSION 3.7.2)
project(highload-c-webserver)

# CMAKE FLAGS
set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage")


# LIB
set(HIGHLOAD_SERVER_LIB_PATH ${PROJECT_SOURCE_DIR})

file(GLOB LIB_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)


add_library(highload_server_lib STATIC
    ${LIB_SOURCES}
)

target_include_directories(highload_server_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)



set_target_properties(highload_server_lib PROPERTIES LINKER_LANGUAGE C)
#
# LIB


# CLI
# 
add_executable(highload_server cli/main.c)

target_link_libraries( highload_server 
    PRIVATE
        highload_server_lib
)

# CLI
