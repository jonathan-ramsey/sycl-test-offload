/**
 * @file base_sycl_api.hpp
 *
 * @author: Jonathan Ramsey
 * @date:   Oct 5, 2023
 *
 */

#ifndef BASE_SYCL_API_HPP_
#define BASE_SYCL_API_HPP_

#include <string>

class BaseSyclClass {
  public:
      virtual std::string pluginName() = 0;
      virtual std::string versionString() = 0;
      virtual int offload(int n_elements) = 0;
  };

#endif

