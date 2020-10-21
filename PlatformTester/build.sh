dpkg -s cmake make  2>/dev/null >/dev/null || (sudo apt-get update && sudo apt-get install cmake make)
if [ -d "build" ]; then rm -rf "build"; fi
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=release32 -DTEST_LOCAL=OFF ..
make
#./TestServer &
./platform_tester
