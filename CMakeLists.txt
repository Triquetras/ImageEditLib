cmake_minimum_required(VERSION 3.13)
project(libimageeditlib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wformat -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_DEFINE_MATH_OPERATORS -DIMGUI_IMPL_OPENGL_LOADER_GLEW -DGLFW_INCLUDE_NONE")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC include})

add_subdirectory(libs)
add_subdirectory(src)

string( TOLOWER "${CMAKE_SYSTEM_NAME}" system_name_lower)

if (${system_name_lower} STREQUAL "linux")
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
    include_directories(/usr/local/include /usr/local/opt /usr/include)
endif()

if (${system_name_lower} STREQUAL "darwin")
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVID_LIBRARY CoreVideo REQUIRED)
  target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY} ${COREVID_LIBRARY} ${IOKIT_LIBRARY})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit")
  include_directories(/usr/local/include /usr/local/opt)
endif()

target_link_libraries(${PROJECT_NAME} GL pthread imagearch glog crypto stdc++fs)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

find_library(PTHREAD_LIBRARY pthread)
target_link_libraries(${PROJECT_NAME} pthread)

find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARY} ${CMAKE_DL_LIBS})

find_package(GLEW REQUIRED STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_search_module(GLFW REQUIRED glfw3)
  if (GLFW_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})
  endif()
  include_directories(${GLFW_INCLUDE_DIRS})

  if (${system_name_lower} STREQUAL "linux")
    PKG_CHECK_MODULES(GTK3 REQUIRED "gtk+-3.0")
    if (GTK3_FOUND)
      target_include_directories(${PROJECT_NAME} PUBLIC ${GTK3_INCLUDE_DIRS})
      target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
      add_definitions(${GTK3_CFLAGS} ${GTK3_CFLAGS_OTHER})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GTK_CFLAGS} ${GTK3_CFLAGS_OTHER}")
      set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${GTK3_CFLAGS}")
    endif()
  endif()
endif()

if (${system_name_lower} STREQUAL "darwin")
  add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND rm -rf bin/${PROJECT_NAME}.app
    	COMMAND mkdir -p bin/${PROJECT_NAME}.app/Contents/MacOS
    	COMMAND mkdir -p bin/${PROJECT_NAME}.app/Contents/Resources
    	COMMAND mkdir -p bin/${PROJECT_NAME}.app/Contents/MacOS/libs
      COMMAND echo '<?xml version="1.0" encoding="UTF-8"?>' > bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '<plist version="1.0">' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '<dict>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleGetInfoString</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>${PROJECT_NAME}.app</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleExecutable</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>${PROJECT_NAME}.sh</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleIdentifier</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>com.sublirip</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleName</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>${PROJECT_NAME}.app</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleShortVersionString</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>0.01</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundleInfoDictionaryVersion</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>6.0</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>CFBundlePackageType</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <string>APPL</string>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>IFMajorVersion</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <integer>0</integer>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <key>IFMinorVersion</key>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '  <integer>1</integer>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '</dict>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
      COMMAND echo '</plist>' >> bin/${PROJECT_NAME}.app/Contents/Info.plist
    	COMMAND cp -r ${PROJECT_NAME} bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
    	COMMAND cp -r ../${PROJECT_NAME}.sh bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}.sh
    	COMMAND chmod a+x bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}.sh
    	COMMAND cp -r ../res/ bin/${PROJECT_NAME}.app/Contents/Resources/
    	COMMAND cp -r /usr/local/opt/glfw/lib/libglfw.3.3.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/libs/libglfw.3.dylib
    	COMMAND cp -r /usr/local/opt/libusb/lib/libusb-1.0.0.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/libs/libusb-1.0.0.dylib
    	COMMAND cp -r /usr/local/opt/glog/lib/libglog.0.4.0.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/libs/libglog.0.dylib
    	COMMAND cp -r /usr/local/opt/openssl/lib/libcrypto.1.1.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/libs/libcrypto.1.1.dylib
      COMMAND install_name_tool -change /usr/local/opt/glfw/lib/libglfw.3.dylib @executable_path/libs/libglfw.3.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
	    COMMAND install_name_tool -change /usr/local/opt/libusb/lib/libusb-1.0.0.dylib @executable_path/libs/libusb-1.0.0.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
	    COMMAND install_name_tool -change /usr/local/opt/glog/lib/libglog.0.dylib @executable_path/libs/libglog.0.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
	    COMMAND install_name_tool -change /usr/local/opt/openssl@1.1/lib/libcrypto.1.1.dylib @executable_path/libs/libcrypto.1.1.dylib bin/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
      COMMENT "Create OSX Bundle"
      VERBATIM
  )
endif()


  add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND echo -e "Done"
      VERBATIM
  )
