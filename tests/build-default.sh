#!/bin/sh
# Verify the config.def.h fallback without changing the checkout's config.h.
set -eu
root=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd -P)
build_dir=$(mktemp -d)
trap 'rm -rf "$build_dir"' EXIT HUP INT TERM
for file in Makefile config.mk config.def.h dwm.c drw.c drw.h util.c util.h; do
    cp "$root/$file" "$build_dir/$file"
done
make -C "$build_dir"
