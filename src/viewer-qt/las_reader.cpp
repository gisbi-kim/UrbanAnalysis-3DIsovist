#include "las_reader.h"

void LasReader::printInfo(char *name, liblas::Header const& header)
{
    std::cout << "File name: " << name << '\n';
    std::cout << "Format : " << header.GetDataFormatId() << '\n';
    std::cout << "Project : " << header.GetProjectId() << '\n';
    std::cout << "Points count: " << header.GetPointRecordsCount() << '\n';
    std::cout << "VLRecords count: " << header.GetRecordsCount() << '\n';
    std::cout << "Points by return: ";
    std::copy(header.GetPointRecordsByReturnCount().begin(), \
           header.GetPointRecordsByReturnCount().end(), \
          std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void LasReader::skipPoints(liblas::Reader reader, unsigned int num)
{
    // how many SKIP? : for efficiency of memory and speed
    for (int i=0; i< num; i++)
    {
        reader.ReadNextPoint();
    }

}

void LasReader::updateOrNotZmin(double cur_z)
{
    if(cur_z-offset_z < zmin)
    {
        zmin = cur_z-offset_z; // update the minimum value of z value
    }
}

void LasReader::initOffset(liblas::Point const & cur_read_point)
{
    //Save the points with new arbitrary coordinates (decided by a first read point)
    if(IsFirstTime == true)
    {
        offset_x = cur_read_point[0];
        offset_y = cur_read_point[1];
        offset_z = cur_read_point[2];
        IsFirstTime = false; // important! must only once executed!
    }
}

void LasReader::readLas(char * las_file_name, PointsContainer & container)
{
    //#0-1 Make Reader
    std::ifstream ifs;
    if (!liblas::Open(ifs, las_file_name))
        throw std::runtime_error(std::string("Can not open ") + las_file_name);
    liblas::Reader reader(ifs);
    liblas::Header const& las_header = reader.GetHeader();

    //#0-2 Print info
    printInfo(las_file_name, las_header);

    //#1 First read : Save All points
    while (reader.ReadNextPoint())
    {
        //parse
        liblas::Point const & cur_read_point = reader.GetPoint();
        liblas::Color cur_read_point_color = cur_read_point.GetColor();

        //init offset (only once)
        initOffset(cur_read_point);

        //pushback point and color of it
        container.las_vertices << QVector3D(cur_read_point[0] - offset_x, cur_read_point[1] - offset_y, cur_read_point[2] - offset_z);
        container.las_colors << QVector4D(cur_read_point_color.GetRed()/65535.f, cur_read_point_color.GetGreen()/65535.f, cur_read_point_color.GetBlue()/65535.f , 1);

        //update the information of z-min value for floor extraction
        updateOrNotZmin(cur_read_point[2]);

        //skip read of some points
        skipPoints(reader, num_read_skip);
    }

    //#2 Second read : Save floor/non-floor points seperately
    //2-1 get range of floor
    int counter[num_floor];
    for (int i=0; i< num_floor; i++)
        counter[i] = 0;
    float gap = 0;

    reader.Seek(0);
    while (reader.ReadNextPoint())
    {
        typedef std::pair<double, double> minmax_t;
        minmax_t mx; minmax_t my; minmax_t mz;

        liblas::Point const& p = reader.GetPoint();
        liblas::Color tmpcol = p.GetColor();

        float temp_z = p[2]-offset_z;
        gap = std::abs(temp_z - zmin); // zmin is identical for each las file

        int quotient = gap/one_floor_height_for_extract_road;
        if(quotient < num_floor)
            counter[quotient]++;

        skipPoints(reader, num_read_skip);
    }

    int max_num = *std::max_element(counter, counter+num_floor );
    int max_index = 0;
    for (int i=0; i< num_floor; i++)
    {
        if ( counter[i] == max_num )
            max_index = i;
    }

    //2-2 divide and save floor/non-floor points
    reader.Seek(0);
    while (reader.ReadNextPoint())
    {
      liblas::Point const& p = reader.GetPoint();
      liblas::Color tmpcol = p.GetColor();

      float temp_x = p[0]-offset_x;
      float temp_y = p[1]-offset_y;
      float temp_z = p[2]-offset_z;
      gap = std::abs(temp_z - zmin); // zmin is identical for each las file

      if( (one_floor_height_for_extract_road*max_index < gap)\
          && (gap < one_floor_height_for_extract_road*(max_index+1)) )
      {
          container.floor_vertices << QVector3D(temp_x, temp_y, temp_z);
          container.floor_colors << QVector4D( tmpcol.GetRed()/65535.f, tmpcol.GetGreen()/65535.f, tmpcol.GetBlue()/65535.f , 1 );
      }
      else
      {
          container.nonfloor_vertices << QVector3D(temp_x, temp_y, temp_z);
          container.nonfloor_colors << QVector4D( tmpcol.GetRed()/65535.f, tmpcol.GetGreen()/65535.f, tmpcol.GetBlue()/65535.f , 1 );
      }

      skipPoints(reader, num_read_skip);
    }

    container.cur_observer_index = 40000;

}

void LasReader::setParentPath(std::string path)
{
    parent_path = path;
}


void LasReader::readLasFiles()
{
    //type your parent path
    setParentPath("/media/gskim/Hitachi_giseop/data/yoido130G/");





}
