mkdir gtest
cd gtest
cmake /usr/src/gtest
cmake --build .
sudo mv libgtest.a /usr/lib
sudo mv libgtest_main.a /usr/lib
