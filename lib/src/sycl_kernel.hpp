/**
 * @file sycl_kernel.hpp
 *
 * @author: Jonathan Ramsey
 * @date:   Oct 5, 2023
 *
*/

#ifndef SYCL_OFFLOAD_HPP_
#define SYCL_OFFLOAD_HPP_

#include "base_sycl_api.hpp"

#define Q(x) #x
#define QUOTE(x) Q(x)

const std::string branch = QUOTE(GIT_BRANCH);
const std::string build = QUOTE(GIT_COMMIT_HASH);
const std::string versionStr = branch + "." + build;

class SyclOffload : public BaseSyclClass {
public:
	SyclOffload();
	virtual ~SyclOffload();

	virtual int offload(int n_elements) override;
	virtual std::string pluginName() override;
	virtual std::string versionString() override;
};

#endif
