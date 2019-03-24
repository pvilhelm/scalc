cmake_minimum_required(VERSION 3.6.2)

project(main)

set(CMAKE_CXX_STANDARD, 17)
set(CMAKE_C_COMPILER, gcc)
set(CMAKE_CXX_COMPILER, g++)

#   Catch2 exposes a cygwin bug that makes linking really slow
#   so don't use the single header but build it as a shared lib
file(GLOB CATCH_FILES
    "../Catch2/include/external/*.h"
    "../Catch2/include/external/*.hpp"
    "../Catch2/include/external/*.cpp"
    "../Catch2/include/internal/*.h"
    "../Catch2/include/internal/*.hpp"
    "../Catch2/include/internal/*.cpp"
    "../Catch2/include/reporters/*.h"
    "../Catch2/include/reporters/*.hpp"
    "../Catch2/include/reporters/*.cpp"
)
add_library(catch2 SHARED ${CATCH_FILES})


# Go through .re-file and generate .cc-files from them
file(GLOB files "../lexers/*.re")
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
                        ../main.cc
                        ../lexers/ast.cc
                        ../lexers/lexer_tokens.cc
                        ../lexers/lexer_numlit.cc
                        ../lexers/lexer_operators.cc
                        ../lexers/ast_nodes/bioperator.cc
                        ../lexers/ast_nodes/number.cc
                        ../lexers/ast_nodes/symbol.cc
                        ../lexers/ast_nodes/statement.cc
                        ../lexers/statement_machine.cc
                )

add_executable(test_main)

target_sources(test_main 
                    PRIVATE
                        ../test_main.cc
                        ../lexers/tests/test_lexer_expressions.cc
                        ../lexers/tests/test_lexer_numbers.cc
                        ../lexers/tests/test_ast.cc
                        ../lexers/tests/test_statement_machine.cc
                        ../lexers/tests/test_lexer_operators.cc
                        ../lexers/ast.cc
                        ../lexers/lexer_tokens.cc
                        ../lexers/lexer_numlit.cc
                        ../lexers/lexer_operators.cc
                        ../lexers/ast_nodes/bioperator.cc
                        ../lexers/ast_nodes/number.cc
                        ../lexers/tests/test_eval.cc
                        ../lexers/ast_nodes/symbol.cc
                        ../lexers/ast_nodes/statement.cc
                        ../lexers/statement_machine.cc
                        )

target_link_libraries(test_main catch2)

include_directories(../)
include_directories(../Catch2/include)