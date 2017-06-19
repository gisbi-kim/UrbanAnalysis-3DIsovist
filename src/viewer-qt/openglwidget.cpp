#include "openglwidget.h"

float OpenglWidget::GetRfromXY(float _x, float _y)
{
    // _x : relative x (from observer to point)
    // _y : relative y (from observer to point)
    float R = std::sqrt(_x*_x + _y*_y);
    return R;
}

float OpenglWidget::getBtnAngle(QVector3D _point1, QVector3D _point2)
{
    //using dot product of two vector : ab=|a||b|cos(theta)
    float cos = ( (_point1.x()*_point2.x()) + (_point1.y()*_point2.y()) + (_point1.z()*_point2.z()) ) / ( _point1.length() * _point2.length());

    float rad = std::acos(cos);
    // deg in [0,180]
    return rad;
}

float OpenglWidget::GetThetafromXY(float _x, float _y)
{

    float theta = 0;
    if( _x > 0 && _y > 0) // 1 quad
        theta = (180/M_PI) * std::atan(_y/_x);
    else if ( _x < 0 && _y > 0) // 2 quad
        theta = 180 - ((180/M_PI) * std::atan(_y/(-_x)));
    else if ( _x < 0 && _y < 0) // 3 quad
        theta = 180 + ((180/M_PI) * std::atan(_y/_x));
    else if ( _x > 0 && _y < 0) // 4 quad
        theta = 360 - ((180/M_PI) * std::atan((-_y)/_x));

    //qDebug() << "theta of the point is  :" << theta;
    return theta;
}

bool OpenglWidget::cmp (QVector3D _point1, QVector3D _point2)
{
  return _point1.z() >= _point2.z();
}

QVector3D OpenglWidget::MakeRepresentative(QVector<QVector3D> & _points_in_piece)
{
   // std::sort(_points_in_piece.begin(), _points_in_piece.end(), cmp);

    int N = _points_in_piece.size();

    float x_of_highest_z = 0;
    float y_of_highest_z = 0;
    float highest_z = 0;
    float highest_delta_abs_z = 0;

    //    float sum_x_topN = 0 ;
//    float sum_y_topN = 0 ;
//    float sum_z_topN = 0 ;
    for(auto iter = _points_in_piece.begin() ; iter != _points_in_piece.end() ; iter++)
    {
//        sum_x_topN =+ iter->x();
//        sum_y_topN =+ iter->y();
//        sum_z_topN =+ iter->z();
        if ( highest_delta_abs_z < std::abs(iter->z() - cur_observer.z()) )
        {
            highest_delta_abs_z = std::abs(iter->z() - cur_observer.z());
            x_of_highest_z = iter->x();
            y_of_highest_z = iter->y();
            highest_z = iter->z();
        }
    }

//    float avg_x_topN = sum_x_topN / N;
//    float avg_y_topN = sum_y_topN / N;
//    float avg_z_topN = sum_z_topN / N;

//    return QVector3D(avg_x_topN, avg_y_topN,avg_z_topN);
    return QVector3D(x_of_highest_z , y_of_highest_z ,highest_z);
}


