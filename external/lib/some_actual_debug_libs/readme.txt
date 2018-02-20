Assimp's release library is included in debug-builds for performance reasons.
If you want to include Assimp's debug library, make sure to move both to the respective "debug"-folders: the .lib and the .dll

stb_image's release library is included in debug-builds for performance reasons.
It is a static library, copy the .lib in this folder to the respective "debug"-folder to include the debug-build in your application.