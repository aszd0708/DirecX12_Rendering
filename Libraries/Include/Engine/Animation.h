#pragma once
#include "AnimationData.h"
#include "AnimationInfo.h"

struct AnimationData;

class Animation : public IMemoryBlockHanlde
{
public:
	Animation();
	virtual ~Animation() override;

	void Init(const AnimationData& animData, const AnimationInfo& animInfo);

public:
	const AnimationData& GetAnimationData() { return _animData; }
	const AnimationInfo& GetAnimationInfo() { return _animInfo; }

private:
	AnimationData _animData;
	AnimationInfo _animInfo;
};

