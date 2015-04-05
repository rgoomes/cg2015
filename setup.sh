mkdir build
cp -R objects build/objects
cp run build/run
cd build
cmake ..
make