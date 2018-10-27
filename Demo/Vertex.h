#pragma once

enum VertexAttribute {
	 Attribute_Position		= 1 << 0
	,Attribute_Diffuse		= 1 << 1
	,Attribute_Normal		= 1 << 2
	,Attribute_Tangent		= 1 << 3
	,Attribute_Bitangent	= 1 << 4
	,Attribute_TexCoord0	= 1 << 5
	,Attribute_TexCoord1	= 1 << 6
	,Attribute_TexCoord2	= 1 << 7
	,Attribute_TexCoord3	= 1 << 8
};

typedef float float3[3];
typedef float float2[2];
typedef unsigned int uint;

#define VertexMember(_type,_name)	_type	_name;\
typedef _type _name##_##_type;\

//dummy Vertex definition
template<int> struct Vertex {};

template<> struct Vertex<Attribute_Position>{
	VertexMember(float3, position);
}; typedef Vertex<Attribute_Position> VertexPos;

template<> struct Vertex<Attribute_Position | Attribute_TexCoord0> {
	VertexMember(float3, position);
	VertexMember(float2, texcoord0);
}; typedef Vertex<Attribute_Position | Attribute_TexCoord0> VertexPosUV0;

template<> struct Vertex<Attribute_Position | Attribute_Diffuse> {
	VertexMember(float3, position);
	VertexMember(uint, diffuse);
}; typedef Vertex<Attribute_Position | Attribute_Diffuse> VertexPosDiffuse;

template<> struct Vertex<Attribute_Position | Attribute_Normal> {
	VertexMember(float3, position);
	VertexMember(float3, normal);
}; typedef Vertex<Attribute_Position | Attribute_Normal> VertexPosNormal;

template<> struct Vertex<Attribute_Position | Attribute_Normal | Attribute_Diffuse> {
	VertexMember(float3, position);
	VertexMember(float3, normal);
	VertexMember(uint, diffuse);
}; typedef Vertex<Attribute_Position | Attribute_Normal | Attribute_Diffuse> VertexPosNormalDiffuse;

template<> struct Vertex<Attribute_Position | Attribute_Normal | Attribute_TexCoord0> {
	VertexMember(float3, position);
	VertexMember(float3, normal);
	VertexMember(float2, texcoord0);
}; typedef Vertex<Attribute_Position | Attribute_Normal | Attribute_TexCoord0> VertexPosNormalUv;

template<> struct Vertex<Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0> {
	VertexMember(float3, position);
	VertexMember(float3, normal);
	VertexMember(float3, tangent);
	VertexMember(float3, bitangent);
	VertexMember(float2, texcoord0);
}; typedef Vertex<Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0> VertexPosNormalTangentBitangentUv;

#undef VertexMember

struct VertexBuffer {
	void*						vertexes;
	unsigned int				vertexSize;
	unsigned int				vertexType;
	unsigned int				numVertexes;
	unsigned int				vertexBufferId;

	VertexBuffer() {
		vertexes = NULL;
		vertexSize = 0;
		vertexType = 0;
		numVertexes = 0;
		vertexBufferId = 0;
	}
};

struct FaceBuffer {
	unsigned int				numIndexes;
	unsigned int				indexesPerFace;
	unsigned int*				indexes;
	unsigned int				faceBufferId;

	FaceBuffer() {
		numIndexes = 0;
		indexesPerFace = 0;
		indexes = NULL;
		faceBufferId = 0;
	}
};