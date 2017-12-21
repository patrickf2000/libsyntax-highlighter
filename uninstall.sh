#!/bin/bash
echo "You are about to uninstall libsyntax-highlighter"
echo "Doing so may break some applications."
echo "Do you wish to continue? (y,n)"
read x
if [ $x == "n" ]
then exit 1
fi

echo "Uninstalling..."

cd /usr/local/lib
rm ./libsyntax-highlighting.so
rm ./libsyntax-highlighting.so.1
rm ./libsyntax-highlighting.so.1.0
rm ./libsyntax-highlighting.so.1.0.0

cd /usr/local/include/cpplib
rm -r ./syntax-highlighting

ldconfig

echo "Done"
