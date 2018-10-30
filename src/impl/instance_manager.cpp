/**
 * InstanceManager.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "instance_manager.h"

namespace raytracer::impl {
    std::vector<std::shared_ptr<InstanceManager>> InstanceManager::ptrs;

    InstanceManager::InstanceManager(dummy) {}
}