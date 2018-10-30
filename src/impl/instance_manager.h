/**
 * instance_manager.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <vector>

namespace raytracer::impl {
    /**
     * This is a bit of a ghastly hack of class that manages a static list of shared_ptrs so that classes can
     * inherit from InstanceManager and then use factory methods to create shared_ptrs of themselves and register
     * them. Since calls to std::make_shared require public classes, we include a private dummy struct here that
     * constructors require so that they can be public but uninstantiable outside of the factory method.
     *
     * This is also so that they can inherit from std::enable_shared_from_this, and then create shared_ptrs with
     * shared_from_this.
     *
     * For an example of this, see Shape.
     */
    class InstanceManager {
    private:
        static std::vector<std::shared_ptr<InstanceManager>> ptrs;

    protected:
        struct dummy {};

        template<typename T>
        static void registerInstance(std::shared_ptr<T> &ptr) {
            ptrs.emplace_back(std::dynamic_pointer_cast<InstanceManager>(ptr));
        }

    public:
        explicit InstanceManager(dummy);
    };
}


