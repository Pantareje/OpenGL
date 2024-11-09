#!/bin/bash

PROJECT=$(realpath "$1" 2> /dev/null)
NAME=$(basename ${PROJECT} 2> /dev/null)

if [ ! -d "$PROJECT" ]; then
	echo "El directorio del proyecto no existe." >&2
	exit 1
fi

SOURCE_FILES=$(find "$PROJECT/src" -type f -name "*.c")

cd "$(dirname $BASH_SOURCE[0])"

if [ $? != 0 ]; then
	echo "No se puede acceder al directorio de run.sh." >&2
	exit 1
fi

if [ ! -d bin ]; then
	mkdir "bin"
	if [ $? != 0 ]; then
		echo "No se pudo crear un directorio para la aplicación compilada." >&2
		exit 1
	fi
fi

CFLAGS="-Wall -Werror -std=c11 -Iglad/include"
LDFLAGS="-lm -lglfw"

BIN_DIR=$(realpath "bin")

if cc ${CFLAGS} -o "${BIN_DIR}/${NAME}" ${SOURCE_FILES} glad/src/glad.c ${LDFLAGS}; then
	if [ -d "${PROJECT}/data" ]; then
		cd "${PROJECT}/data"
	fi

	"${BIN_DIR}/${NAME}" "${@:2}"
	exit $?
else
	echo "Error al compilar." >&2
	exit 1
fi
