cmake_minimum_required(VERSION 3.6.2)

project(main)

set(CMAKE_CXX_STANDARD, 17)
set(CMAKE_C_COMPILER, gcc)
set(CMAKE_CXX_COMPILER, g++)

# Go through .re-file and generate .cc-files from them
file(GLOB files "lexers/*.re")
foreach(file ${files})
    message(STATUS "Parsing .re-file ${file}")
    set(file_cc ${file})
    string(REPLACE ".re" ".cc" file_cc ${file_cc})

    #Args for re2c as a list
    set(args "--tags" "-o" "${file_cc}" "${file}")

    string(CONCAT out "${file_cc}")
    string(CONCAT dep "${file}")

    add_custom_command(
        OUTPUT ${out}
        COMMAND re2c ${args}
        DEPENDS ${dep}
        WORKING_DIRECTORY "../lexers/"
    )
endforeach()

add_executable(main)
target_sources(main 
                    PRIVATE
                        main.cc)

add_executable(test_main)
target_sources(test_main 
                    PRIVATE
                        test_main.cc
                        lexers/tests/test_lexer_expressions.cc
                        lexers/tests/test_lexer_numbers.cc
                        lexers/lexer_tokens.cc
                        lexers/lexer_numlit.cc)
include_directories(./)