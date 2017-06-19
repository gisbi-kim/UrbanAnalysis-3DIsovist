#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <stdio.h>
#include <fstream>

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <liblas/liblas.hpp>
#include <liblas/reader.hpp>

#include "visualizer.h"
#include "las_reader.h"
#include "container.h"


class OpenglWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit OpenglWidget(QWidget *parent = 0);
    ~OpenglWidget();

    void LAS_read(char* name);
    int GetNumOfRoad (liblas::Reader _reader, float _onefloorheight);
    float AdjustObserverZ();

    void MakeTriangle(QVector3D _cur_observer);
    void MakeOpenHemisphere(QVector3D _cur_ob, int _num_piece, int _r_visible);

    void ClearHemisphere(void);

protected:
    // Opengl Default Drawer, mouse and key event
    void initializeGL();
    void initShaders();

    void resizeGL(int width, int height);
    void paintGL();
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void draw(void);


private:

    LasReader las_reader;
    PointsContainer points_container;
    Visualizer visualizer;


    QPoint _lastMousePosition;

    // shader and data
    QOpenGLShaderProgram _shaderProgram;
//    QVector<QVector3D> vertices;
//    QVector<QVector4D> colors;


    // Transformation matrix
    QMatrix4x4 _T;

    // camera motion
    QMatrix4x4 _pMatrix;
    QMatrix4x4 _mMatrix;
    QMatrix4x4 _vMatrix;

    // for lookat
    QVector3D _camPosition;
    QVector3D _camFront;
    QVector3D _camUpDirection;

    QVector3D _rot;

    QVector<QVector3D> las_vertices;
    QVector<QVector4D> las_colors;

    QVector<QVector3D> floor_vertices;
    QVector<QVector4D> floor_colors;
    QVector<QVector3D> nonfloor_vertices;
    QVector<QVector4D> nonfloor_colors;

    QVector<QVector3D> cur_traiangle_vertices;
    QVector<QVector4D> cur_traiangle_colors;
    QVector<QVector3D> cur_traiangle_otherside_vertices;
    QVector<QVector4D> cur_traiangle_otherside_colors;
    QVector<QVector3D> cur_traiangle_teduri_vertices;
    QVector<QVector4D> cur_traiangle_teduri_colors;

//========================================================

    QVector<QVector3D> representatives;
    int num_pieces = 360; // 360/N degree per section
    float one_piece_angle = 360.f/num_pieces;
    int num_interpolation = 20;
    void MakeGlobalHemisphere(QVector3D _cur_observer);
    float getBtnAngle(QVector3D _point1, QVector3D _point2);

    static bool cmp (QVector3D _point1, QVector3D _point2);
    QVector3D MakeRepresentative(QVector<QVector3D> & _points_in_piece);


    QVector3D cur_observer;
    //QVector3D cur_ceiling;

    int cur_observer_index;

    float GetRfromXY(float _x, float _y);
    float GetThetafromXY(float _x, float _y);

    /*
     * # One piece of the hemisphere is composed of 4 planes(at each has 2 sides and edge)
     */
    // plane 1
    QVector<QVector3D> openpieces_left_plane_side1;
    QVector<QVector4D> openpieces_left_plane_side1_color;
    QVector<QVector3D> openpieces_left_plane_side2;
    QVector<QVector4D> openpieces_left_plane_side2_color;
    QVector<QVector3D> openpieces_left_edge;
    QVector<QVector4D> openpieces_left_edge_color;

    // plane 2
    QVector<QVector3D> openpieces_right_plane_side1;
    QVector<QVector4D> openpieces_right_plane_side1_color;
    QVector<QVector3D> openpieces_right_plane_side2;
    QVector<QVector4D> openpieces_right_plane_side2_color;
    QVector<QVector3D> openpieces_right_edge;
    QVector<QVector4D> openpieces_right_edge_color;

    // plane 3
    QVector<QVector3D> openpieces_top_plane_side1;
    QVector<QVector4D> openpieces_top_plane_side1_color;
    QVector<QVector3D> openpieces_top_plane_side2;
    QVector<QVector4D> openpieces_top_plane_side2_color;
    QVector<QVector3D> openpieces_top_edge;
    QVector<QVector4D> openpieces_top_edge_color;

    // plane 4
    QVector<QVector3D> openpieces_bottom_plane_side1;
    QVector<QVector4D> openpieces_bottom_plane_side1_color;
    QVector<QVector3D> openpieces_bottom_plane_side2;
    QVector<QVector4D> openpieces_bottom_plane_side2_color;
    QVector<QVector3D> openpieces_bottom_edge;
    QVector<QVector4D> openpieces_bottom_edge_color;

    QVector<QVector3D> point_P_debug;
    QVector<QVector4D> point_P_color_debug;

    QVector<QVector3D> point_Pp_debug;
    QVector<QVector4D> point_Pp_color_debug;

    QVector<QVector3D> CP_debug;
    QVector<QVector4D> CP_color_debug;


//========================================================
    float moving_gamdo = 3;
    float visible_region_radius = 40.f;

    float zmin;
    float floor_boundary;

    bool IsFirstTime = true;
    double offset_x = 0;
    double offset_y = 0;
    double offset_z = 0;


    const float one_floor_height_for_extract_road = 0.2;
    const int num_floor = 50;

    int las_read_skip_num;
    float a_count;

    std::ofstream log;

	bool VISUALIZE_OPENHEMISPHERE = true;
    bool VISUALIZE_FLOOR_ONLY = false;
    bool VISUALIZE_NONFLOOR_ONLY = true;
    bool VISUALIZE_ALL = true;
    bool VISUALIZE_TRIANGLE = true;


    QVector<QVector3D> testtrianglestrip;
    QVector<QVector4D> testtrianglestrip_colors;

    bool toggel_onoff_hemisphere;

signals:

public slots:
};

#endif // OPENGLWIDGET_H
