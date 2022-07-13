//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}

Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, FitSize);
	if (!ret) {
		throw std::exception();
	}
		
}

Model::~Model()
{
	delete pMeshes;
	delete pMaterials;
	pMeshes = nullptr;
	pMaterials = nullptr;
	deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
	if (!pNode)
		return;
	for (unsigned int i = 0; i < pNode->ChildCount; ++i)
		deleteNodes(&(pNode->Children[i]));
	if (pNode->ChildCount > 0)
		delete[] pNode->Children;
	if (pNode->MeshCount > 0)
		delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

	if (pScene == NULL || pScene->mNumMeshes <= 0)
		return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, FitSize);
	loadMaterials(pScene);
	loadNodes(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
	calcBoundingBox(pScene, BoundingBox);
	MeshCount = pScene->mNumMeshes;
	pMeshes = new Mesh[MeshCount];

	for (size_t i = 0; i < MeshCount; i++) {

	
		aiMesh* aiMesh = pScene->mMeshes[i];
		Mesh& mesh = this->pMeshes[i];
		mesh.MaterialIdx = aiMesh->mMaterialIndex;

		
		mesh.VB.begin();
		int vertexCount = aiMesh->mNumVertices;
		for (size_t j = 0; j < vertexCount; j++) {

			if (aiMesh->HasNormals()) {
				aiVector3D normal = aiMesh->mNormals[j];
				mesh.VB.addNormal(normal.x, normal.y, normal.z);
			}

			if (aiMesh->HasTextureCoords(0)) {
				aiVector3D tcoor = aiMesh->mTextureCoords[0][j];
				mesh.VB.addTexcoord0(tcoor.x, -tcoor.y); 
			}

			if (aiMesh->HasPositions()) {
				aiVector3D vector = aiMesh->mVertices[j];
				mesh.VB.addVertex(vector.x, vector.y, vector.z);
			}
		}
		mesh.VB.end();

		
		mesh.IB.begin();
		int faceCount = aiMesh->mNumFaces;
		for (size_t j = 0; j < faceCount; j++) {
			aiFace face = aiMesh->mFaces[j];
			int indicesOfFace = face.mNumIndices;
			if (indicesOfFace < 3) continue;

			for (size_t k = 0; k < indicesOfFace - 2; k++) {

				mesh.IB.addIndex(face.mIndices[0]);
				mesh.IB.addIndex(face.mIndices[k + 1]);
				mesh.IB.addIndex(face.mIndices[k + 2]);
			}
		}
		mesh.IB.end();

		pMeshes[i] = mesh;

		if (FitSize == true) {
			Matrix m;
			m.scale(5);
			this->transform(m);
		}
	}
}

void Model::loadMaterials(const aiScene* pScene) //ist super
{

	if (pScene->HasMaterials()) {
		MaterialCount = pScene->mNumMaterials;
		pMaterials = new Material[MaterialCount];

		for (size_t i = 0; i < MaterialCount; i++) {
			aiMaterial* mtrl = pScene->mMaterials[i];
			Material material;

			
			aiColor3D color;
			mtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			material.DiffColor = Color(color.r, color.g, color.b);

			mtrl->Get(AI_MATKEY_COLOR_SPECULAR, color);
			material.SpecColor = Color(color.r, color.g, color.b);

			mtrl->Get(AI_MATKEY_COLOR_AMBIENT, color);
			material.AmbColor = Color(color.r, color.g, color.b);


			aiString einString;
			mtrl->GetTexture(aiTextureType_DIFFUSE, 0, &einString);
			std::string p = Path + einString.data;
			material.DiffTex = Texture().LoadShared(p.c_str());

			pMaterials[i] = material;
		}
	}
}

void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
	float minX, minY, minZ, maxX, maxY, maxZ;
	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = FLT_MIN;

	
	MeshCount = pScene->mNumMeshes;
	for (size_t i = 0; i < MeshCount; i++)
	{
		for (size_t j = 0; j < pScene->mMeshes[i]->mNumVertices; j++)
		{
			aiVector3D v = pScene->mMeshes[i]->mVertices[j];
			if (minX > v.x) minX = v.x;
			if (minY > v.y) minY = v.y;
			if (minZ > v.z) minZ = v.z;
			if (maxX < v.x) maxX = v.x;
			if (maxY < v.y) maxY = v.y;
			if (maxZ < v.z) maxZ = v.z;
		}
	}


	Box = AABB(minX, minY, minZ, maxX, maxY, maxZ);
}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convert(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0)
		return;

	pNode->ChildCount = paiNode->mNumChildren;
	pNode->Children = new Node[pNode->ChildCount];
	for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
	{
		copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
		pNode->Children[i].Parent = pNode;
	}
}

void Model::applyMaterial(unsigned int index)
{
	if (index >= MaterialCount)
		return;

	PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
	if (!pPhong) {
		std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
		return;
	}

	Material* pMat = &pMaterials[index];
	pPhong->ambientColor(pMat->AmbColor);
	pPhong->diffuseColor(pMat->DiffColor);
	pPhong->specularExp(pMat->SpecExp);
	pPhong->specularColor(pMat->SpecColor);
	pPhong->diffuseTexture(pMat->DiffTex);
}

void Model::draw(const BaseCamera& Cam)
{
	if (!pShader) {
		std::cout << "BaseModel::draw() no shader found" << std::endl;
		return;
	}
	pShader->modelTransform(transform());

	std::list<Node*> DrawNodes;
	DrawNodes.push_back(&RootNode);

	while (!DrawNodes.empty())
	{
		Node* pNode = DrawNodes.front();
		Matrix GlobalTransform;

		if (pNode->Parent != NULL)
			pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
		else
			pNode->GlobalTrans = transform() * pNode->Trans;

		pShader->modelTransform(pNode->GlobalTrans);

		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
		{
			Mesh& mesh = pMeshes[pNode->Meshes[i]];
			mesh.VB.activate();
			mesh.IB.activate();
			applyMaterial(mesh.MaterialIdx);
			pShader->activate(Cam);
			glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
			mesh.IB.deactivate();
			mesh.VB.deactivate();
		}
		for (unsigned int i = 0; i < pNode->ChildCount; ++i)
			DrawNodes.push_back(&(pNode->Children[i]));

		DrawNodes.pop_front();
	}
}

Matrix Model::convert(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}



