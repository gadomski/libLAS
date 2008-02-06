#if defined(_MSC_VER) && defined(USE_VLD)
#include <vld.h>
#endif
// liblas
#include <liblas/cstdint.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/detail/timer.hpp>
//std
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
// sample
#include "utility.hpp"
using namespace std;

int main()
{
    try
    {
        //char const* name = "d:\\data\\lidar\\LDR030828_213450_0.LAS"; 
        //char const* name = "d:\\data\\lidar\\Sample_LiDAR_LAS_File.las";
        //char const* name = "d:\\data\\lidar\\iowa\\04164492.las";
        //char const* name = "d:\\data\\lidar\\Serpent_Mound_Model.las";
        //char const* name = "d:\\dev\\liblas\\_svn\\trunk\\test\\data\\TO_core_last_clip.las";
        char const* name = "d:\\data\\lidar\\gilmer\\000001.las";

        std::ifstream ifs(name, ios::in | ios::binary);
        liblas::LASReader reader(ifs);
        
        cout << "File name: " << name << '\n';
        cout << "Signature: " << reader.GetHeader().GetFileSignature() << '\n';
        cout << "Version  : " << reader.GetVersion() << '\n';
        cout << "Format   : " << reader.GetHeader().GetDataFormatId() << '\n';
        cout << "Project  : " << reader.GetHeader().GetProjectId() << '\n';
        cout << "Points count: " << reader.GetHeader().GetPointRecordsCount() << '\n';
        cout << "VLRecords count: " << reader.GetHeader().GetRecordsCount() << '\n';
        

        liblas::detail::Timer t;
        t.start();
        
        typedef std::pair<double, double> minmax_t;
        minmax_t mx;
        minmax_t my;
        minmax_t mz;
        liblas::uint32_t i = 0;
        while (reader.ReadNextPoint())
        {
            liblas::LASPoint const& p = reader.GetPoint();

            mx.first = std::min<double>(mx.first, p[0]);
            mx.second = std::max<double>(mx.second, p[0]);
            my.first = std::min<double>(my.first, p[1]);
            my.second = std::max<double>(my.second, p[1]);
            mz.first = std::min<double>(mz.first, p[2]);
            mz.second = std::max<double>(mz.second, p[2]);
            
            // Warning: Printing zillion of points may take looong time
            //cout << i << ". " << p << '\n';
            i++;
        }
        double const d = t.stop();

        if (reader.GetHeader().GetPointRecordsCount() != i)
            throw std::runtime_error("read incorrect number of point records");

        cout << "Read points: " << i << " (" << d << ")\n"
            << std::fixed << std::setprecision(6)
            << "\nX: " << mx
            << "\nY: " << my
            << "\nZ: " << mz    
            << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
    }

    return 0;
}