/*
 * BaseModel
 * - Dient als Basisklasse fuer alle weiteren Ableitungen dieser
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef BaseModel_h
#define BaseModel_h

#include "camera.h"
#include "matrix.h"
#include "baseshader.h"
#include "Aabb.h"

class BaseModel
{
protected:
    Matrix Transform;
    BaseShader* pShader;
    bool DeleteShader;
    bool ShadowCaster;

public:
    BaseModel();
    virtual ~BaseModel();
    virtual void draw(const BaseCamera& Cam);
    const Matrix& transform() const { return Transform; }
    void transform(const Matrix& m) { Transform = m; }
    virtual void shader(BaseShader* shader, bool deleteOnDestruction = false);
    virtual BaseShader* shader() const { return pShader; }
    virtual const AABB& boundingBox() const { return AABB::unitBox(); }
    bool shadowCaster() const { return ShadowCaster; }
    void shadowCaster(bool sc) { ShadowCaster = sc; }
};


#endif 
