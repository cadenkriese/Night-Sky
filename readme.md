# Night Sky
Night Sky is my final project for CSCI 200: Foundations of Programming at the Colorado School of Mines. It takes date, time, latitude and longitude as input, and renders the stars that would be seen at that time and location using OpenGL. 

# Compiling
If on Mac or Linux, install sdl3 with Homebrew: `brew install sdl3`. If on Windows, download a development (devel) release of SDL3 for your system from https://github.com/libsdl-org/SDL/releases, decompress it and place it in the dependencies folder, and name it "SDL3_Windows".

Also, download the latest release of glm from https://github.com/g-truc/glm/releases, decompress it and place it in dependencies. Make sure it is named "glm".

From there, run `make`, wait for it to compile, and run the Night_Sky executable.