void OpenglWidget::MakeGlobalHemisphere(QVector3D _cur_observer)
{
    ClearHemisphere();

    cur_observer = _cur_observer;

    point_P_debug.clear();
    point_Pp_debug.clear();
    CP_debug.clear();
    CP_color_debug.clear();

    //   for( int i = 0 ; i < 6 ; i++)
    for( int i = 0 ; i < num_pieces ; i++)
    {
        QVector3D ceiling(cur_observer.x(), cur_observer.y(), _cur_observer.z() + visible_region_radius );
        QVector3D point_P;
        QVector3D point_Pp;

        QVector<QVector3D> points_in_piece;

        /* #1
         * Put points in the piece with some conditions
         */

        int index_of_the_piece = i+1;

        for(auto iter = points_container.nonfloor_vertices.begin() ; iter != points_container.nonfloor_vertices.end() ; iter++)
        {
            QVector3D cur_tmp_point = *iter;
            // condition 1 : isInVisibleRegion
            if(cur_observer.distanceToPoint(cur_tmp_point) < visible_region_radius)
            {
                float px = cur_tmp_point.x();
                float py = cur_tmp_point.y();
                float pz = cur_tmp_point.z();
                float relative_x = px - this->cur_observer.x();
                float relative_y = py - this->cur_observer.y();
                float relative_r = GetRfromXY(relative_x, relative_y);
                float relative_theta = GetThetafromXY(relative_x, relative_y);

                // condition 2 : isIntheAngle
                float unit_angle = 360.0/num_pieces;
                if( unit_angle*(index_of_the_piece-1) < relative_theta && relative_theta < unit_angle*(index_of_the_piece) )
                {
                  //save
                  points_in_piece << cur_tmp_point;

//                  qDebug() << i+1 << "th piece at " << unit_angle*(index_of_the_piece-1) << " to " << unit_angle*(index_of_the_piece);
//                  qDebug() << "save the point at degree : " << relative_theta << endl;
                }
            }

        }
//        qDebug() << "num of points in the" << i << "th piece is " << points_in_piece.size();


        /* #2
         * Make a representative(this is point 'P') of the piece by specific rule
         * (defulat rule is highest z vaule)
         */

        float theta_for_P_of_the_piece = (this->one_piece_angle) * (index_of_the_piece);
        float theta_for_Pp_of_the_piece = (this->one_piece_angle) * (index_of_the_piece - 1);

//        qDebug() << i << "th theta is : " <<  theta_for_P_of_the_piece ;

        if( points_in_piece.size() > 20 )
        {
              QVector3D representative = MakeRepresentative(points_in_piece);

//              QVector<float> copy;

//              for( auto iter= points_in_piece.begin(); iter<points_in_piece.end();iter++)
//              {
//                  copy<< iter->z();
//              }


//              qDebug() << "max z of the piece is " << *std::max_element(copy.begin(),copy.end());

            float avg_r = std::sqrt( (representative.x()-cur_observer.x())*(representative.x()-cur_observer.x()) + (representative.y()-cur_observer.y())*(representative.y()-cur_observer.y()));

//            qDebug() << "avg_r is : " << avg_r;
//            qDebug() << "theta_for_P_of_the_piece is : " << theta_for_P_of_the_piece;
//            qDebug() << "std::cos((theta_for_P_of_the_piece/180)*M_PI) is : " << std::cos((theta_for_P_of_the_piece/180)*M_PI);
//            qDebug() << "std::sin((theta_for_P_of_the_piece/180)*M_PI) is : " << std::sin((theta_for_P_of_the_piece/180)*M_PI);

//            qDebug() << "avg_r*std::cos(theta_for_P_of_the_piece) is : " << avg_r*std::cos((theta_for_P_of_the_piece/180)*M_PI); // rad
//            qDebug() << "avg_r*std::sin(theta_for_P_of_the_piece) is : " << avg_r*std::sin((theta_for_P_of_the_piece/180)*M_PI);

//            float curobx = cur_observer.x();
//            float xdelta = avg_r*std::cos((theta_for_P_of_the_piece/180)*M_PI);
//            float Pxtest = curobx + xdelta;
//            qDebug() << "### test curobx is : " << cur_observer.x();
//            qDebug() << "### test xdelta is : " << xdelta;
//            qDebug() << "### test Pxtest is : " << Pxtest;
//            float curoby = cur_observer.y();
//            float ydelta = avg_r*std::sin((theta_for_P_of_the_piece/180)*M_PI);
//            float Pytest = curoby + ydelta;
//            qDebug() << "### test curoby is : " << cur_observer.y();
//            qDebug() << "### test ydelta is : " << ydelta;
//            qDebug() << "### test Pytest is : " << Pytest;


            float Px = this->cur_observer.x() +  avg_r*std::cos((theta_for_P_of_the_piece/180)*M_PI);
            float Py = this->cur_observer.y() +  avg_r*std::sin((theta_for_P_of_the_piece/180)*M_PI);
            float Ppx = this->cur_observer.x() + avg_r*std::cos((theta_for_Pp_of_the_piece/180)*M_PI);
            float Ppy = this->cur_observer.y() + avg_r*std::sin((theta_for_Pp_of_the_piece/180)*M_PI);

            point_P.setX(Px);
            point_P.setY(Py);
            point_P.setZ(representative.z());
//            qDebug() << "cur ob x  is : " << cur_observer.x();
//            qDebug() << "cur ob y is : " << cur_observer.y();
//            qDebug() << "cur_observer.x() + avg_r*std::cos(theta_for_P_of_the_piece) is : " << this->cur_observer.x() + avg_r*std::cos((theta_for_P_of_the_piece/180)*M_PI);
//            qDebug() << "cur_observer.y() + avg_r*std::sin(theta_for_P_of_the_piece) is : " << this->cur_observer.y() + avg_r*std::sin((theta_for_P_of_the_piece/180)*M_PI);
//            qDebug() << "Px is : " << Px;
//            qDebug() << "Py is : " << Py;


            point_Pp.setX(Ppx);
            point_Pp.setY(Ppy);
            point_Pp.setZ(representative.z());


        }
        else
        {
            float avg_r = visible_region_radius;

            float Px = this->cur_observer.x() + avg_r*std::cos((theta_for_P_of_the_piece/180)*M_PI);
            float Py = this->cur_observer.y() + avg_r*std::sin((theta_for_P_of_the_piece/180)*M_PI);
            float Ppx = this->cur_observer.x() + avg_r*std::cos((theta_for_Pp_of_the_piece/180)*M_PI);
            float Ppy = this->cur_observer.y() + avg_r*std::sin((theta_for_Pp_of_the_piece/180)*M_PI);

            point_P.setX(Px);
            point_P.setY(Py);
            point_P.setZ(cur_observer.z());
            point_Pp.setX(Ppx);
            point_Pp.setY(Ppy);
            point_Pp.setZ(cur_observer.z());
        }

        // this is also visualization good
        point_P_debug << point_P;
        point_P_color_debug << QVector4D(1,1,0 , 0.35 );

        point_Pp_debug << point_Pp;
        point_Pp_color_debug << QVector4D(1,1,102/255 , 0.35 );


        /* #3
         * from the P, get Pp and save points into shader array
         */

        // @@@ right @@@ //////////////////////////////////////////////////////////////////////////////////////////
        // point 1
        openpieces_right_plane_side1 << cur_observer;
        //            openpieces_right_plane_side1_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_right_plane_side1_color << QVector4D(0,0,1 , 0.35 );
        // point 2
        openpieces_right_plane_side1 << ceiling;
        //            openpieces_right_plane_side1_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_right_plane_side1_color << QVector4D(0,0,1 , 0.35 );

        QVector3D PO( point_P.x() - cur_observer.x(), point_P.y() - cur_observer.y(), point_P.z() - cur_observer.z() );
        QVector3D CO( ceiling.x() - cur_observer.x(), ceiling.y() - cur_observer.y(), ceiling.z() - cur_observer.z() );

        float l1_right = visible_region_radius;
        float l2_right = PO.length();
        float pi1_right = std::acos(PO.z()/l2_right);
        float alpha_right = (1/pi1_right) * (std::acos(l2_right/l1_right));

        const float delta_pi_right =  pi1_right / (num_interpolation); //rad, // +1 means to except P        for(int i =0 ; i < num_interpolation-1; i++)

        float unit_angle = 360.0/num_pieces * M_PI / 180; //rad
        float theta_for_P = unit_angle*(index_of_the_piece); //rad

        float cur_pi_for_right = 0.f;
        QVector<QVector3D> CP;

        float cur_index_for_right = 1;
        // num_interpolation - 1 points

        for (int i = 0 ; i< num_interpolation; i++)
        {

            cur_pi_for_right = delta_pi_right*cur_index_for_right ; //rad
            cur_index_for_right++;

            float cur_l_for_right = l1_right * std::cos(alpha_right * cur_pi_for_right); // rad, eq by junho

            QVector3D tmp_point;
            tmp_point.setX( cur_l_for_right*std::sin(cur_pi_for_right)*std::cos(theta_for_P) + cur_observer.x()); // rad!!
            tmp_point.setY( cur_l_for_right*std::sin(cur_pi_for_right)*std::sin(theta_for_P) + cur_observer.y());
            tmp_point.setZ( cur_l_for_right*std::cos(cur_pi_for_right) + cur_observer.z());


            CP << tmp_point;
            openpieces_right_plane_side1 << tmp_point;
//            openpieces_right_plane_side1_color << QVector4D(1,1,1 , 0.35 );
            openpieces_right_plane_side1_color << QVector4D(0,0,1 , 0.35 );

// this is also visualization good
            CP_debug << tmp_point;
            CP_color_debug << QVector4D(0,0.8,1 , 1 );

        }

        // side 2
        openpieces_right_plane_side2 << cur_observer;
//        openpieces_right_plane_side2_color << QVector4D(1,1,1 , 0.35 );
        openpieces_right_plane_side2_color << QVector4D(0,0,1 , 0.35 );

        for(auto iter = CP.rbegin(); iter< CP.rend(); iter++)
        {
            openpieces_right_plane_side2 << *iter;
            //        openpieces_right_plane_side2_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_right_plane_side2_color << QVector4D(0,0,1 , 0.35 );
        }
        openpieces_right_plane_side2 << ceiling;
        //        openpieces_right_plane_side2_color << QVector4D(1,1,1 , 0.35 );
                openpieces_right_plane_side2_color << QVector4D(0,0,1 , 0.35 );


        // edge
        openpieces_right_edge << cur_observer;
        openpieces_right_edge_color << QVector4D(0,0,1 , 1 );
        for(auto iter = CP.rbegin(); iter< CP.rend(); iter++)
        {
            openpieces_right_edge << *iter;
            openpieces_right_edge_color << QVector4D(0,0,1 ,1 );
        }
        openpieces_right_edge << ceiling;
        openpieces_right_edge_color << QVector4D(0,0,1 , 1 );

        // @@@ end right @@@ //////////////////////////////////////////////////////////////////////////////////////////


        // @@@ left @@@ //////////////////////////////////////////////////////////////////////////////////////////

        // side 1
        // point 1
        openpieces_left_plane_side1 << cur_observer;
        //            openpieces_left_plane_side1_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_left_plane_side1_color << QVector4D(0,0,1 , 0.35 );
        // point 2
        openpieces_left_plane_side1 << ceiling;
        //            openpieces_left_plane_side1_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_left_plane_side1_color << QVector4D(0,0,1 , 0.35 );

        QVector3D PpO( point_Pp.x() - cur_observer.x(), point_Pp.y() - cur_observer.y(), point_Pp.z() - cur_observer.z() );

        float l1_left = visible_region_radius;
        float l2_left = PpO.length();
        float pi1_left = std::acos(PpO.z()/l2_left);
        float alpha_left = (1/pi1_left) * (std::acos(l2_left/l1_left));

        const float delta_pi_left =  pi1_left / (num_interpolation); //rad

        float theta_for_Pp = unit_angle*(index_of_the_piece-1); //rad

        float cur_pi_for_left = 0.f;

        float cur_index_for_left = 1;
        QVector<QVector3D> CPp;
        // num_interpolation - 1 points
        for (int i = 0 ; i< num_interpolation; i++)
        {
            cur_pi_for_left = delta_pi_left*cur_index_for_left ; //rad
            cur_index_for_left++;

            float cur_l_for_left = l1_left * std::cos(alpha_left * cur_pi_for_left); // rad, eq by junho

            QVector3D tmp_point;
            tmp_point.setX( cur_l_for_left*std::sin(cur_pi_for_left)*std::cos(theta_for_Pp) + cur_observer.x()); // rad!!
            tmp_point.setY( cur_l_for_left*std::sin(cur_pi_for_left)*std::sin(theta_for_Pp) + cur_observer.y());
            tmp_point.setZ( cur_l_for_left*std::cos(cur_pi_for_left) + cur_observer.z());


            CPp << tmp_point;
            openpieces_left_plane_side1 << tmp_point;
//            openpieces_left_plane_side1_color << QVector4D(1,1,1 , 0.35 );
            openpieces_left_plane_side1_color << QVector4D(0,0,1 , 0.35 );

        }

        //side 2
        openpieces_left_plane_side2 << cur_observer;
//        openpieces_left_plane_side2_color << QVector4D(1,1,1 , 0.35 );
        openpieces_left_plane_side2_color << QVector4D(0,0,1 , 0.35 );

        for(auto iter = CPp.rbegin(); iter< CPp.rend(); iter++)
        {
            openpieces_left_plane_side2 << *iter;
            //        openpieces_left_plane_side2_color << QVector4D(1,1,1 , 0.35 );
                    openpieces_left_plane_side2_color << QVector4D(0,0,1 , 0.35 );
        }
        openpieces_left_plane_side2 << ceiling;
        //        openpieces_left_plane_side2_color << QVector4D(1,1,1 , 0.35 );
                openpieces_left_plane_side2_color << QVector4D(0,0,1 , 0.35 );

        //edge
        openpieces_left_edge << cur_observer;
        openpieces_left_edge_color << QVector4D(0,0,1 , 1 );
        for(auto iter = CPp.rbegin(); iter< CPp.rend(); iter++)
        {
            openpieces_left_edge << *iter;
            openpieces_left_edge_color << QVector4D(0,0,1 ,1 );
        }
        openpieces_left_edge << ceiling;
        openpieces_left_edge_color << QVector4D(0,0,1 , 1 );

        // @@@ end left @@@ //////////////////////////////////////////////////////////////////////////////////////////


        // @@@ Top @@@ //////////////////////////////////////////////////////////////////////////////////////////

        // side 1

        for(int i=0; i < CP.size() ; i++)
        {
            openpieces_top_plane_side1 << CP[i];
            openpieces_top_plane_side1_color << QVector4D(0,0,0.4 , 0.65 );
            openpieces_top_plane_side1 << CPp[i];
            openpieces_top_plane_side1_color << QVector4D(0,0,0.4 , 0.65 );
        }

//        openpieces_top_plane_side1 << ceiling;
//        openpieces_top_plane_side1_color << QVector4D(0,0,0.4 , 0.35 );

//        for(auto iter = CP.begin(); iter < CP.end(); iter++)
//        {
//            openpieces_top_plane_side1 << *iter;
//            openpieces_top_plane_side1_color << QVector4D(0,0,0.4 , 0.35 );
//        }
//        for(auto iter = CPp.begin(); iter < CP.rend(); iter++)
//        {
//            openpieces_top_plane_side1 << *iter;
//            openpieces_top_plane_side1_color << QVector4D(0,0,0.4 , 0.35 );
//        }


//        // side 2
//        // point 1
//        openpieces_top_plane_side2 << ceiling;
//        openpieces_top_plane_side2_color << QVector4D(1,1,1 , 0.35 );

//        //int size_CPp = CPp.size();
//        // 2*(n-1) points
//        for (int i = 0 ; i< size_CP; i++)
//        {
//            openpieces_top_plane_side2 << CPp[i];
//            openpieces_top_plane_side2_color << QVector4D(1,1,1 , 0.35 );

//            openpieces_top_plane_side2 << CP[i];
//            openpieces_top_plane_side2_color << QVector4D(1,1,1 , 0.35 );
//        }

        // @@@ end top @@@ //////////////////////////////////////////////////////////////////////////////////////////


//        qDebug() << "###" << i <<"th info ###";
//        qDebug() << i <<"th x is " << point_P.x();
//        qDebug() << i <<"th y is " << point_P.y();
//        qDebug() << i <<"th z is " << point_P.z();
//        qDebug() << i <<"th/ current z is " << cur_observer.z();
//        qDebug() << i <<"th r is" <<std::sqrt( (point_P.x()-cur_observer.x())*(point_P.x()-cur_observer.x()) + (point_P.y()-cur_observer.y())*(point_P.y()-cur_observer.y()));
//        qDebug() << i <<"th angle is " << getBtnAngle(point_P, cur_observer ) *180/M_PI;
//        qDebug() << "";



    } // one piece end

}// end of the function : MakeGlobalHemisphere


