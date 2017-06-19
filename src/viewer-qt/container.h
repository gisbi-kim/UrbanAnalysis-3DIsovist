#ifndef Container_H
#define Container_H

#include <stdio.h>
#include <fstream>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>

class PointsContainer
{
public :
    void ClearHemisphere(void);

public :
    ///// Original Raw points /////
    QVector<QVector3D> las_vertices;
    QVector<QVector4D> las_colors;

    QVector<QVector3D> floor_vertices;
    QVector<QVector4D> floor_colors;
    QVector<QVector3D> nonfloor_vertices;
    QVector<QVector4D> nonfloor_colors;
    ///// end of Original Raw points /////

    ///// Closed Triangle /////
    QVector<QVector3D> cur_traiangle_vertices;
    QVector<QVector4D> cur_traiangle_colors;
    QVector<QVector3D> cur_traiangle_otherside_vertices;
    QVector<QVector4D> cur_traiangle_otherside_colors;
    QVector<QVector3D> cur_traiangle_teduri_vertices;
    QVector<QVector4D> cur_traiangle_teduri_colors;
    ///// end of Closed Triangle /////

    ///// Edges related to Open Hemispher /////
    //Trace of "Sight Limit Point"
    QVector<QVector3D> point_P_debug;
    QVector<QVector4D> point_P_color_debug;
    QVector<QVector3D> point_Pp_debug;
    QVector<QVector4D> point_Pp_color_debug;
    //Edges of "Open Hemisphere"
    QVector<QVector3D> CP_debug;
    QVector<QVector4D> CP_color_debug;
    ///// end of Edges related to Open Hemispher /////

    ///// Main Open Hemisphere /////
    // left side
    QVector<QVector3D> openpieces_left_plane_side1;
    QVector<QVector4D> openpieces_left_plane_side1_color;
    QVector<QVector3D> openpieces_left_plane_side2;
    QVector<QVector4D> openpieces_left_plane_side2_color;
    QVector<QVector3D> openpieces_left_edge;
    QVector<QVector4D> openpieces_left_edge_color;
    // right side
    QVector<QVector3D> openpieces_right_plane_side1;
    QVector<QVector4D> openpieces_right_plane_side1_color;
    QVector<QVector3D> openpieces_right_plane_side2;
    QVector<QVector4D> openpieces_right_plane_side2_color;
    QVector<QVector3D> openpieces_right_edge;
    QVector<QVector4D> openpieces_right_edge_color;
    // top side
    QVector<QVector3D> openpieces_top_plane_side1;
    QVector<QVector4D> openpieces_top_plane_side1_color;
    QVector<QVector3D> openpieces_top_plane_side2;
    QVector<QVector4D> openpieces_top_plane_side2_color;
    QVector<QVector3D> openpieces_top_edge;
    QVector<QVector4D> openpieces_top_edge_color;
    // bottome side
    QVector<QVector3D> openpieces_bottom_plane_side1;
    QVector<QVector4D> openpieces_bottom_plane_side1_color;
    QVector<QVector3D> openpieces_bottom_plane_side2;
    QVector<QVector4D> openpieces_bottom_plane_side2_color;
    QVector<QVector3D> openpieces_bottom_edge;
    QVector<QVector4D> openpieces_bottom_edge_color;
    ///// end of Main Open Hemisphere /////

    //Observer information
    int cur_observer_index;
    QVector3D cur_observer;


};

#endif
