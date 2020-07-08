#ifndef SWAG_SCANNER_FILEHANDLER_H
#define SWAG_SCANNER_FILEHANDLER_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "CloudType.h"
#include <string>
#include <unordered_map>
#include <CoreServices/CoreServices.h>
#include "json.hpp"

using json = nlohmann::json;
namespace file {
    /**
     * Abstract class for File Handling objects.
     */
    class FileHandler {
    public:

        /**
         * Save the given cloud to the current output_path.
         * @param cloud the cloud you want to save.
         * @para cloud_type enum for the type of cloud you are saving. Affects the subfolder path.
         */
        virtual void save_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud,
                                const std::string &cloud_name,
                                CloudType::Type cloud_type) = 0;

        /*
         * Load a pointcloud from the scan folder given the name and type.
         * @param cloud the cloud you want to load the cloud into
         * @param cloud_name name of the cloud.
         * @param cloud_type type of the cloud.
         *
         * Example: load_cloud(cloud, "testScan", "12.pcd", CloudType::RAW)
         */
        virtual void load_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                                const std::string &cloud_name,
                                CloudType::Type cloud_type) = 0;


        /**
         * Loads all clouds in the current scan folder into a vector given the cloud type.
         * Only works with files that have numbers in the file name.
         *
         * @Edge case when passed CALIBRATION as the type, it will search through the
         * /calibration folder in the root directory and load the latest calibration.
         * @param cloud_vector the vector you want to load the clouds into.
         * @param cloud_type determines which folder to search for.
         *
         */
        virtual void load_clouds(
                std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr,
                Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr> > &cloud_vector,
                CloudType::Type cloud_type) = 0;


        virtual std::string get_scan_name() = 0;

        virtual void set_scan_name(const std::string &scan_name) = 0;

        virtual ~FileHandler() {
            std::cout << "calling FileHandler destructor \n";
        }

    protected:
        boost::filesystem::path swag_scanner_path = []() {
            FSRef ref;
            OSType folderType = kApplicationSupportFolderType;
            char path[PATH_MAX];

            FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);
            FSRefMakePath(&ref, (UInt8 *) &path, PATH_MAX);
            boost::filesystem::path program_folder = "/SwagScanner";
            program_folder = path / program_folder;
            return program_folder;
        }();
        //TODO: migrate to std::filesystem once changes are implementede
        boost::filesystem::path scan_folder_path;
        std::string scan_name;

        /**
         * Sorting function that sorts files and directories numerically in order from lowest to greatest.
         * @param path1 first path.
         * @param path2 second path.
         * @return true if the first file is smaller than the second, false otherwise.
         */
        inline static bool path_sort(boost::filesystem::path &path1, boost::filesystem::path &path2) {
            std::string string1 = path1.string();
            std::string string2 = path2.string();

            // remove ".pcd" from files
            if (path1.has_extension()) {
                size_t idx = string1.find_last_of(".");
                string1 = string1.substr(0, idx);
            }

            if (path2.has_extension()) {
                size_t idx = string2.find_last_of(".");
                string2 = string2.substr(0, idx);
            }

            // find the ending numbers of string1
            size_t last_index = string1.find_last_not_of("0123456789");

            std::string result1 = (string1.substr(last_index + 1));
            // find the ending numbers of string 2
            last_index = string2.find_last_not_of("0123456789");
            std::string result2 = string2.substr(last_index + 1);

            if (result1.length() == 0 || result2.length() == 0) {
                return true;
            }
            return (std::stoi(result1) < std::stoi(result2));
        }

        /**
         * Go to the SwagScanner/calibration directory and find the latest calibration by date.
         * @return path to the latest calibration.
         */
        inline boost::filesystem::path find_latest_calibration() {
            std::string someDir = swag_scanner_path.string() + "/calibration";
            typedef std::multimap<std::time_t, boost::filesystem::path> result_set_t;
            result_set_t result_set;

            // store folders in ascending order
            if (boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir)) {
                for (auto &&x : boost::filesystem::directory_iterator(someDir)) {
                    if (is_directory(x) && x.path().filename() != ".DS_Store") {
                        result_set.insert(result_set_t::value_type(last_write_time(x.path()), x.path()));
                    }
                }
            }
            // get the last element which is the latest date
            // gives the path to the .json file inside the folder so that's why it's dirty
            boost::filesystem::path p =
                    result_set.rbegin()->second / "/" / result_set.rbegin()->second.filename() / ".json";
            return p;
        }
    };
}
#endif //SWAG_SCANNER_FILEHANDLER_H
