# e186[Library]

A real-time rendering framework for:
* OpenGL 4
* C++17

Environment for which the project is preconfigured:
* Windows 10, 64bit (make sure to have an SDK installed)
* Visual Studio 2019 (Community Edition is fine)
* There's a Linux port at [github.com/lemurni/Engine186-Linux](https://github.com/lemurni/Engine186-Linux) (It's a bit out of date, though)

Setup:
* Clone the repository 
* Open the solution file [visual_studio/e186_framework_and_example.sln](https://github.com/cg-tuwien/e186/tree/master/visual_studio/e186_framework_and_example.sln) with Visual Studio 2019
* Build and run (Make sure that _"example"_ is selected as the start-up project; _"e186"_ is the library project which contains the framework)

# Example Code

The example project located at [visual_studio/example](https://github.com/cg-tuwien/e186/tree/master/visual_studio/example) and it's single implementation file [visual_studio/example/main.cpp](https://github.com/cg-tuwien/e186/blob/master/visual_studio/example/main.cpp) may serve as a starting point. Implementation-wise, _e186_ is organized into scenes, two of which can be found at:
* [src/ModelExplorerScene.cpp](https://github.com/cg-tuwien/e186/blob/master/src/ModelExplorerScene.cpp) ... a scene which loads a model and renders it.
* [src/SampleSceneTessIco.cpp](https://github.com/cg-tuwien/e186/blob/master/src/SampleSceneTessIco.cpp) ... a scene wich loads a icosahedron model and renders it using tessellation shaders.      
Please refer to these scene implementation files in order to learn how to use _e186_.

# Credits and Copyright Notices

Developed at TU Wien's Rendering and Modeling Group, Institute of Visual Computing & Human-Centered Technology.

**ASSIMP**

The following files are part of ASSIMP, Copyright (c) assimp team, which is released as Open Source under the terms of a 3-clause BSD license:   
 * all files under `external/include/assimp`
 * `external/lib/release/assimp-vc140.lib`
 * `external/lib/debug/assimp-vc140.lib`
 * `external/bin/release/assimp-vc140-Release.dll`
 * `external/bin/debug/assimp-vc140-Release.dll`     
 
See `external/include/assimp/Importer.hpp` or http://assimp.sourceforge.net/main_license.html for full license details.

**GLFW**

The following files are part of GLFW, Copyright (c) Marcus Geelnard and Camilla Löwy, licensed under the zlib/libpng license:   
 * all files under `external/include/GLFW`
 * `external/lib/release/glfw3.lib`
 * `external/lib/debug/glfw3.lib`
 
See `external/include/GLFW/glfw3.h` or http://www.glfw.org/license.html for full license details.

**GLAD**

The following files are part of GLAD, Copyright (c) David Herberth, licensed under the MIT license:   
 * all files under `external/include/glad`
 * `external/src/glad.cpp`
 
See `external/include/glad/glad.h` or https://github.com/Dav1dde/glad/blob/master/LICENSE for full license details.

**GLM**

The following files are part of GLM, Copyright (c) G-Truc Creation, licensed under The Happy Bunny License and MIT License:
 * all files under `external/include/glm`
 
See `external/include/glm/glm.hpp` or http://glm.g-truc.net/copying.txt for full license details.

**AntTweakBar**

The following files are is part of AntTweakBar, Copyright (c) Philippe Decaudin, licensed under the zlib/libpng license:     
 * `external/include/AntTweakBar.h` 
 * `external/lib/release/AntTweakBar64.lib`
 * `external/lib/debug/AntTweakBar64.lib`
 * `external/bin/release/AntTweakBar64.dll`
 * `external/bin/debug/AntTweakBar64.dll`
 
See `external/include/AntTweakBar.h` or http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:license for full license details.

**stb_image**

The following files are part of stb_image, Copyright (c) by Sean Barrett, licensed under the MIT License and released into the Public Domain.     
 * `external/include/stb_image.h`
 * `external/lib/release/stb_image.lib`
 * `external/lib/debug/stb_image.lib`

See `external/include/stb_image.h` or https://github.com/nothings/stb for full license details.

**FileWatcher**

The following files are part of FileWatcher, Copyright (c) by James Wynn, licensed under the MIT License.     
 * all files under `external/include/FileWatcher`
 * all files under `external/src/FileWatcher`
 
See `external/include/FileWatcher/FileWatcher.h` or https://code.google.com/archive/p/simplefilewatcher for full license details.

**3D Models and Textures under Stanford Scan**

The following 3D models and textures are released by Stanford University      
 * all files under `assets/models/stanford_bunny`
 
See http://www.graphics.stanford.edu/data/3Dscanrep/ for full license details.

**3D Models and Textures under CC BY 3.0**

The following 3D models and textures are released under the Attribution 3.0 Unported (CC BY 3.0) License      
 * all files under `assets/models/cornell_box`
 * all files under `assets/models/fireplace_room`
 * all files under `assets/models/sponza`
 
See https://creativecommons.org/licenses/by/3.0/ for full license details.

**3D Models and Textures under CC BY-NC 2.0 FR**

 * all files under `assets/textures/large_metal_debris`
 * all files under `assets/textures/rough_cobblestones`
 * all files under `assets/textures/small_metal_debris`

See https://creativecommons.org/licenses/by-nc/2.0/fr/deed.en for full license details.

**3D Models and Textures under CC0 1.0**

The following 3D models and textures are released under CC0 1.0 Universal (CC0 1.0), Public Domain Dedication
 * all files under `assets/textures/Scifi_Hex_Wall`
 * all files under `assets/textures/cobblestone`
 
See https://creativecommons.org/publicdomain/zero/1.0/ for full license details.
