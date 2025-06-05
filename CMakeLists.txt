cmake_minimum_required(VERSION 3.10)
project(OpenGLProgram LANGUAGES CXX)

# ──────────────────────────────── Répertoires ────────────────────────────────
set(SRC_DIR       "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR   "${CMAKE_SOURCE_DIR}/include")
set(EXTERNAL_DIR  "${CMAKE_SOURCE_DIR}/external")

set(GLEW_DIR "${EXTERNAL_DIR}/glew-cmake")
set(GLFW_DIR "${EXTERNAL_DIR}/glfw")
set(GLM_DIR  "${EXTERNAL_DIR}/glm")

# ─────────────────────────── Fichiers sources ────────────────────────────────
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/node.cpp
    ${SRC_DIR}/shape.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/viewer.cpp
    ${SRC_DIR}/skybox.cpp
    ${SRC_DIR}/triangle.cpp
    ${SRC_DIR}/pyramid.cpp
    ${SRC_DIR}/cylinder.cpp
    ${SRC_DIR}/sphere.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/textured_sphere.cpp
    ${SRC_DIR}/lighting_sphere.cpp
)

# ──────────────────────────── Cible principale ───────────────────────────────
add_executable(opengl_program ${SOURCES})

target_include_directories(opengl_program PRIVATE
    ${INCLUDE_DIR}
    ${GLEW_DIR}/include
    ${GLFW_DIR}/include
    ${GLM_DIR}
)

# Chemins RELATIFS pour les assets (plus de chemins absolus dans l’EXE)
target_compile_definitions(opengl_program PRIVATE
    SHADER_DIR="shaders/"
    TEXTURE_DIR="textures/"
)

# ─────────────────────── Ajout / lien des bibliothèques ──────────────────────
add_subdirectory(${GLEW_DIR})   # produit libglew_static
add_subdirectory(${GLFW_DIR})   # produit glfw
add_subdirectory(${GLM_DIR})    # header-only, pas de binaire

target_link_libraries(opengl_program PRIVATE
    libglew_static
    glfw
)

# ─────────────────────── Copie auto des assets après build ───────────────────
add_custom_command(TARGET opengl_program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/shaders"   "$<TARGET_FILE_DIR:opengl_program>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/textures"  "$<TARGET_FILE_DIR:opengl_program>/textures"
)

# ─────────────────────────────────────────────────────────────────────────────
#   Après un « Configure » + « Generate », tu obtiendras un exécutable portable :
#   Build/Release/opengl_program.exe
#   │
#   ├── shaders/   (recopié)
#   └── textures/  (recopié)
#
#   Plus aucun chemin absolu n’apparaît dans le binaire ; il fonctionnera donc
#   sur n’importe quel PC dès lors que les DLL tierces sont à côté de l’EXE.
