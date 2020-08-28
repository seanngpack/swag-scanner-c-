#include "ProcessingController.h"
#include "Constants.h"
#include <cmath>
#include <pcl/common/transforms.h>
#include "json.hpp"

using json = nlohmann::json;

controller::ProcessingController::ProcessingController(std::shared_ptr<model::Model> model,
                                                       std::shared_ptr<visual::Visualizer> viewer,
                                                       std::shared_ptr<file::ScanFileHandler> file_handler) :
        model(std::move(model)), viewer(std::move(viewer)), file_handler(std::move(file_handler)) {}

void controller::ProcessingController::run() {
    rotate_all_clouds(CloudType::Type::FILTERED);
}

void controller::ProcessingController::crop_clouds(CloudType::Type cloud_type) {
    using namespace constants;

    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr>> cloud_vector;
    file_handler->load_clouds(cloud_vector, cloud_type);
    for (int i = 0; i < cloud_vector.size(); i++) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cropped_cloud = model->crop_cloud(cloud_vector[i],
                                                                              min_x, max_x,
                                                                              min_y, max_y,
                                                                              min_z, max_z);
        std::cout << "saving cropped cloud to" << std::endl;
        file_handler->save_cloud(cropped_cloud, std::to_string(i) + ".pcd", CloudType::Type::PROCESSED);
    }
}

void controller::ProcessingController::remove_planes(CloudType::Type cloud_type) {
    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr>> cloud_vector;
    file_handler->load_clouds(cloud_vector, cloud_type);
    for (int i = 0; i < cloud_vector.size(); i++) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr segmented_cloud = model->remove_plane(cloud_vector[i]);
        std::cout << "saving segmented cloud" << std::endl;
        file_handler->save_cloud(segmented_cloud, std::to_string(i) + ".pcd", CloudType::Type::PROCESSED);
    }
}


void controller::ProcessingController::register_all_clouds(CloudType::Type cloud_type) {
    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr>> cloud_vector;
    file_handler->load_clouds(cloud_vector, cloud_type);
    Eigen::Matrix4f global_transform = Eigen::Matrix4f::Identity();
    pcl::PointCloud<pcl::PointXYZ>::Ptr result(new pcl::PointCloud<pcl::PointXYZ>), source, target;
    pcl::PointCloud<pcl::PointXYZ>::Ptr global_cloud(new pcl::PointCloud<pcl::PointXYZ>);


    for (int i = 1; i < cloud_vector.size(); i++) {

        source = cloud_vector[i - 1];
        target = cloud_vector[i];
        pcl::PointCloud<pcl::PointXYZ>::Ptr temp_registered(new pcl::PointCloud<pcl::PointXYZ>);

        Eigen::Matrix4f transform = model->icp_register_pair_clouds(source, target, temp_registered);
        Eigen::Matrix4f targetToSource = transform.inverse();

        pcl::transformPointCloud(*temp_registered, *result, global_transform);

//        file_handler->save_cloud(result, std::to_string(i), CloudType::Type::NORMAL);
        *global_cloud += *result;
        global_transform *= targetToSource;

    }

    visualize_cloud(global_cloud);
}

void controller::ProcessingController::rotate_all_clouds(CloudType::Type cloud_type) {
    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr, Eigen::aligned_allocator<pcl::PointCloud<pcl::PointXYZ>::Ptr>> cloud_vector;
    file_handler->load_clouds(cloud_vector, cloud_type);
    json info_json = file_handler->get_info_json();
    json calibration_json = file_handler->get_calibration_json();

    std::vector<float> origin = calibration_json["origin point"];
    std::vector<float> direction = calibration_json["axis direction"];
    float theta = -(float) info_json["angle"] * (M_PI / 180.0);

    pcl::PointCloud<pcl::PointXYZ>::Ptr global_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    *global_cloud = *cloud_vector[0];
    for (int i = 1; i < cloud_vector.size(); i++) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr rotated(new pcl::PointCloud<pcl::PointXYZ>);
        rotated = model->rotate_cloud_about_line(cloud_vector[i], origin, direction, theta * i);
        *global_cloud += *rotated;
    }

    viewer->simpleVis(global_cloud);
}

void controller::ProcessingController::visualize_cloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    viewer->simpleVis(cloud);
}