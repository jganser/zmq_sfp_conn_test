cmake_minimum_required(VERSION 3.20)
project(SFP_TEST VERSION 0.1)

find_package(Threads REQUIRED)
find_package(PkgConfig)

pkg_search_module(LIBCSP libcsp)
file(REAL_PATH "../libcsp/include" csp_inc)

add_executable(img_rcv csp_client_posix.c
  img.c
  minimal_zmq_client.c
)

add_executable(img_sender csp_client_posix.c
  img_sender.c
  minimal_zmq_client.c
)

target_include_directories(img_rcv     PRIVATE ${csp_inc})
target_include_directories(img_sender  PRIVATE ${csp_inc})

target_link_libraries(img_rcv    PRIVATE csp Threads::Threads)
target_link_libraries(img_sender PRIVATE csp Threads::Threads)
