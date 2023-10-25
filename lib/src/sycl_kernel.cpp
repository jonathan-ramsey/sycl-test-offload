/**
 * @file sycl_kernel.cpp
 *
 * @author: Jonathan Ramsey
 * @date:   Oct 5, 2023
 *
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <chrono>

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

int SyclOffload::offload(int n_elements) {

	auto start_whole = std::chrono::high_resolution_clock::now();
	//std::vector<float> a = { 1.0, 2.0, 3.0, 4.0 };
	//std::vector<float> b = { 4.0, 3.0, 2.0, 1.0 };
	//std::vector<float> c = { 0.0, 0.0, 0.0, 0.0 };
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<float> a(n_elements);
	std::vector<float> b(n_elements);
	std::vector<float> c(n_elements);
	std::iota(a.begin(), a.end(), 1);
	std::iota(std::rbegin(b), std::rend(b), 1);
	fill(c.begin(), c.end(), 0.0);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time spent (initialization; on host): " << duration.count() << " milliseconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	try {
		cl::sycl::queue queue(cl::sycl::default_selector_v, exception_handler, cl::sycl::property::queue::enable_profiling{});

		std::cout << "Running on "
			<< queue.get_device().get_info<cl::sycl::info::device::name>()
			<< "\n";
	
		cl::sycl::buffer a_sycl(a);
		cl::sycl::buffer b_sycl(b);
		cl::sycl::buffer c_sycl(c);

		cl::sycl::range<1> num_items{a.size()};

		std::cout << "Entering queue..." << std::endl;
		cl::sycl::event evt = queue.submit([&](cl::sycl::handler& cgh) {
			std::cout << "Creating accessors." << std::endl;
			auto a_acc = a_sycl.get_access<cl::sycl::access::mode::read>(cgh);
			auto b_acc = b_sycl.get_access<cl::sycl::access::mode::read>(cgh);
			auto c_acc = c_sycl.get_access<cl::sycl::access::mode::discard_write>(cgh);

			std::cout << "Starting computation." << std::endl;
			cgh.parallel_for<my_vector_addition>( num_items,
				[=](auto i) { c_acc[i] = a_acc[i] + b_acc[i]; });

		});
		queue.wait_and_throw();
		auto delta = evt.template get_profiling_info<cl::sycl::info::event_profiling::command_end>() -
					 evt.template get_profiling_info<cl::sycl::info::event_profiling::command_start>();
		std::cout << "Exited queue." << std::endl;
		std::cout << "Time spent (in offload, w/o data transfer): " << delta*1.0e-6 << " milliseconds" << std::endl;
		
	}
	catch (const sycl::exception &e) {
		std::cout << "Exception caught: " << e.what() << std::endl;
	}
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time spent (SYCL-related, inclusive): " << duration.count() << " milliseconds" << std::endl;

	int n = n_elements;
	start = std::chrono::high_resolution_clock::now();
    std::cout << "Check results:" << std::endl;
	std::cout << "  A { " << a[0] << ", " << a[1] << ", " << a[2]
        << " ... " << a[n-3] << ", " << a[n-2] << ", " << a[n-1] << " }\n"
		<< "+ B { " << b[0] << ", " << b[1] << ", " << b[2]
		<< " ... " << b[n-3] << ", " << b[n-2] << ", " << b[n-1] << " }\n"
		<< "------------------\n"
		<< "= C { " << c[0] << ", " << c[1] << ", " << c[2]
		<< " ... " << c[n-3] << ", " << c[n-2] << ", " << c[n-1] << " }\n" << std::endl;
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time spent (printing results): " << duration.count() << " milliseconds" << std::endl;
	auto stop_whole = std::chrono::high_resolution_clock::now();
	auto duration_whole = std::chrono::duration_cast<std::chrono::milliseconds>(stop_whole - start_whole);
	std::cout << "Time spent (in DLL): " << duration_whole.count() << " milliseconds" << std::endl;

	return 0;

}

std::string SyclOffload::pluginName() { return "SyclTestPlugin"; };

std::string SyclOffload::versionString() { return versionStr; };