void OpenglWidget::MakeTriangle( QVector3D _cur_observer )
{

    points_container.cur_traiangle_vertices.clear();
    points_container.cur_traiangle_otherside_vertices.clear();
    points_container.cur_traiangle_teduri_vertices.clear();
    points_container.cur_traiangle_colors.clear();
    points_container.cur_traiangle_otherside_colors.clear();
    points_container.cur_traiangle_teduri_colors.clear();

    points_container.cur_observer = _cur_observer;

    qDebug() << "num of floor_vertices :" << points_container.floor_vertices.size();

    QVector<QVector3D>::iterator iter;
    for(iter = points_container.nonfloor_vertices.begin() ; iter != points_container.nonfloor_vertices.end() ; iter++)
    {
        if( points_container.cur_observer.distanceToPoint(*iter) < visible_region_radius \
                && std::abs(iter->z() - points_container.cur_observer.z()) > 2.0f )
        {
            points_container.cur_traiangle_vertices << points_container.cur_observer;
            points_container.cur_traiangle_colors << QVector4D(0,1,1 , 0.35);

            points_container.cur_traiangle_vertices << QVector3D( iter->x(), iter->y(), iter->z() );
            points_container.cur_traiangle_colors << QVector4D(0,1,1 , 0.35);

            points_container.cur_traiangle_vertices << QVector3D( iter->x(), iter->y(), points_container.cur_observer.z() );
            points_container.cur_traiangle_colors << QVector4D(0,1,1 , 0.35);


            // # for other side (opposite normal vector)
            points_container.cur_traiangle_otherside_vertices << points_container.cur_observer;
            points_container.cur_traiangle_otherside_colors << QVector4D(0, 1, 1 , 0.35);

            points_container.cur_traiangle_otherside_vertices << QVector3D( iter->x(), iter->y(), points_container.cur_observer.z() );
            points_container.cur_traiangle_otherside_colors << QVector4D(0, 1, 1 , 0.35);

            points_container.cur_traiangle_otherside_vertices << QVector3D( iter->x(), iter->y(), iter->z() );
            points_container.cur_traiangle_otherside_colors << QVector4D(0, 1, 1 , 0.35);

            // # for teduri
            points_container.cur_traiangle_teduri_vertices << points_container.cur_observer;
//            cur_traiangle_teduri_colors << QVector4D(0, 0.7, 0 , 1 );
            points_container.cur_traiangle_teduri_colors << QVector4D(1,1, 1 , 1 );

            points_container.cur_traiangle_teduri_vertices << QVector3D( iter->x(), iter->y(), points_container.cur_observer.z() );
            points_container.cur_traiangle_teduri_colors << QVector4D(1,1, 1 , 1 );

            points_container.cur_traiangle_teduri_vertices << QVector3D( iter->x(), iter->y(), iter->z() );
            points_container.cur_traiangle_teduri_colors << QVector4D(1,1, 1 , 1 );
        }
    }
} // end of MakeTriangles


