#include "visualizer.h"

void Visualizer::drawPoints(QVector<QVector3D> & vertices, QVector<QVector4D> & colors)
{
  _shaderProgram.bind();
  _shaderProgram.setUniformValue("mvpMatrix", _T);
  _shaderProgram.setAttributeArray("vertex", vertices.constData(), 0);
  _shaderProgram.enableAttributeArray("vertex");
  _shaderProgram.setAttributeArray("color", colors.constData(), 0);
  _shaderProgram.enableAttributeArray("color");
  glDrawArrays( GL_POINTS, 0, vertices.size() );
  _shaderProgram.disableAttributeArray("vertex");
  _shaderProgram.disableAttributeArray("color");
  _shaderProgram.release();

}

void Visualizer::drawLinestrip(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit)
{

}

void Visualizer::drawTriangles(QVector<QVector3D> & vertices, QVector<QVector4D> & colors) // triangle has always 3 points per 1 unit
{

}

void Visualizer::drawTriangleFan(QVector<QVector3D> & vertices, QVector<QVector4D> & colors, int point_num_of_one_unit)
{

}

void Visualizer::setLineWidth(float width)
{

}
