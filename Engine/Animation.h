#pragma once
#include "AnimationData.h"

struct AnimationData;

class Animation : public IMemoryBlockHanlde
{
public:
	Animation();
	virtual ~Animation() override;

	void Init(const AnimationData& animData);

public:
	const AnimationData& GetAnimationData() { return _animData; }

private:
	AnimationData _animData;
};