void OpenglWidget::ClearHemisphere()
{
    openpieces_left_plane_side1.clear();
    openpieces_left_plane_side1_color.clear();
    openpieces_left_plane_side2.clear();
    openpieces_left_plane_side2_color.clear();
    openpieces_left_edge.clear();
    openpieces_left_edge_color.clear();
    openpieces_right_plane_side1.clear();
    openpieces_right_plane_side1_color.clear();
    openpieces_right_plane_side2.clear();
    openpieces_right_plane_side2_color.clear();
    openpieces_right_edge.clear();
    openpieces_right_edge_color.clear();
    openpieces_top_plane_side1.clear();
    openpieces_top_plane_side1_color.clear();
    openpieces_top_plane_side2.clear();
    openpieces_top_plane_side2_color.clear();
    openpieces_top_edge.clear();
    openpieces_top_edge_color.clear();
    openpieces_bottom_plane_side1.clear();
    openpieces_bottom_plane_side1_color.clear();
    openpieces_bottom_plane_side2.clear();
    openpieces_bottom_plane_side2_color.clear();
    openpieces_bottom_edge.clear();
    openpieces_bottom_edge_color.clear();
}

int OpenglWidget::GetNumOfRoad (liblas::Reader _reader, float _one_floor_height )
{

    int counter[num_floor];
    for (int i=0; i< num_floor; i++)
    {
        counter[i] = 0;
    }

    float gap = 0;

    _reader.Seek(0);
    while (_reader.ReadNextPoint())
    {
        typedef std::pair<double, double> minmax_t;
        minmax_t mx; minmax_t my; minmax_t mz;

        liblas::Point const& p = _reader.GetPoint();
        liblas::Color tmpcol = p.GetColor();

        float temp_x = p[0]-offset_x;
        float temp_y = p[1]-offset_y;
        float temp_z = p[2]-offset_z;

        gap = std::abs(temp_z - zmin); // zmin is identical for each las file

        int quotient = gap/(_one_floor_height);

        if( quotient < num_floor )
            counter[quotient]++;

        // SKIP
        for (int i=0; i< this->las_read_skip_num; i++)
        {
            _reader.ReadNextPoint();
        }
    }

    int max_num = *std::max_element(counter, counter+num_floor );

    return max_num;
}

