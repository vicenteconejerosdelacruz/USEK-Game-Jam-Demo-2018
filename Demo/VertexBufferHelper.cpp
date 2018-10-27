#include "stdafx.h"
#include "VertexBufferHelper.h"
#include <gl/glew.h>

vertexbufferCreatorMap vertexBufferCreator;
vertexbufferDrawerMap vertexBufferDrawer;

template<unsigned int _type1, unsigned int _type2>
struct CopyVertex { static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index) {}; };

template<unsigned int _type2>
struct CopyVertex<Attribute_Position, _type2> {
	static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index) {
		v.position[0] = mesh->mVertices[index][0];
		v.position[1] = mesh->mVertices[index][1];
		v.position[2] = mesh->mVertices[index][2];
	}
};

template<unsigned int _type2>
struct CopyVertex<Attribute_Normal, _type2> {
	static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index) {
		v.normal[0] = mesh->mNormals[index][0];
		v.normal[1] = mesh->mNormals[index][1];
		v.normal[2] = mesh->mNormals[index][2];
	}
};

template<unsigned int _type2>
struct CopyVertex<Attribute_Tangent, _type2> {
	static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index) {
		v.tangent[0] = mesh->mTangents[index][0];
		v.tangent[1] = mesh->mTangents[index][1];
		v.tangent[2] = mesh->mTangents[index][2];
	}
};

template<unsigned int _type2>
struct CopyVertex<Attribute_Bitangent, _type2> {
	static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index) {
		v.bitangent[0] = mesh->mBitangents[index][0];
		v.bitangent[1] = mesh->mBitangents[index][1];
		v.bitangent[2] = mesh->mBitangents[index][2];
	}
};

template<unsigned int _type2>
struct CopyVertex<Attribute_TexCoord0, _type2> {
	static void copy(Vertex<_type2>& v, aiMesh* mesh, unsigned int index)	{
		v.texcoord0[0] = mesh->mTextureCoords[0][index][0];
		v.texcoord0[1] = mesh->mTextureCoords[0][index][1];
	}
};

template<unsigned int _type>
static void CreateVertexBuffer(aiMesh* mesh, VertexBuffer& vertexbuffer ) {

	vertexbuffer.vertexes = new Vertex<_type>[mesh->mNumVertices];
	vertexbuffer.numVertexes = mesh->mNumVertices;
	vertexbuffer.vertexSize = sizeof(Vertex<_type>);
	vertexbuffer.vertexType = _type;

	Vertex<_type> *vertex = (Vertex<_type>*)vertexbuffer.vertexes;

	for (unsigned int i = 0; i < vertexbuffer.numVertexes; i++) {
		CopyVertex<_type&Attribute_Position	, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_Diffuse	, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_Normal	, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_Tangent	, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_Bitangent, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_TexCoord0, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_TexCoord1, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_TexCoord2, _type	>::copy(*vertex, mesh, i);
		CopyVertex<_type&Attribute_TexCoord3, _type	>::copy(*vertex, mesh, i);
		vertex++;
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glGenBuffers(1, &vertexbuffer.vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer.vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex<_type>) * vertexbuffer.numVertexes, vertexbuffer.vertexes, GL_STATIC_DRAW);
}

template<unsigned int _type1, unsigned int _type2>
struct DrawVertex { static void draw(VertexBuffer& vertexBuffer) {}; };

template<unsigned int _type2>
struct DrawVertex<Attribute_Position, _type2> {
	static void draw(VertexBuffer& vertexbuffer) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<_type2>), (((Vertex<_type2>*)NULL)->position));
	}
};

template<unsigned int _type2>
struct DrawVertex<Attribute_TexCoord0, _type2> {
	static void draw(VertexBuffer& vertexbuffer) {
		glActiveTexture(GL_TEXTURE0);
		glEnableVertexAttribArray(1);
		glTexCoordPointer(1, GL_FLOAT, sizeof(Vertex<_type2>), (((Vertex<_type2>*)NULL)->texcoord0));
	}
};

template<unsigned int _type>
static void DrawVertexBuffer(VertexBuffer& vertexbuffer, FaceBuffer& facebuffer, unsigned int texture) {
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer.vertexBufferId);

	DrawVertex<_type&Attribute_Position, _type	>::draw(vertexbuffer);
	DrawVertex<_type&Attribute_TexCoord0, _type	>::draw(vertexbuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facebuffer.faceBufferId);
	glDrawElements(GL_TRIANGLES, facebuffer.numIndexes, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void initVertexBufferHelpers() {
	//this stuff are basically jump tables
	vertexBufferCreator[Attribute_Position] = CreateVertexBuffer<Attribute_Position>;
	vertexBufferCreator[Attribute_Position | Attribute_TexCoord0] = CreateVertexBuffer<Attribute_Position | Attribute_TexCoord0>;
	vertexBufferCreator[Attribute_Position | Attribute_Diffuse] = CreateVertexBuffer<Attribute_Position | Attribute_Diffuse>;
	vertexBufferCreator[Attribute_Position | Attribute_Normal] = CreateVertexBuffer<Attribute_Position | Attribute_Normal>;
	vertexBufferCreator[Attribute_Position | Attribute_Normal | Attribute_Diffuse] = CreateVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_Diffuse>;
	vertexBufferCreator[Attribute_Position | Attribute_Normal | Attribute_TexCoord0] = CreateVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_TexCoord0>;
	vertexBufferCreator[Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0] = CreateVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0>;

	//same here
	vertexBufferDrawer[Attribute_Position] = DrawVertexBuffer<Attribute_Position>;
	vertexBufferDrawer[Attribute_Position | Attribute_TexCoord0] = DrawVertexBuffer<Attribute_Position | Attribute_TexCoord0>;
	vertexBufferDrawer[Attribute_Position | Attribute_Diffuse] = DrawVertexBuffer<Attribute_Position | Attribute_Diffuse>;
	vertexBufferDrawer[Attribute_Position | Attribute_Normal] = DrawVertexBuffer<Attribute_Position | Attribute_Normal>;
	vertexBufferDrawer[Attribute_Position | Attribute_Normal | Attribute_Diffuse] = DrawVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_Diffuse>;
	vertexBufferDrawer[Attribute_Position | Attribute_Normal | Attribute_TexCoord0] = DrawVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_TexCoord0>;
	vertexBufferDrawer[Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0] = DrawVertexBuffer<Attribute_Position | Attribute_Normal | Attribute_Tangent | Attribute_Bitangent | Attribute_TexCoord0>;

}

void faceBufferCreator(aiMesh* mesh, FaceBuffer& fb){
	if (mesh->HasFaces()) {
		//initialize data
		fb.numIndexes = mesh->mNumFaces*mesh->mFaces[0].mNumIndices;
		fb.indexesPerFace = mesh->mFaces[0].mNumIndices;
		fb.indexes = new unsigned int[fb.numIndexes];
			   
		//store as flat buffer
		unsigned int index = 0;
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++) {
				fb.indexes[index] = mesh->mFaces[j].mIndices[k];
				index++;
			}
		}

		glGenBuffers(1, &fb.faceBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fb.faceBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * fb.numIndexes, fb.indexes, GL_STATIC_DRAW);
	}
}