#!/bin/bash
for head in $(find -name '*.tête')
do
	cp $head /usr/include/$head -v
done
