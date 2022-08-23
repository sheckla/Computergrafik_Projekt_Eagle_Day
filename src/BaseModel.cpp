
#include "BaseModel.h"

BaseModel::BaseModel() : pShader(NULL), DeleteShader(false), ShadowCaster(true)
{
    Transform.identity();
}

BaseModel::~BaseModel()
{
    if (DeleteShader && pShader)
        delete pShader;
    DeleteShader = false;
    pShader = NULL;
}

void BaseModel::shader(BaseShader* shader, bool deleteOnDestruction)
{
    pShader = shader;
    DeleteShader = deleteOnDestruction;
}

void BaseModel::draw(const BaseCamera& Cam)
{
    if (!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }

    pShader->modelTransform(transform());
    pShader->activate(Cam);

}

