//
// Created by MorningTZH on 2023/2/17.
//

#ifndef MORNINGRING_SYSTEM_HPP
#define MORNINGRING_SYSTEM_HPP

#define DEVICE_CONNECT_PORT 18650

class System {

};

int StartDetectDevice();

System *system() {
    static class System *i = nullptr;
    if (!i) {
        i = new class System;
    }

    return i;
}

#endif //MORNINGRING_SYSTEM_HPP
