#pragma once

#include <iostream>
#include <string>

constexpr int DEFAULT_PORT = 8082;
constexpr int BUFFER_SIZE = 1024;

namespace Common {
	inline void error(const std::string& msg) {
		std::cerr << "Error: " << msg << std::endl;
	}
}