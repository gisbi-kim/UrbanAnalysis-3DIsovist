#include "visualizer.h"

void Visualizer::initializeCamera(QMatrix4x4 T, QMatrix4x4 pMatrix, QMatrix4x4 mMatrix, QMatrix4x4 vMatrix, QVector3D camPosition, QVector3D camFront, QVector3D camUpDirection)
{
    _T = T;
    _pMatrix = pMatrix;
    _mMatrix = mMatrix;
    _vMatrix = vMatrix;
    _camPosition = camPosition;
    _camFront = camFront;
    _camUpDirection = camUpDirection;
}

void Visualizer::initShaders()
{
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vertexShader.vsh");
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fragmentShader.fsh");
    _shaderProgram.link();
}

void Visualizer::drawPoints(QVector<QVector3D> & vertices, QVector<QVector4D> & colors)
{
    _mMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _vMatrix.lookAt(_camPosition, _camPosition+_camFront, _camUpDirection);
    _T = _pMatrix * _vMatrix * _mMatrix;

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
    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    for(int i=0; i < vertices.size()/(point_num_of_one_unit); i++)
    {
        glDrawArrays(GL_LINE_STRIP, i*(point_num_of_one_unit), point_num_of_one_unit);
    }
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();
}

void Visualizer::drawTriangles(QVector<QVector3D> & vertices_side1, QVector<QVector4D> & colors_side1, QVector<QVector3D> & vertices_side2, QVector<QVector4D> & colors_side2) // triangle has always 3 points per 1 unit
{
    glEnable(GL_BLEND); // ref : http://diehard98.tistory.com/entry/OpenGL-%EB%B8%94%EB%A0%8C%EB%94%A9m, http://collagefactory.blogspot.kr/2009/10/opengl.html
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  );

    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    glDrawArrays( GL_TRIANGLES, 0, vertices_side1.size() );
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();

    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices_side2.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors_side2.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    glDrawArrays( GL_TRIANGLES, 0, vertices_side2.size() );
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();

    glDisable(GL_BLEND);
}

void Visualizer::drawTriangleFan(QVector<QVector3D> & vertices_side1, QVector<QVector4D> & colors_side1, QVector<QVector3D> & vertices_side2, QVector<QVector4D> & colors_side2, int point_num_of_one_unit)
{
    glEnable(GL_BLEND); // ref : http://diehard98.tistory.com/entry/OpenGL-%EB%B8%94%EB%A0%8C%EB%94%A9m, http://collagefactory.blogspot.kr/2009/10/opengl.html
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  );

    //side 1
    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    for(int i=0; i < vertices_side1.size()/(point_num_of_one_unit); i++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i*(point_num_of_one_unit), point_num_of_one_unit);
    }
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();

    //side 2
    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices_side2.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors_side2.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    for(int i=0; i < vertices_side2.size()/(point_num_of_one_unit); i++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i*(point_num_of_one_unit), point_num_of_one_unit);
    }
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();

    glDisable(GL_BLEND);
}

void Visualizer::drawQuadstrip(QVector<QVector3D> & vertices_side1, QVector<QVector4D> & colors_side1, int point_num_of_one_unit)
{
    glEnable(GL_BLEND); // ref : http://diehard98.tistory.com/entry/OpenGL-%EB%B8%94%EB%A0%8C%EB%94%A9m, http://collagefactory.blogspot.kr/2009/10/opengl.html
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  );

    //side 1
    _shaderProgram.bind();
    _shaderProgram.setUniformValue("mvpMatrix", _T);
    _shaderProgram.setAttributeArray("vertex", vertices_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("vertex");
    _shaderProgram.setAttributeArray("color", colors_side1.constData(), 0);
    _shaderProgram.enableAttributeArray("color");
    for(int i=0; i < vertices_side1.size()/(point_num_of_one_unit); i++)
    {
        glDrawArrays(GL_QUAD_STRIP, i*(point_num_of_one_unit), point_num_of_one_unit);
    }
    _shaderProgram.disableAttributeArray("vertex");
    _shaderProgram.disableAttributeArray("color");
    _shaderProgram.release();

    //side 2 (in near future)

    glDisable(GL_BLEND);
}

void Visualizer::setLineWidth(float width)
{
    glLineWidth(width);
}
