# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        INCLUDEPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        DEPENDPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        LIBS += -lbbsystem \
                -lbb

        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.cpp) \
                 $$quote($$BASEDIR/src/media/FileType.cpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.cpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.hpp) \
                 $$quote($$BASEDIR/src/media/FileType.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile_p.hpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.hpp)
    }

    CONFIG(release, debug|release) {
        INCLUDEPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        DEPENDPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        LIBS += -lbbsystem \
                -lbb

        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.cpp) \
                 $$quote($$BASEDIR/src/media/FileType.cpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.cpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.hpp) \
                 $$quote($$BASEDIR/src/media/FileType.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile_p.hpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.hpp)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        INCLUDEPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        DEPENDPATH +=  $$quote(${QNX_TARGET}/usr/include/bb) \
                 $$quote(${QNX_TARGET}/usr/include/bb/system)

        LIBS += -lbbsystem \
                -lbb

        SOURCES +=  $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.cpp) \
                 $$quote($$BASEDIR/src/media/FileType.cpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.cpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/media/FilePerimeter.hpp) \
                 $$quote($$BASEDIR/src/media/FileType.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile.hpp) \
                 $$quote($$BASEDIR/src/media/MediaFile_p.hpp) \
                 $$quote($$BASEDIR/src/media/MediaLibrary.hpp)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src/media) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../src/media/*.c) \
             $$quote($$BASEDIR/../src/media/*.c++) \
             $$quote($$BASEDIR/../src/media/*.cc) \
             $$quote($$BASEDIR/../src/media/*.cpp) \
             $$quote($$BASEDIR/../src/media/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)