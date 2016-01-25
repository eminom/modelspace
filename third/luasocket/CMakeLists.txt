cmake_minimum_required(VERSION 2.8)

if(WIN32)
	set(LUASOCKET_SRC
		luasocket.c
		timeout.c
		buffer.c
		io.c
		auxiliar.c
		compat.c
		options.c
		inet.c
		except.c
		select.c
		tcp.c
		udp.c
		wsocket.c
		mime.c
	)
else()
	set(LUASOCKET_SRC
		luasocket.c
		timeout.c
		buffer.c
		io.c
		auxiliar.c
		compat.c
		options.c
		inet.c
		except.c
		select.c
		tcp.c
		udp.c
		mime.c
		usocket.c
	)
	endif()
include_directories(".")
include_directories("../lua5p1var/src")
add_library(libluasocket ${LUASOCKET_SRC})


