#ifndef LasReader_H
#define LasReader_H

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

#include "container.h"

class LasReader
{
public :
    void readLas(char * name, PointsContainer & container);
        //sub-function of readLas
        void printInfo(char *name, liblas::Header const& header);
        void skipPoints(liblas::Reader reader, unsigned int num);
        void updateOrNotZmin(double cur_z);
        void initOffset(liblas::Point const & cur_read_point);

    void LasReader::readLasFiles();
        //sub-functions of reaLasFiles
        void setParentPath(std::string path);


private :

    //re-coordinates with offset(first read point is (0,0,0))
    bool IsFirstTime = true;
    double offset_x = 0;
    double offset_y = 0;
    double offset_z = 0;

    //how many skip
    int num_read_skip = 100;

    //parameters for floor extraction
    int zmin = 100; // initialized with enough large number. It will be changed with minimum value of z value amoung points at the stage of first read
    const float one_floor_height_for_extract_road = 0.3;
    const int num_floor = 30;

    //flags for selective load
    bool On_TrackA = false;
    int num_start_A;
    bool On_TrackB = false;
    bool On_TrackC = false;
    bool On_TrackD = false;
    bool On_TrackE = false;
    bool On_TrackF = false;
    bool On_TrackG = false;
    bool On_TrackL = false;
    bool On_TrackM = false;
    bool On_TrackN = false;
    bool On_TrackP = false;

    //for massive reading
    std::string parent_path;



};

#endif
