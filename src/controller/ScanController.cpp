#include "ScanController.h"

controller::ScanController::ScanController(camera::ICamera *camera,
                                           arduino::Arduino *arduino,
                                           std::shared_ptr<model::Model> model,
                                           std::shared_ptr<file::FileHandler> file_handler) :
        camera(camera), arduino(arduino), model(model), file_handler(file_handler) {}


void controller::ScanController::scan(int degs) {
    if (360 % degs != 0) {
        throw std::invalid_argument("Invalid input, scanning input must be a factor of 360");
    }

    // get current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%m-%d-%Y %H:%M:%S");
    auto str = oss.str();

    file_handler->update_info_json(str, degs, file_handler->find_latest_calibration());


    int num_rotations = 360 / degs;

    const camera::ss_intrinsics *intrin = camera->get_intrinsics();
    std::cout << "starting scanning..." << std::endl;
    for (int i = 0; i < num_rotations; i++) {
        std::string name = std::to_string(i * degs);
        const uint16_t *depth_frame = camera->get_depth_frame();
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = model->create_point_cloud(depth_frame, intrin);
        file_handler->save_cloud(cloud, name, CloudType::Type::RAW);
        arduino->rotate_table(degs);
    }
}


controller::ScanController::~ScanController() {
    delete camera;
    delete arduino;
}




