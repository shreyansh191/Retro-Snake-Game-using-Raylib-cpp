# Retro-Snake-Game-using-Raylib-cpp
"RetroSnake" programmed in c++ and ready to go for OS X.

<p align="center">
    <img align="center" width="570" alt="retrosnake_image" src="https://github.com/shreyansh191/RetroSnakeGame/assets/118434205/95840c69-833e-4ea7-bd0a-2875115620ca">
</p>


## Features
* Use Arrow Keys to control the snake
* Press `M` key to change the modes
  -  **Fist Mode** - Snake passes through the wall
  -  **Second Mode** - Snake is restricted at the boundary
  -  Each mode has its own High Score!

  
## Installation for OSX

### 1.Install Homebrew (on macOS or Linux)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

More installation information and options: <https://docs.brew.sh/Installation>.

### 2.Install gcc

```bash
brew install gcc
```
Check the version of gcc [use `brew info gcc` alternatively] , if the version is like `13.2.0` , then we use `g++-13`  

### 3. Clone this repo

```bash
git clone https://github.com/shreyansh191/RetroSnakeGame.git
cd RetroSnakeGame
```
Or click on **Download Zip** from `<> Code` pop up menu.

### 4.Compile the cpp file

Replace the compiler `g++-13` or change its version eg- `g++-14` accordingly.

```bash
eval g++-13 retrosnake.cpp -o retrosnake -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L./lib -lraylib
```

Now you are all set with the installation :)

### 5.Run The Game -
```bash
./retrosnake
```

## Extra 
How to build raylib games on different platforms <https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation>

