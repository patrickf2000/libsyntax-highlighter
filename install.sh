#!/bin/bash
if [ ! -d ./build/lib ] ; then
	echo "Error: You must build the program before installing."
	exit 1
fi
cd build/lib
cp ./libsyntax-highlighting.so /usr/local/lib
cp ./libsyntax-highlighting.so.1 /usr/local/lib
cp ./libsyntax-highlighting.so.1.0 /usr/local/lib
cp ./libsyntax-highlighting.so.1.0.0 /usr/local/lib
cd ../..
mkdir -p /usr/local/include/cpplib/syntax-highlighting
cp lib/*.hh /usr/local/include/cpplib/syntax-highlighting
cp lib/*.h /usr/local/include/cpplib/syntax-highlighting
mkdir -p /usr/share/syntax-highlighting
cp ./syntax/*.xml /usr/share/syntax-highlighting
ldconfig
echo "Done!"
