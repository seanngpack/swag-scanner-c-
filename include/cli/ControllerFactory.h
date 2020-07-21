#ifndef SWAG_SCANNER_CONTROLLERFACTORY_H
#define SWAG_SCANNER_CONTROLLERFACTORY_H


#include "IController.h"
#include <boost/program_options.hpp>

namespace cli {
    class ControllerFactory {
    public:

        ControllerFactory() = default;

        std::unique_ptr<controller::IController> create(boost::program_options::variables_map vm);


    private:

        /**
         * Creates a new scanning controller. if -name is not passed, it will create
         * a new folder.
         * @param vm
         * @return
         */
        std::unique_ptr<controller::IController>
        create_scan_controller(boost::program_options::variables_map vm);

        /**
         * Creates a new calibration controller. If -name is not passed then it will
         * use the latest calibration folder.
         * @param vm
         * @return
         */
        std::unique_ptr<controller::IController>
        create_calibrate_controller(boost::program_options::variables_map vm);

        /**
         * Create a processing controller. If -name is not passed then it will use the latest scan according
         * to the info.json file.
         * @param vm
         * @return
         */
        std::unique_ptr<controller::IController>
        create_processing_controller(boost::program_options::variables_map vm);

        /**
         * Create a filter testing controller.
         * @param vm
         * @return
         */
        std::unique_ptr<controller::IController>
        create_filter_testing_controller(boost::program_options::variables_map vm);

        /**
         * Create a move controller.
         * @param vm
         * @return
         */
        std::unique_ptr<controller::IController>
        create_move_controller(boost::program_options::variables_map vm);
    };
}

#endif //SWAG_SCANNER_CONTROLLERFACTORY_H
