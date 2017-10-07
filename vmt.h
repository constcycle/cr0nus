#pragma once

#include <cstdint>
#include <cstddef>

class VMT
{
private:
	uintptr_t* vmt;
public:
	// New virtual method table
	uintptr_t** p_interface = nullptr;

	uintptr_t* original_vmt = nullptr;

	uint32_t methodCount = 0;

	VMT(void* p_interface)
	{
		this->p_interface = reinterpret_cast<uintptr_t**>(p_interface);

		size_t method_count = 0;

		while (reinterpret_cast<uintptr_t*>(*this->p_interface)[method_count])
			method_count++;

		original_vmt = *this->p_interface;

		vmt = new uintptr_t[sizeof(uintptr_t) * method_count];

		memcpy(vmt, original_vmt, sizeof(uintptr_t) * method_count);
	}

	// Hook virtual method
	void HookVM(void* method, size_t methodIndex)
	{
		vmt[methodIndex] = reinterpret_cast<uintptr_t>(method);
	}

	template<typename Fn>
	Fn GetOriginalMethod(size_t methodIndex)
	{
		return reinterpret_cast<Fn>(original_vmt[methodIndex]);
	}

	void ApplyVMT()
	{
		*this->p_interface = vmt;
	}

	void ReleaseVMT()
	{
		*this->p_interface = original_vmt;
	}
};

