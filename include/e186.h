#pragma once

#include <vector>
#include <string>
#include <exception>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <ostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <variant>
#include <iomanip>

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/anim.h>

#include <AntTweakBar.h>

#include "IScene.h"
#include "Stopwatch.h"
#include "Context.h"
#include "VertexAttribData.h"
#include "Engine.h"
#include "TexInfo.h"
#include "TexData.h"
#include "Transform.h"
#include "Camera.h"
#include "ExceptionWithCallstack.h"
#include "log.h"
#include "VertexAttribLocation.h"
#include "AutoMatrix.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Shader.h"
#include "MaterialData.h"
#include "Model.h"