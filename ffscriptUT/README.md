# ffscriptUT
Ffscript's Unit test project

# Prerequisites
Conan version 1.9.2 or higher.

# How to build

1. You may need to add 'bincrafters' into conan remote repositories.  
```
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```
2. Create 'conan' directory.  

3. Run command.  
This project uses multi-configurations, so use following commands.
```
conan install .. -g cmake_multi -s build_type=Release
conan install .. -g cmake_multi -s build_type=Debug
```