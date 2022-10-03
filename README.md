#PACMAN

## Requirements
  - C compiler
  - ncurses library


## Running PACMAN

1. Linux/Mac OS

    - To run the file on a UNIX based system, type the following commands in the terminal: 

```bash
  clang pacman.c -lncurses
  ./a.out
```
    
2. Windows

   - To run the file on a Windows system, type the following commands (use correct path) in CMD:
   - Note: Include /mingw64/include/ncurses and /mingw64/bin directories while compiling.

```bash
  gcc /mingw64/include/ncurses pacman.c -lncurses /mingw64/bin -static
  a.exe
```


## Screenshots
<img width="500" alt="pacman" src="https://user-images.githubusercontent.com/91797078/190451842-115789fe-cf88-483d-8e11-d1ebed0f2fb1.png">
