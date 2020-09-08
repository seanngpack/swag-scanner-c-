#ifndef SWAG_SCANNER_ICONTROLLERGUI_H
#define SWAG_SCANNER_ICONTROLLERGUI_H

#include <memory>
#include <string>
#include "IController.h"

class SwagGUI;

class IFormsPayload;

namespace controller {
    /**
     * Represents a controller for GUIs.
     */
    class IControllerGUI : public virtual IController {
    public:

        explicit IControllerGUI(std::shared_ptr<SwagGUI> gui);

        virtual ~IControllerGUI() = default;


        /**
         * This method connects gui to this controller. Must run this method whenever
         * swapping controllers for GUI. Call this method in the factory class.
         *
         */
        void setup_gui();

        /**
         * Update the controller with the given payload.
         *
         * @param payload payload from GUI.
         */
        virtual void update(const IFormsPayload &payload) = 0;

        /**
         * Write message to the GUI console.
         *
         * @param info message you want to write.
         */
        void update_console(const std::string &info);


        std::vector<std::string> get_all_scans();

        std::vector<std::string> get_all_calibrations();

    protected:
        std::shared_ptr<SwagGUI> gui;

    };

}

#endif //SWAG_SCANNER_ICONTROLLERGUI_H
