# Skeletal and Character Animation - COSC422 Advanced Computer Graphics

This repository contains a skeletal and character animation using OpenGL and Assimp libraries. See the report in the root directory for more context.

## Dependencies

- OpenGL
- FreeGLUT
- Assimp
- DevIL

## How to run

### Skeletal Animation
`cd SkeletalAnimation`
`g++ -o SkeletalAnimation SkeletalAnimation.cpp -lGL -lGLU -lglut -lassimp && ./SkeletalAnimation`

### Character Animation

`cd CharAnimation`
`g++ CharAnimation.cpp -o CharAnimation -lGL -lGLU -lglut -lassimp -lIL -lILU -lILUT && ./CharAnimation`
