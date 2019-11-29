#!/bin/bash

# Get list of headers

set -e -u

for f in $(find $1 -name '*.h');
do
	echo -n "$f "
done
echo ""
