#pragma once
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
	ComPtr<ID3D12DescriptorHeap> _descHeap;
};

