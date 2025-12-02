cmake_minimum_required(VERSION 4.0)
project(aoc CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

function(add_year YEAR)
    set(YEAR_SRC_DIR "${CMAKE_SOURCE_DIR}/${YEAR}")
    file(GLOB DAY_DIRS RELATIVE "${YEAR_SRC_DIR}" "${YEAR_SRC_DIR}/*")

    foreach (day_dir ${DAY_DIRS})
        if (day_dir MATCHES "^day-[0-9][0-9]$")
            file(GLOB CPP_FILES "${YEAR_SRC_DIR}/${day_dir}/*.cpp")

            if (CPP_FILES)
                set(INPUT_FILE "${YEAR_SRC_DIR}/${day_dir}/input.txt")
                set(TEST_INPUT_FILE "${YEAR_SRC_DIR}/${day_dir}/test-input.txt")
                set(TARGET_NAME "${YEAR}-${day_dir}")
                set(INPUT_TARGET "${TARGET_NAME}-input")

                add_custom_command(
                        OUTPUT ${INPUT_FILE}
                        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/download_input.py ${YEAR_SRC_DIR}/${day_dir}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        COMMENT "Downloading input for ${YEAR} ${day_dir}"
                        VERBATIM
                )
                add_custom_target(${INPUT_TARGET} DEPENDS ${INPUT_FILE})

                add_executable(${TARGET_NAME} ${CPP_FILES})
                target_compile_definitions(${TARGET_NAME} PRIVATE AOC_INPUT_FILE="${INPUT_FILE}")
                target_compile_definitions(${TARGET_NAME} PRIVATE AOC_TEST_INPUT_FILE="${TEST_INPUT_FILE}")
                target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/common")
                add_dependencies(${TARGET_NAME} ${INPUT_TARGET})
            else ()
                message(WARNING "No .cpp files found in ${day_dir}")
            endif ()
        endif ()
    endforeach ()
endfunction()

add_year(2025)