#!/bin/bash
for head in $(find -name '*.tête')
do
	rm /usr/include/$head -v
done
