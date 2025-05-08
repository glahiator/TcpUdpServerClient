# conan install . -pr=conan_cpp20.profile --build=missing
[settings]
compiler=gcc
compiler.version=11
compiler.cppstd=20
# Или gnu17, если нужны расширения GNU
os=Linux
arch=x86_64
compiler.libcxx=libstdc++11
