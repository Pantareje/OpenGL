#!/bin/bash

# TODO: Añadir robustez para el directorio de trabajo y argumentos.

PROJECT=$1

if [ ! -d "$PROJECT" ]; then
	echo "El directorio del proyecto no existe." >&2
	exit 1
fi

if [ ! -d bin ]; then
	mkdir bin
fi

SOURCE_FILES=$(find "$PROJECT/src" -type f -name "*.c")
BIN_DIR=$(realpath bin)

CFLAGS="-Wall -Werror -std=c11 -Iglad/include"
LDFLAGS="-lm -lglfw"

if cc ${CFLAGS} -o "${BIN_DIR}/${PROJECT}" ${SOURCE_FILES} glad/src/glad.c ${LDFLAGS}; then
	if [ -d "${PROJECT}/data" ]; then
		cd "${PROJECT}/data"
	fi

	"${BIN_DIR}/${PROJECT}" "${@:2}"
fi
