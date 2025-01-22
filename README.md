# How to install and build


To run this program, you'll need :
- `NPM`
- `MinGW` or `WSL` with `gcc`


## Installation
First, if not already in your system, you'll need to install node :
```sh
winget install Schniz.fnm
fnm install 22
```

Then, using NPM, install the lux-ai program :
```sh
npm install -g @lux-ai/2021-challenge@latest
```

## Build

Compile using cmake
```shell
cmake --build lux_2021
```

If you don't want to use CMake, you can run
```shell
g++ main.cpp -O3 -std=c++11 -o main.out
lux-ai-2021 main.out main.out --out=replay.json
```