float OpenglWidget::AdjustObserverZ()
{
    // input : cur_observer, road points

    float epsilon = 2;
    QVector<QVector3D> near_vertices;

    float meanz = 0;
    float sumz = 0;
    int numz = 0;

    for (auto iter = points_container.floor_vertices.begin() ; iter != points_container.floor_vertices.end(); iter++)
    {
        if( cur_observer.distanceToPoint(*iter) < epsilon )
        {
            sumz = sumz + (*iter).z() ;
            numz = numz + 1;
        }
    }

    meanz = sumz / numz ;

    return meanz;
}

void OpenglWidget::LAS_read(char* name)
{
    las_reader.readLas(name, points_container);
}// end of LAS_read


// ### Only once executed when the widget is starated ###
OpenglWidget::OpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    char filename[1000];

    this->a_count = 0;
    zmin = 0;
    cur_observer_index = 600000; // initialize

    log.open("coordinates.log");

    /*### note ###
        - skip 1000 is too sparse at near. recommend at least 100
        - color of n is wrong...

    */

    // ### one file test region
    sprintf(filename, "/media/gskim/Hitachi_giseop/data/yoido130G/Track_B_Track_B_20161205_034659 Profiler.zfs_27.las"); // A24 : floor_boundary = 6.7f; good
    LAS_read(filename);
    sprintf(filename, "/media/gskim/Hitachi_giseop/data/yoido130G/Track_B_Track_B_20161205_034659 Profiler.zfs_28.las"); // A24 : floor_boundary = 6.7f; good
    LAS_read(filename);
    sprintf(filename, "/media/gskim/Hitachi_giseop/data/yoido130G/Track_B_Track_B_20161205_034659 Profiler.zfs_29.las"); // A24 : floor_boundary = 6.7f; good
    LAS_read(filename);

    //    sprintf(filename, "../data/Track_A_Track_A_20161205_032536 Profiler.zfs_%d.las", 10); // A10 : boundary =7  good
//    LAS_read(filename);

//    sprintf(filename, "../data/Track_B_Track_B_20161205_034659 Profiler.zfs_%d.las", 15);
//    LAS_read(filename);
//    sprintf(filename, "../data/Track_B_Track_B_20161205_034659 Profiler.zfs_%d.las", 16);
//    LAS_read(filename);


