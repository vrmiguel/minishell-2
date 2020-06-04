# minishell-2
Unix shell written in pure C++11.

## Building

### With QMake

Run ```qmake && make``` on the root folder.

### Without QMake

Run 
``` g++ -c -pipe -std=gnu++11 -Wall -W -fPIC -I. Sources/main.cpp Sources/opsys.cpp Sources/input.cpp ``` followed by ``` g++ -Wl,-O1 -o minishell-2 input.o main.o opsys.o ```
