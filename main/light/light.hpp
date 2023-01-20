//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_LIGHT_HPP
#define MORNINGRING_LIGHT_HPP

#include <string>
#include <map>
#include <list>
#include "restful.hpp"

namespace light {

    using ModeConfig = std::map<std::string, int>;

    struct Mode {
        std::string name;
        ModeConfig config;
    };

    class manager {
    private:
        std::map<std::string, Mode> modeList;
        std::string mode;

    public:
        manager() {

        }

        std::list<std::string> GetModeList();
        bool SetMode(std::string name);
        void GetModeConfig(std::string name);
        void SetModeConfig(std::string name);

        void RegisterMode();
    };
}





#endif //MORNINGRING_LIGHT_HPP
