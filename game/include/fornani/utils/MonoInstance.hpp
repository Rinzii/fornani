#pragma once

#include <stdexcept>

namespace fornani::core
{
template <typename Type>
class MonoInstance
{
public:
	MonoInstance()
	{
		if (s_instance != nullptr) { throw std::runtime_error{"Instance already exists"}; }
		s_instance = static_cast<Type *>(this);
	}

	~MonoInstance() { s_instance = nullptr; }

	MonoInstance(MonoInstance const &)			   = delete;
	MonoInstance(MonoInstance &&)				   = delete;
	MonoInstance & operator=(MonoInstance const &) = delete;
	MonoInstance & operator=(MonoInstance &&)	   = delete;

	static Type & get()
	{
		if (s_instance == nullptr) { throw std::runtime_error{"Instance does not exist"}; }
		return *s_instance;
	}

	static Type & self() { return get(); }

	static bool exists() { return s_instance != nullptr; }

protected:
	inline static Type * s_instance{};
};
} // namespace gen
