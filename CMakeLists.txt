cmake_minimum_required(VERSION 3.17)
project(libimobiledevice C)

set(CMAKE_C_STANDARD 99)

# list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS c)

FILE(GLOB_RECURSE header_file_list ${PROJECT_SOURCE_DIR}/include/*.h)
FILE(GLOB_RECURSE c_file_list ${PROJECT_SOURCE_DIR}/src/*.c)
FILE(GLOB_RECURSE common_c_file_list ${PROJECT_SOURCE_DIR}/common/*.c)


find_package(libplist)
find_package(libimobiledevice-glue)
find_package(libusbmuxd)
find_package(OpenSSL REQUIRED)
find_package(libzip REQUIRED)





list(APPEND libimobiledevice_source_files    
    ${c_file_list}
    ${common_c_file_list}    
)

set(additional_include_dir "")

IF (WIN32)
    list(APPEND libimobiledevice_source_files    
        win32_port/getopt.c
        win32_port/libgen.c
    )

    list(APPEND additional_include_dir    
        win32_port
    )
ENDIF()


add_library(imobiledevice STATIC ${libimobiledevice_source_files})

target_include_directories(imobiledevice
    PUBLIC         
        include                
        src  
        ${CMAKE_SOURCE_DIR}          
        ${additional_include_dir}
)

target_compile_definitions(imobiledevice PUBLIC 
    PACKAGE_VERSION="2.6.0"
    HAVE_OPENSSL
    HAVE_VASPRINTF
    HAVE_ASPRINTF
)

target_link_libraries(imobiledevice PUBLIC 
    libplist::libplist
    libimobiledevice-glue::libimobiledevice-glue
    libusbmuxd::libusbmuxd
    openssl::openssl
)

# ole32 
# ws2_32 
# gdi32 
# Iphlpapi

set_target_properties(imobiledevice PROPERTIES 
    PUBLIC_HEADER "${header_file_list}"
)

# Tools / CLI
list(APPEND idevice_id_source    
    tools/idevice_id.c     
)
IF (WIN32)
    list(APPEND idevice_id_source    
        win32_port/getopt.c
        win32_port/libgen.c
    )
ENDIF()
add_executable(idevice_id ${idevice_id_source})
target_link_libraries(idevice_id PRIVATE imobiledevice)


# Tools / CLI
list(APPEND idevicepair_source    
    tools/idevicepair.c
)
IF (WIN32)
    list(APPEND idevicepair_source    
        win32_port/getopt.c
        win32_port/libgen.c
    )
ENDIF()
add_executable(idevicepair
    ${idevicepair_source}
)

target_link_libraries(idevicepair PRIVATE 
    imobiledevice
    libzip::zip
)




# Tools / CLI
list(APPEND ideviceinstaller_source    
    tools/ideviceinstaller.c 
)
IF (WIN32)
    list(APPEND ideviceinstaller_source    
        win32_port/getopt.c
        win32_port/libgen.c
    )
ENDIF()
add_executable(ideviceinstaller 
    ${ideviceinstaller_source}
)

target_link_libraries(ideviceinstaller PRIVATE 
    imobiledevice
    libzip::zip
)


install(
    TARGETS imobiledevice
    EXPORT plist_targets
    PUBLIC_HEADER DESTINATION "include/libusbmuxd"
    RUNTIME DESTINATION "bin/x64/$<$<PLATFORM_ID:Windows>:windows>$<$<PLATFORM_ID:Linux>:linux>$<$<CONFIG:Debug,DebugASan,DebugTSan>:/debug>"
    ARCHIVE DESTINATION "lib/x64/$<$<PLATFORM_ID:Windows>:windows>$<$<PLATFORM_ID:Linux>:linux>$<$<CONFIG:Debug,DebugASan,DebugTSan>:/debug>"
    LIBRARY DESTINATION "lib/x64/$<$<PLATFORM_ID:Windows>:windows>$<$<PLATFORM_ID:Linux>:linux>$<$<CONFIG:Debug,DebugASan,DebugTSan>:/debug>"
)

