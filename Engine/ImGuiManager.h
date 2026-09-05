#pragma once
#include "DescriptorHeapAllocator.h"

class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	void Init();
	void Update();
	void Render();

private:
	void CreateView();

private:
	DescriptorHandle _descHandle;
};