//    #define trackA
//      #define trackB
//    #define trackC
//    #define trackD
//    #define trackE
//    #define trackF
//    #define trackG
//    #define trackL
//    #define trackM // onefloorheight = 2 is proper.
//    #define trackN
//    #define trackP


    /*
     * ## set loading hyper parameter
     *  - 1. set start index
     *  - 2. set the number of how many files from start num
     */

    // ## Before call LAS_read, set how many : keep interval (for efficiency of speed and memory)
    //las_read_skip_num = 1000;     // for estimate the shape of roads for many files
    //las_read_skip_num = 100;      // for treating one track normally
    //las_read_skip_num = 30;       // for treating one track withour big loose of accuracy
    las_read_skip_num = 10;       // for treating one track withour big loose of accuracy
    //las_read_skip_num = 0;        // for very high accuracy


    // Index of A : 0 to 44
    const int num_of_file_yeouido_track_A = 45;
    int index_of_start_file_A = 0;
    //int index_of_start_file_A = 23;
    //int how_many_load_A = 20;
    int how_many_load_A = num_of_file_yeouido_track_A;

    const int num_of_file_yeouido_track_B = 45;
    int index_of_start_file_B = 40;
    int how_many_load_B = 5;
    //int how_many_load_B = num_of_file_yeouido_track_B;

    const int num_of_file_yeouido_track_C = 52;
    int index_of_start_file_C = 0;
    //int how_many_load_C = 2;
    int how_many_load_C = num_of_file_yeouido_track_C;

    const int num_of_file_yeouido_track_D = 10;
    int index_of_start_file_D = 0;
    //int how_many_load_D = 3;
    int how_many_load_D = num_of_file_yeouido_track_D;

    const int num_of_file_yeouido_track_E = 47;
    int index_of_start_file_E = 0;
    int how_many_load_E = num_of_file_yeouido_track_E;


    const int num_of_file_yeouido_track_F = 58;
    int index_of_start_file_F = 0;
    //int how_many_load_F = 10;
    int how_many_load_F = num_of_file_yeouido_track_F;


    const int num_of_file_yeouido_track_G = 58;
    int index_of_start_file_G = 0;
    int how_many_load_G = num_of_file_yeouido_track_G;

    const int num_of_file_yeouido_track_L_1 = 19; // 0 to 18, and 19to22 are not exist
    int index_of_start_file_L_1 = 0;
    int how_many_load_L_1 = 0;
    //int how_many_load_L_1 = num_of_file_yeouido_track_L_1;

    const int num_of_file_yeouido_track_L_2 = 18; // 23 to 40
    int index_of_start_file_L_2 = 23;
    int how_many_load_L_2 = 4;
    //int how_many_load_L_2 = num_of_file_yeouido_track_L_2;

    const int num_of_file_yeouido_track_M = 45;
    int index_of_start_file_M = 0;
    int how_many_load_M = 5;
    //int how_many_load_M = num_of_file_yeouido_track_M;

    const int num_of_file_yeouido_track_N = 18; // 2 to 19, and 0to1 are not exist
    int index_of_start_file_N = 2;
    int how_many_load_N = num_of_file_yeouido_track_N;

    const int num_of_file_yeouido_track_P = 22; // 2 to 24, and 0to1 are not exist
    int index_of_start_file_P = 2;
    int how_many_load_P = num_of_file_yeouido_track_P;


    int how_many_load = 0;

#ifdef trackA
    // ## load track A ##
    for(int i = 0 ; i < how_many_load_A ; i++)
    {
        sprintf(filename, "../data/Track_A_Track_A_20161205_032536 Profiler.zfs_%d.las", index_of_start_file_A);
        LAS_read(filename);
        index_of_start_file_A++;
    }
#endif

#ifdef trackB
    // ## load track B ##
    for(int i = 0 ; i < how_many_load_B ; i++)
    {
//        sprintf(filename, "../data/Track_B_Track_B_20161205_034659 Profiler.zfs_%d.las", index_of_start_file_B);
        sprintf(filename, "/media/gskim/Data/lasdata/yoido/Track_B_Track_B_20161205_034659 Profiler.zfs_%d.las", index_of_start_file_B);
        LAS_read(filename);
        index_of_start_file_B++;
    }
#endif

#ifdef trackC
    // ## load track C ##
    for(int i = 0 ; i < how_many_load_C ; i++)
    {
        sprintf(filename, "../data/Track_C_Track_C_20161205_040304 Profiler.zfs_%d.las", index_of_start_file_C);
        LAS_read(filename);
        index_of_start_file_C++;
    }
#endif

#ifdef trackD
    // ## load track D ##
    for(int i = 0 ; i < how_many_load_D ; i++)
    {
        sprintf(filename, "../data/Track_D_Track_D_20161205_042749 Profiler.zfs_%d.las", index_of_start_file_D);
        LAS_read(filename);
        index_of_start_file_D++;
    }
#endif

#ifdef trackE
    // ## load track E #
    for(int i = 0 ; i < how_many_load_E ; i++)
    {
        sprintf(filename, "../data/Track_E_Track_E_20161205_043706 Profiler.zfs_%d.las", index_of_start_file_E);
        LAS_read(filename);
        index_of_start_file_E++;
    }
#endif

#ifdef trackF
    // ## load track F ##
    for(int i = 0 ; i < how_many_load_F ; i++)
    {
        sprintf(filename, "../data/Track_F_Track_F_20161205_045859 Profiler.zfs_%d.las", index_of_start_file_F);
        LAS_read(filename);
        index_of_start_file_F++;
    }
#endif

#ifdef trackG
    // ## load track G ##
    for(int i = 0 ; i < how_many_load_G ; i++)
    {
        sprintf(filename, "../data/Track_G_Track_G_20161206_002811 Profiler.zfs_%d.las", index_of_start_file_G);
        LAS_read(filename);
        index_of_start_file_G++;
    }
#endif

#ifdef trackL
    // ## load track L ##
    for(int i = 0 ; i < how_many_load_L_1 ; i++)
    {
        sprintf(filename, "../data/Track_L_Track_L_20161216_044051 Profiler.zfs_%d.las", index_of_start_file_L_1);
        LAS_read(filename);
        index_of_start_file_L_1++;
    }

    for(int i = 0 ; i < how_many_load_L_2 ; i++)
    {
        sprintf(filename, "../data/Track_L_Track_L_20161216_044051 Profiler.zfs_%d.las", index_of_start_file_L_2);
        LAS_read(filename);
        index_of_start_file_L_2++;
    }

#endif

#ifdef trackM
    // ## load track M ##
    for(int i = 0 ; i < how_many_load_M ; i++)
    {
        sprintf(filename, "../data/Track_M_Track_M_20161216_050038 Profiler.zfs_%d.las", index_of_start_file_M);
        LAS_read(filename);
        index_of_start_file_M++;
    }
#endif

#ifdef trackN
    // ## load track N ##
    for(int i = 0 ; i < how_many_load_N ; i++)
    {
        sprintf(filename, "../data/Track_N_Track_N_20161216_052806 Profiler.zfs_%d.las", index_of_start_file_N);
        LAS_read(filename);
        index_of_start_file_N++;
    }
#endif

