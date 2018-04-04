set(SOURCES
  nfa.h
  parse.h
  precedence_parser.cc
)

add_library(libmagex ${SOURCES})
