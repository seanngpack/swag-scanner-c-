#ifndef SWAG_SCANNER_SCANCONTROLLER_H
#define SWAG_SCANNER_SCANCONTROLLER_H

#include "Model.h"
#include "Arduino.h"
#include "SR305.h"
#include "Visualizer.h"
#include "FileHandler.h"

/**
 * This controller handles data acquisition.
 */
namespace controller {
    class ScanController {
    public:
        ScanController(camera::ICamera *camera,
                       arduino::Arduino *arduino,
                       std::shared_ptr<model::Model> model,
                       std::shared_ptr<file::FileHandler> file_handler);


        /**
         * Write folders, run the scan and collect data.
         * @param degs number of degrees per rotation interval.
         */
        void scan(int degs);


        ~ScanController();

    private:
        camera::ICamera *camera;
        arduino::Arduino *arduino;
        std::shared_ptr<model::Model> model;
        std::shared_ptr<file::FileHandler> file_handler;
    };
}

#endif //SWAG_SCANNER_SCANCONTROLLER_H