#ifdef trackP
    // ## load track P ##
    for(int i = 0 ; i < how_many_load_P ; i++)
    {
        sprintf(filename, "../data/Track_P_Track_P_20161216_055438 Profiler.zfs_%d.las", index_of_start_file_P);
        LAS_read(filename);
        index_of_start_file_P++;
    }
#endif


    //// Set Initial Gl variables
    //_camPosition = QVector3D(0, 0, 10);
    _camPosition = QVector3D(0, 0, 150); // look at high height (for seeing large area)
    //_camPosition = QVector3D( offset_x , offset_y, offset_z+250);

    _camFront = QVector3D(0, 0, -1);
    _camUpDirection = QVector3D(0, 1, 0);
    _rot = QVector3D(0, 0, 0);

    _pMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _mMatrix.setToIdentity();
    setFocusPolicy(Qt::StrongFocus);

    qDebug() << "Start GL";

    qDebug() << "Making Openness polygons...";
    MakeTriangle(points_container.floor_vertices[points_container.cur_observer_index]);
    MakeGlobalHemisphere(points_container.floor_vertices[points_container.cur_observer_index]);


}

OpenglWidget::~OpenglWidget()
{

}


void OpenglWidget::initShaders()
{
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vertexShader.vsh");
    _shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fragmentShader.fsh");
    _shaderProgram.link();
}


void OpenglWidget::initializeGL()
{
    qDebug() << "initial GL";

    // set up the rendering context
    initShaders();
    visualizer.initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_5_Core>();
    f->glClearColor(.0f, .0f, .0f, 1.0f); //background is black

    qDebug() << "initial GL done";

}


