#ifndef SWAG_SCANNER_FILEHANDLER_H
#define SWAG_SCANNER_FILEHANDLER_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "CloudType.h"
#include "PathType.h"
#include <string>
#include <unordered_map>

namespace file {
    /**
     * Contains functions to handle file saving and loading.
     */
    class FileHandler {
    public:


        inline static const std::string default_data_path = "/Users/seanngpack/Programming Stuff/Projects/scanner_files";

        /**
         * Sorting function that sorts files and directories in order from lowest to greatest.
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
         * Default constructor makes a FileHandler using my default path. Only works
         * on Sean's computer. The default path points to my all data folder then it will
         * auto generate a scan folder for me.
         */
        FileHandler();

        /**
         * Constructor takes in a flag and determines whether to create a new scan folder
         * or not. If the flag is set to true then it is identical to the default constructor.
         * @param auto_create_flag
         */
        FileHandler(bool auto_create_flag);

        /**
         * Constructor for FileHandler given a folder path and a PathType indicating
         * whether it is a path to all the scans or a specific scan folder. If it is the former,
         * will auto create a new scan folder and use that as the current scan folder.
         * If it is the latter than it will not auto create a new folder and just use
         * the given scan folder as the current scan folder.
         *
         * @param folder_path A path to either all the scans or a path to a specific scan folder.
         * @param path_type whether it is an all data path or specific scan folder path.
         */
        FileHandler(const std::string &folder_path,
                    PathType::Type path_type);


        /**
         * Set the scan_folder_path instance variable. Should be pointed to the scan folders.
         * E.g user/scanner_data/10
         * If the subfolders don't exist then create them.
         * @param path the folder path.
         */
        void set_scan_folder_path(const std::string &path);

        /**
         * Get the current scan folder.
         * @return scan folder.
         */
        std::string get_scan_folder_path();

        /**
         * NOTE: Currently does not support full paths.
         * Save the given cloud to the current output_path.
         * @param cloud the cloud you want to save. This will change the folder
         * path that the cloud gets saved in.
         * @para cloud_type enum for the type of cloud you are saving.
         */
        void save_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud,
                        const std::string &cloud_name,
                        CloudType::Type cloud_type);

        /**
         * NOTE: Currently does not support full paths.
         * TODO: Give this method same optional path functionality as laod_clouds
         * Load a pointcloud from the current_scan_folder given the name and type.
         * @param cloud the cloud you want to load the cloud into.
         * @param cloud_name name of the cloud.
         * @param cloud_type type of the cloud.
         *
         * Example: load_cloud("12.pcd", CloudType::RAW)
         */
        void load_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                        const std::string &cloud_name,
                        CloudType::Type cloud_type);


        /**
         * Loads all clouds in the current scan folder into a vector.
         * Only works with files that have numbers in the file name.
         * If the folder path is not given and the auto_create_flag is false, then
         * throw and error because the path is not set. If you give the method a parameter for the
         * folder_path then it will ignore scan_folder_path and use the inputted path.
         * @param cloud_vector the vector you want to load the clouds into.
         * @param cloud_type determines which folder to search for.
         */
        void load_clouds(
                std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr,
                        Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr> > &cloud_vector,
                CloudType::Type cloud_type,
                const std::string &folder_path = std::string());

    private:
        std::string all_data_folder_path;
        std::string scan_folder_path;


        /**
         * Given the all data folder, find the current scan folder.
         * E.g. if there are scans 1->10 in the all data folder, that means the current
         * scan must be 11.
         * Does not make the folder for the current scan.
         * @param folder the all data folder.
         *
         */
        std::string find_scan_folder(const std::string &folder);

        /**
         * Create the sub folders defined in CloudTypes in the scan_folder_path if they
         * don't exist.
         */
        void create_sub_folders();

        /**
         * Check if the folder exists. If not, then throw an error.
         * @param folder the folder that houses all the scanner data.
         * @returns true if the input is good.
         */
        static bool check_folder_input(const std::string &folder);

        /**
         * Check to see if a file exists given the path.
         * @param file_path the path to the file.
         * @return true if it exists.
         * @throws illegal argument exception if there isn't a file there.
         */
        static bool check_file_input(const std::string &file_path);
    };
}
#endif //SWAG_SCANNER_FILEHANDLER_H
