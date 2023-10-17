/**
 * @file sycl_test_plugin.cpp
 *
 * @author: jonathan.ramsey
 * @date:   Oct 4, 2023
 *
 */

#include <boost/dll.hpp>
#include "sycl_kernel.hpp"

using namespace boost;

// Exporting `my_namespace::plugin` variable with alias name `plugin`
extern "C" BOOST_SYMBOL_EXPORT SyclOffload plugin;
SyclOffload plugin;

