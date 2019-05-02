
if (CMAKE_VERSION VERSION_LESS 2.8.3)
    message(FATAL_ERROR "Qt 5 requires at least CMake version 2.8.3")
endif()

macro(_qt5_HelpTools_check_file_exists file)
    if(NOT EXISTS "${file}" )
        message(FATAL_ERROR "The package \"Qt5HelpTools\" references the file
   \"${file}\" but this file does not exist. ")
    endif()
endmacro()


if (NOT TARGET Qt5::qcollectiongenerator)
    add_executable(Qt5::qcollectiongenerator IMPORTED)

	if(WIN32)
		set(imported_location "${QT5_BASE_DIR}/bin/qcollectiongenerator.exe")
		_qt5_HelpTools_check_file_exists(${imported_location})
	else(WIN32)
    	set(imported_location "/usr/lib/x86_64-linux-gnu/qt5/bin/qcollectiongenerator")
    	_qt5_HelpTools_check_file_exists(${imported_location})
	endif(WIN32)

    set_target_properties(Qt5::qcollectiongenerator PROPERTIES
        IMPORTED_LOCATION ${imported_location}
    )
endif()

set(Qt5_QCOLLECTIONGENERATOR_EXECUTABLE Qt5::qcollectiongenerator)
                                                                                   