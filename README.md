# C++ Injector

### Compile DLL

<code>g++ -shared -o exampleDLL.dll exampleDLL.cpp</code>

### Compile Injecrot

<code>g++ Injector.cpp -static -o Injector.exe -lws2_32 -static-libgcc -static-libstdc++</code>
