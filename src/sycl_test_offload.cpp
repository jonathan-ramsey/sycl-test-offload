/**
 * @file sycl_test_offload.cpp
 *
 * @author: Jonathan Ramsey
 * @date:   Oct 6, 2023
 *
 */

#include <string>
#include <iostream>
#include <filesystem>
#include <string_view>

#include "base_sycl_api.hpp"

#include <boost/dll/import.hpp>
#include <boost/shared_ptr.hpp>

int main(int argc, char* argv[]) {

	std::string_view modulePath = "sycl_test_lib_d.dll";
	boost::filesystem::path lib_path((std::string)modulePath);

	boost::shared_ptr<BaseSyclClass> plugin;
	std::cout << "Attempting to load plugin: " << modulePath << std::endl;
	if (!std::filesystem::exists(modulePath)) {
		std::cout << "DLL not found!: " << modulePath << std::endl;
		return -1;
	}

	plugin = boost::dll::import_symbol<BaseSyclClass>(lib_path, "plugin");
	std::cout << "Loaded plugin: " << plugin->pluginName() << std::endl;

	std::cout << "Offloading time!" << std::endl;
	int result = plugin->offload();

	return 0;

}
