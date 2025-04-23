#pragma once
#include "memory"

namespace Utils {
	template<class T>
	struct CopyablePtr {
		std::unique_ptr<T> _ptr{};

		CopyablePtr() = default;
		CopyablePtr(const T &val) : _ptr(val) {};

		CopyablePtr(const CopyablePtr &other)
			: _ptr([&]() -> std::unique_ptr<T> {
				  if (other._ptr)
					  return std::make_unique<T>(*other._ptr);
				  else
					  return std::unique_ptr<T>();
			  }()) {}
		CopyablePtr(CopyablePtr &&other) : _ptr(std::move(other._ptr)) {}
		CopyablePtr &operator=(const CopyablePtr &other) {
			if (this != &other) {
				if (!other._ptr)
					this->_ptr.reset();
				else
					this->_ptr = std::make_unique<T>(*other._ptr);
			}
			return *this;
		}
		CopyablePtr &operator=(CopyablePtr &&other) {
			if (other._ptr) {
				this->_ptr = std::move(other._ptr);
			}
		}
	};
}// namespace Utils