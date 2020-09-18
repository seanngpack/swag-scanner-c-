#ifndef SWAG_SCANNER_PROCESSINGCONTROLLER_H
#define SWAG_SCANNER_PROCESSINGCONTROLLER_H

#include "IController.h"
#include "CloudType.h"
#include <memory>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace visual {
    class Visualizer;
}

namespace model {
    class Model;
}

namespace file {
    class ScanFileHandler;
}


namespace controller {

    /**
     * This controller handles data processing commands.
     */
    class ProcessingController : public virtual IController {
    public:
        ProcessingController(std::shared_ptr<model::Model> model,
                             std::shared_ptr<visual::Visualizer> viewer,
                             std::shared_ptr<file::ScanFileHandler> file_handler);

        /**
        * Process the data. Filters, segments, and rotates the clouds.
        */
        void run() override;


        /**
         * Crop clouds based on box boundary defined in constants.h, remove all NaN points,
         * @param cloud_type which calibration types do you want to crop.
         * @param leaf size.
         */
        void filter(const CloudType::Type &cloud_type);


        /**
         * Register all point clouds in given folder location.
         * @param folder_path the path to the scan folder.
         * @param cloud_type the type of the calibration, tells which folder to look into for clouds.
         */
        void register_all_clouds(const CloudType::Type &cloud_type);


    protected:
        std::shared_ptr<model::Model> model;
        std::shared_ptr<visual::Visualizer> viewer;
        std::shared_ptr<file::ScanFileHandler> file_handler;
    };


}

#endif //SWAG_SCANNER_PROCESSINGCONTROLLER_H
