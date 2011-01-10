#!/bin/sh

llvm-regexp -p _lex_RE '[[:space:]]+' '[[:alpha:]_][[:alnum:]_]*' '<[[:alpha:]_][[:alnum:]_]*>' '!=.*\n' | llc -filetype=obj -o _lex_RE.o
