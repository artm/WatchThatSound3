#!/bin/bash

for header in */lib/*.hpp ; do
  component=$(dirname $(dirname $header))
  basename=$(basename $header .hpp)
  public="$component/$basename"
  if [ ! -f $public ] ; then
    echo "Adding $public"
    echo "#include \"lib/${basename}.hpp\"" > $public
    git add $public
  fi
done
