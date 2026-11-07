#include "pch.h"
#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Init(const AnimationData& animData, const AnimationInfo& animInfo)
{
	_animData = animData;
	_animInfo = animInfo;
}
