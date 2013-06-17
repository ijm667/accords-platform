#!/bin/sh -e
#
# Migrate schemas
#
# Copyright 2013 Jean Parpaillon <jean.parpaillon@free.fr>
#
transform=migrate/latest.xsl

if [ $# -lt 1 ]; then
    echo "Usage: $0 schema.xml"
fi

for file in $*; do
    latest=$(xmlstarlet sel -t -v '//occi:schema/@version' ${transform} || true)
    current=$(xmlstarlet sel -t -v '//occi:schema/@version' ${file} 2> /dev/null || true)
    echo -n "$file (version $current): "
    if [ "x${latest}" != "x${current}" ]; then
	echo "migrating to ${latest}"
	xsltproc ${transform} $file > $file.new
	mv $file.new $file
    else
	echo "up-to-date"
    fi
done 

exit 0
