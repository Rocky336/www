cd libs
cmd /c compile.bat

cd ..
g++ Main.cpp libs/libs.a -lws2_32 -o Main.exe