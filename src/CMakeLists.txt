set(SOURCES
  nfa.h
  nfa.cc
  parse.h
  precedence_parser.cc
)

add_library(libmagex ${SOURCES})
set_target_properties(libmagex PROPERTIES
  LIBRARY_OUTPUT_DIR ../lib
)
