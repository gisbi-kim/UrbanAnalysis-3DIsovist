
#ifndef _HEDEAR_Visualizer
#define _HEDEAR_Visualizer

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
  void drawPoints(QVector<QVector3D> & vertices, QVector<QVector4D> & colors);
  void drawLinestrip(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit);
  void drawTriangles(QVector<QVector3D> & vertices, QVector<QVector4D> & colors); // triangle has always 3 points per 1 unit
  void drawTriangleFan(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit);
  void setLineWidth(float width);

};

#endif
