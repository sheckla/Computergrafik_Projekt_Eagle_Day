/*
 * Camera & Subsclasses
 *
 * Weiterentwickelt auf Basis vom Praktikum:
 *      Created by Philipp Lensing on 16.09.16.
 *      Copyright © 2016 Philipp Lensing. All rights reserved.
 */

#ifndef Camera_h
#define Camera_h

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "vector.h"
#include "matrix.h"

class BaseCamera
{
public:
    virtual void update() = 0;
    virtual const Matrix& getViewMatrix() const = 0;
    virtual const Matrix& getProjectionMatrix() const = 0;
    virtual Vector position() const = 0;
    virtual ~BaseCamera() {};
};

class SimpleCamera : public BaseCamera
{
protected:
    Matrix View;
    Matrix Proj;

public:
    virtual void update() {}
    virtual const Matrix& getViewMatrix() const { return View; }
    virtual const Matrix& getProjectionMatrix() const { return Proj; }
    virtual Vector position() const { Matrix m = View; m.invert(); return m.translation(); }
    void setViewMatrix(const Matrix& m) { View = m; }
    void setProjectionMatrix(const Matrix& m) { Proj = m; }
    virtual ~SimpleCamera() {};
};

class Camera : public BaseCamera
{
protected:
    void updateMouseInput();

    void pan(float dx, float dy);
    void rotate(float x, float y);
    Vector getVSpherePos(float x, float y);
    Vector rotateAxisAngle(Vector v, Vector n, float a);

    Matrix m_ViewMatrix;
    Matrix m_ProjMatrix;
    Vector m_Position;
    Vector m_Target;
    Vector m_Up;
    Vector m_Panning;
    Vector m_Zoom;
    Vector m_Rotation;
    int m_LastMouseX;
    int m_LastMouseY;
    int WindowWidth;
    int WindowHeight;
    GLFWwindow* pWindow;

public:
    Camera(GLFWwindow* pWin);
    virtual ~Camera() {};

    virtual Vector position() const;
    Vector target() const;
    Vector up() const;

    void setPosition(const Vector& Pos);
    void setTarget(const Vector& Target);
    void setUp(const Vector& Up);

    void mouseInput(int x, int y, int Button, int State);

    virtual void update();
    virtual const Matrix& getViewMatrix() const;
    virtual const Matrix& getProjectionMatrix() const;
    void zoom(float dz);
};


#endif 
