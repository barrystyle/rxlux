git submodule update --init
cd RandomX
mkdir build
cd build 
cmake ..
make
cd ../..

rm tests

g++ -O2 tests.cpp RandomX/build/librandomx.a -o tests
