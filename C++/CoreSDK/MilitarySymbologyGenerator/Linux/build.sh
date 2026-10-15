#!/bin/bash

set -e  # Exit on error

MAPLINK_HOME="${MAPLINK_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)}"
variant="${variant:-debug}"
example_source="example"
BUILD_LIB_ROOT="$MAPLINK_HOME/linux-gcc/x86_64/obj/x86_64-unknown-linux-gnu/$variant"

INC_DIRS=(
    "$MAPLINK_HOME/Prebuilt/$variant/include"
    "/usr/include/X11"
    "/usr/include/Xm"
    "$MAPLINK_HOME/SDK/CoreSDK/api"
    "$MAPLINK_HOME/SDK/CoreSDK/api/geometryapi"
    "$MAPLINK_HOME/SDK/CoreSDK/TGM"
    "$MAPLINK_HOME/SDK/CoreSDK/TMS"
    "$MAPLINK_HOME/SDK/CoreSDK/transforms"
    "$MAPLINK_HOME/dist/x86_64-unknown-linux-gnu/$variant/include"
    "$MAPLINK_HOME/SDK/CoreSDK/TMS/militarysymbology"
)

LIB_DIRS=(
    "$BUILD_LIB_ROOT/CoreSDK/api"
    "$BUILD_LIB_ROOT/CoreSDK/RasterProjection"
    "$BUILD_LIB_ROOT/CoreSDK/TMS"
    "$BUILD_LIB_ROOT/CoreSDK/transforms"
    "$BUILD_LIB_ROOT/CoreSDK/TGM"
    "$BUILD_LIB_ROOT/CoreSDK/CompatibilityHelper"
    "$BUILD_LIB_ROOT/CoreSDK/Compression"
    "$BUILD_LIB_ROOT/CoreSDK/Threading"
    "$BUILD_LIB_ROOT/CoreSDK/XMLExpat"
    "$BUILD_LIB_ROOT/CoreSDK/CustomLineStyles/stroke"
    "$BUILD_LIB_ROOT/CoreSDK/CustomLineStyles/weatherfront"
    "$BUILD_LIB_ROOT/CoreSDK/TRASH/Unix_LeadTools"
    "$BUILD_LIB_ROOT/DirectImport"
    "$BUILD_LIB_ROOT/Filters/generic"
    "$BUILD_LIB_ROOT/TerrainSDK"
    "$MAPLINK_HOME/dist/x86_64-unknown-linux-gnu/$variant/lib64"
    "$MAPLINK_HOME/Prebuilt/$variant/lib64"
    "$MAPLINK_HOME/Prebuilt/$variant/lib"
)

LIBS=(
    X11 Xt Xm MapLink ttltrashx MapLinkDirectImport GenericFilter ttlrpj MapLinkTerrain
    gts ttltms ttltxf ttltgm ttlch ttlcompression ttlmt gdal ttlexpat cryptopp
    minizip tbb tbbmalloc openjp2 png16 pcre curl-d jpeg GeographicLib pango-1.0 harfbuzz
)

INCLUDE_FLAGS=()
for dir in "${INC_DIRS[@]}"; do
    INCLUDE_FLAGS+=("-I" "$dir")
done

LIB_FLAGS=()
for dir in "${LIB_DIRS[@]}"; do
    LIB_FLAGS+=("-L" "$dir")
done

LIB_LINKS=()
for lib in "${LIBS[@]}"; do
    LIB_LINKS+=("-l$lib")
done

LIB_RPATHS="./"
for lib_path in "${LIB_DIRS[@]}"; do
    LIB_RPATHS+=":$lib_path"
done

g++ -c -std=c++17 -pipe -g -Wall -W -fPIC "${INCLUDE_FLAGS[@]}" \
    -o $example_source.o $example_source.cpp

g++ -o $example_source $example_source.o \
	/usr/lib64/libgnutls.so.30 \
	/usr/lib64/libhogweed.so.4 \
	/usr/lib64/libnettle.so.6 \
	"${LIB_FLAGS[@]}" "${LIB_LINKS[@]}" \
	-Wl,--disable-new-dtags,-rpath="${LIB_RPATHS[@]}"

LD_LIBRARY_PATH="${LIB_RPATHS#./:}${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
FONTCONFIG_PATH=/etc/fonts/ MAPL_HOME=$MAPLINK_HOME ./$example_source "$@"

