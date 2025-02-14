#include "Manager/Manager.h"
#include <memory>

int main(int argc, char *argv[]) {
    std::unique_ptr<Manager> manager = std::make_unique<Manager>(argc, argv);
    return manager->run();
}
