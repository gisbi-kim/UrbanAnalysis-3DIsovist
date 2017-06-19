
#ifndef Visualizer_H
#define Visualizer_H

#include <stdio.h>
#include <fstream>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>

class Visualizer
{
public :
    void initializeCamera(QMatrix4x4 _T, QMatrix4x4 _pMatrix, QMatrix4x4 _mMatrix, QMatrix4x4 _vMatrix, QVector3D _camPosition, QVector3D _camFront, QVector3D _camUpDirection);
    void initShaders(void);
    void drawPoints(QVector<QVector3D> & vertices, QVector<QVector4D> & colors);
    void drawLinestrip(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit);
    void drawTriangles(QVector<QVector3D> & vertices_side1, QVector<QVector4D> & colors_side1, QVector<QVector3D> & vertices_side2, QVector<QVector4D> & colors_side2);
    void drawTriangleFan(QVector<QVector3D> & vertices_side1, QVector<QVector4D> & colors_side1, QVector<QVector3D> & vertices_side2, QVector<QVector4D> & colors_side2, int point_num_of_one_unit);
    void drawQuadstrip(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit);

    void setLineWidth(float width);

private :

    // Own shader
    QOpenGLShaderProgram _shaderProgram;

    // parameters of camera motion
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _mMatrix;
    QMatrix4x4 _vMatrix;
    QVector3D  _camPosition;
    QVector3D  _camFront;
    QVector3D  _camUpDirection;
    QMatrix4x4 _T; // Transformation matrix

    // flags
    bool VISUALIZE_OPENHEMISPHERE = true;
    bool VISUALIZE_FLOOR_ONLY = false;
    bool VISUALIZE_NONFLOOR_ONLY = true;
    bool VISUALIZE_ALL = true;
    bool VISUALIZE_TRIANGLE = true;

};

#endif