void OpenglWidget::resizeGL(int width, int height)
{
    qDebug() << "resizeGL GL";

    // Calculate aspect ratio
    qreal aspect = qreal(width) / qreal(height ? height : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.001, zFar = 5000, fov = 60.0;

    // Reset projection
    _pMatrix.setToIdentity();

    // Set perspective projection
    _pMatrix.perspective(fov, aspect, zNear, zFar);

}

void OpenglWidget::paintGL()
{
    ///// Must Necessary before Drawing /////
    //#Buffer Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //#Set Camera Parameter
    _mMatrix.setToIdentity();
    _vMatrix.setToIdentity();
    _vMatrix.lookAt(_camPosition, _camPosition+_camFront, _camUpDirection);
    _T = _pMatrix * _vMatrix * _mMatrix;

    visualizer.initializeCamera(_T, _pMatrix, _mMatrix, _vMatrix, _camPosition, _camFront, _camUpDirection);
    ///// end of Must Necessary before Drawing /////

    ///// Main Part of Visualization /////
    //#All points
    if(VISUALIZE_ALL == true) visualizer.drawPoints(points_container.las_vertices, points_container.las_colors);
    //#Only floor points
    if(VISUALIZE_FLOOR_ONLY == true) visualizer.drawPoints(points_container.floor_vertices, points_container.floor_colors);
    //#Non-floor points
    if(VISUALIZE_NONFLOOR_ONLY == true) visualizer.drawPoints(points_container.nonfloor_vertices, points_container.nonfloor_colors);
    //#Visualize triangles(defulat : both side, transparency)
    if(VISUALIZE_TRIANGLE == true)
    {
        visualizer.drawTriangles(points_container.cur_traiangle_vertices, points_container.cur_traiangle_colors, \
                                 points_container.cur_traiangle_otherside_vertices, points_container.cur_traiangle_otherside_colors); // area(both side)
        visualizer.drawLinestrip(points_container.cur_traiangle_teduri_vertices, points_container.cur_traiangle_teduri_colors, 3); // edegs
    }
    //#Trace of P(first cutting points)
    visualizer.drawLinestrip(point_P_debug, point_P_color_debug, point_P_debug.size());
    visualizer.drawLinestrip(point_Pp_debug, point_Pp_color_debug, point_Pp_debug.size());
    //#Edges of Open Hemisphere
    visualizer.drawLinestrip(CP_debug, CP_color_debug, num_interpolation);
    //#Volume of Open Hemisphere
    if( VISUALIZE_OPENHEMISPHERE == true )
    {
        //left side
        float period_of_left_plane = num_interpolation+2; // num(O+C+CP) = N+1 because cp has N points
        visualizer.drawTriangleFan(openpieces_left_plane_side1, openpieces_left_plane_side1_color, openpieces_left_plane_side2, openpieces_left_plane_side2_color, period_of_left_plane);
        visualizer.drawLinestrip(openpieces_left_edge, openpieces_left_edge_color, period_of_left_plane);

        //right side
        float period_of_right_plane = num_interpolation+2;
        visualizer.drawTriangleFan(openpieces_right_plane_side1, openpieces_right_plane_side1_color, openpieces_right_plane_side2, openpieces_right_plane_side2_color, period_of_right_plane);
        visualizer.drawLinestrip(openpieces_left_edge, openpieces_left_edge_color, period_of_right_plane);

        //top side
        float period_of_top_plane = 2*num_interpolation;
        visualizer.drawQuadstrip(openpieces_top_plane_side1, openpieces_top_plane_side1_color, period_of_top_plane);
    }
    ///// end of Main Part of Visualization /////
}

QSize OpenglWidget::sizeHint() const
{
    return QSize(1024,960);
}

void OpenglWidget::mousePressEvent(QMouseEvent *event)
{
    _lastMousePosition = event->pos();
    event->accept();
}

void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{

    float deltaX = event->x() - _lastMousePosition.x();
    float deltaY = _lastMousePosition.y() - event->y(); // because 2d image has lower side, high y value

    float default_sensitivity = 0.1;
    float gamdo_for_transform = 0.01;
    float weight_for_rotation = 0.3;

    // depending on the height, adjust the translation gamdo
    if( _camPosition[2] <50 )
        gamdo_for_transform = 0.04;
    else if ( _camPosition[2] < 200)
        gamdo_for_transform = 0.2;
    else
        gamdo_for_transform = 0.5;

    // Translation motion
    if (event->buttons() & Qt::LeftButton)
    {
        QVector3D left = QVector3D::normal(_camUpDirection, _camFront);
        _camPosition = _camPosition -  gamdo_for_transform * deltaY * _camUpDirection;
        _camPosition = _camPosition +  gamdo_for_transform * deltaX * left ;
    }

    // Rotation motion
    if (event->buttons() & Qt::RightButton)
    {
        deltaX *= default_sensitivity;
        deltaY *= default_sensitivity;

        QMatrix4x4 rotation;
        rotation.setToIdentity();
        QVector3D right = QVector3D::normal(_camFront, _camUpDirection);

        rotation.rotate(deltaY, right);
        rotation.rotate(-deltaX, 0, 0, 1);
        _camFront = - weight_for_rotation*rotation*_camFront;
        qDebug() << "cam front :" <<_camFront <<endl;
        _camUpDirection = - weight_for_rotation*rotation*_camUpDirection;
        qDebug() << "cam up dir :" <<_camUpDirection <<endl;

    }

    _lastMousePosition = event->pos();
    event->accept();
    update();

}

void OpenglWidget::wheelEvent(QWheelEvent *event)
{

    int delta = (float)event->delta();
    float gamdo = 1;

    if( _camPosition[2] <50 )
        gamdo = 3;
    else if ( _camPosition[2] < 200)
        gamdo = 7;
    else
        gamdo = 15;

    if (event->orientation() == Qt::Vertical)
    {
        if (delta < 0)
        {
            _camPosition -= gamdo * _camFront;
        }
        else if (delta > 0)
        {
            _camPosition += gamdo * _camFront;
        }
        update();
    }

}

void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 0.05f;
    if (event->key() == Qt::Key_Up) {
        float newy = points_container.cur_observer.y() + moving_gamdo;
        points_container.cur_observer.setY(newy);
        MakeTriangle(points_container.cur_observer);

        if(toggel_onoff_hemisphere == true)
            MakeGlobalHemisphere(points_container.cur_observer);

        //        qDebug() << "total floor vertices num is : " << floor_vertices.size();
//        qDebug() << "total nonfloor vertices num is : " << nonfloor_vertices.size();
//        qDebug() << "total all vertices num is : " << las_vertices.size();
//        qDebug() << endl;
    }
    if (event->key() == Qt::Key_Down) {
        float newy = points_container.cur_observer.y() - moving_gamdo;
        points_container.cur_observer.setY(newy);
        MakeTriangle(points_container.cur_observer);
        if(toggel_onoff_hemisphere == true)
            MakeGlobalHemisphere(points_container.cur_observer);
    }
    if (event->key() == Qt::Key_Left ) {
        float newx = points_container.cur_observer.x() + moving_gamdo;
        points_container.cur_observer.setX(newx);
        MakeTriangle(points_container.cur_observer);
        if(toggel_onoff_hemisphere == true)
            MakeGlobalHemisphere(points_container.cur_observer);
    }
    if (event->key() == Qt::Key_Right ) {
        float newx = points_container.cur_observer.x() - moving_gamdo;
        points_container.cur_observer.setX(newx);
        MakeTriangle(points_container.cur_observer);
        if(toggel_onoff_hemisphere == true)
            MakeGlobalHemisphere(points_container.cur_observer);
    }
    // also need to make z up down function key
    if (event->key() == Qt::Key_Delete ) {
        float newz = cur_observer.z() - 0.2;
        cur_observer.setZ(newz);
        MakeTriangle(cur_observer);

        MakeGlobalHemisphere(cur_observer);
    }
    if (event->key() == Qt::Key_Insert ) {
        float newz = cur_observer.z() + 0.2;
        cur_observer.setZ(newz);
        MakeTriangle(cur_observer);
        MakeGlobalHemisphere(cur_observer);
    }
    if (event->key() == Qt::Key_R ) {
        float newz = AdjustObserverZ();
        cur_observer.setZ(newz);
        MakeTriangle(cur_observer);
//        MakeGlobalHemisphere(cur_observer);
    }
    if (event->key() == Qt::Key_O ) {
        if(toggel_onoff_hemisphere == true)
            toggel_onoff_hemisphere = false;
        else
            toggel_onoff_hemisphere = true;
    }


    if (event->key() == Qt::Key_0 ) {

        _camFront = QVector3D(0, 0, -1);
        _camUpDirection = QVector3D(0, 1, 0);
        _rot = QVector3D(0, 0, 0);
    }

    if (event->key() == Qt::Key_1 ) {
        if(VISUALIZE_ALL == false )
            VISUALIZE_ALL = true;
        else
        {
            VISUALIZE_ALL = false;
            VISUALIZE_FLOOR_ONLY = false;
            VISUALIZE_NONFLOOR_ONLY = false;
        }
    }
    if (event->key() == Qt::Key_2 ) {
        if(VISUALIZE_FLOOR_ONLY == false )
            VISUALIZE_FLOOR_ONLY = true;
        else
            VISUALIZE_FLOOR_ONLY = false;
    }
    if (event->key() == Qt::Key_3 ) {
        if(VISUALIZE_NONFLOOR_ONLY == false )
            VISUALIZE_NONFLOOR_ONLY = true;
        else
            VISUALIZE_NONFLOOR_ONLY = false;
    }
    if (event->key() == Qt::Key_4 ) {
        if(VISUALIZE_TRIANGLE == false )
            VISUALIZE_TRIANGLE = true;
        else
            VISUALIZE_TRIANGLE = false;
    }

    if (event->key() == Qt::Key_5 ) {
        if(VISUALIZE_OPENHEMISPHERE == false )
            VISUALIZE_OPENHEMISPHERE = true;
        else
            VISUALIZE_OPENHEMISPHERE = false;
    }



    update();
}
