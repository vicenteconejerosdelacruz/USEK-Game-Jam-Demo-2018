#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "Vertex.h"

typedef void(*vertexBufferCreationFunction)(aiMesh* mesh, VertexBuffer& vb);
typedef std::map<unsigned int, vertexBufferCreationFunction> vertexbufferCreatorMap;

typedef void(*vertexBufferDrawerFunction)(VertexBuffer& vertexbuffer, FaceBuffer& facebuffer, unsigned int textureId);
typedef std::map<unsigned int, vertexBufferDrawerFunction> vertexbufferDrawerMap;
