/**
 * @file sycl_kernel.cpp
 *
 * @author: Jonathan Ramsey
 * @date:   Oct 5, 2023
 *
 */

#include <iostream>
#include <vector>

#include <CL/sycl.hpp>

#include "sycl_kernel.hpp"

class my_vector_addition;

SyclOffload::SyclOffload() { }

SyclOffload::~SyclOffload() { }

// Create an exception handler for asynchronous SYCL exceptions
static auto exception_handler = [](sycl::exception_list e_list) {
	for (auto& e : e_list) {
		std::rethrow_exception(e);
	}
};

int SyclOffload::offload() {

	std::vector<float> a = { 1.0, 2.0, 3.0, 4.0 };
	std::vector<float> b = { 4.0, 3.0, 2.0, 1.0 };
	std::vector<float> c = { 0.0, 0.0, 0.0, 0.0 };

	try {
		cl::sycl::queue queue(cl::sycl::default_selector_v, exception_handler);
		//cl::sycl::queue queue(cl::sycl::cpu_selector_v, exception_handler);


		std::cout << "Running on "
			<< queue.get_device().get_info<cl::sycl::info::device::name>()
			<< "\n";
	
		cl::sycl::buffer a_sycl(a);
		cl::sycl::buffer b_sycl(b);
		cl::sycl::buffer c_sycl(c);

		cl::sycl::range<1> num_items{a.size()};

		std::cout << "Entering queue..." << std::endl;
		queue.submit([&](cl::sycl::handler& cgh) {
			std::cout << "Creating accessors." << std::endl;
			auto a_acc = a_sycl.get_access<cl::sycl::access::mode::read>(cgh);
			auto b_acc = b_sycl.get_access<cl::sycl::access::mode::read>(cgh);
			auto c_acc = c_sycl.get_access<cl::sycl::access::mode::discard_write>(cgh);

			std::cout << "Starting computation." << std::endl;
			cgh.parallel_for<my_vector_addition>( num_items,
				[=](auto i) { c_acc[i] = a_acc[i] + b_acc[i]; });

			std::cout << "Finished computation." << std::endl;
		});
		queue.wait_and_throw();
		std::cout << "Exited queue." << std::endl;
	}
	catch (const sycl::exception &e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}

    std::cout << "Check results:" << std::endl;
	std::cout << "  A { " << a[0] << ", " << a[1] << ", " << a[2] << ", "
		<< a[3] << " }\n"
		<< "+ B { " << b[0] << ", " << b[1] << ", " << b[2] << ", "
		<< b[3] << " }\n"
		<< "------------------\n"
		<< "= C { " << c[0] << ", " << c[1] << ", " << c[2] << ", "
		<< c[3] << " }" << std::endl;

	return 0;

}

std::string SyclOffload::pluginName() { return "SyclTestPlugin"; };

std::string SyclOffload::versionString() { return versionStr; };
