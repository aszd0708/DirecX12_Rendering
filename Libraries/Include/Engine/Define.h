#pragma once

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}										\

#define DECLARE_COMPONENT_TYPE(type) public: static eComponentType GetType() { return type; }

#define GET_SINGLE(classname)	classname::GetInstance()

#define CHECK(p)			assert(SUCCEEDED(p))
#define GAME				GET_SINGLE(Game)		
#define GRAPHICS			GET_SINGLE(Graphics)
#define DEVICE				GRAPHICS->GetDevice()
#define FACTORY				GRAPHICS->GetFactory()
#define COMMAND_QUEUE		GRAPHICS->GetQueue()
#define COMMAND_ALLOCATOR	GRAPHICS->GetAllocator()
#define COMMAND_LIST		GRAPHICS->GetList()
#define INPUT				GET_SINGLE(InputManager)
#define TIME				GET_SINGLE(TimeManager)
#define DT					TIME->GetDeltaTime()
#define RESOURCES			GET_SINGLE(ResourceManager)
#define INSTANCING			GET_SINGLE(InstancingManager)
#define GUI					GET_SINGLE(ImGuiManager)
//#define SCENE		GET_SINGLE(SceneManager)
//#define CUR_SCENE	SCENE->GetCurrentScene()

#define SCREEN_WIDTH	GAME->GetGameDesc().width
#define SCREEN_HEIGHT	GAME->GetGameDesc().height

#define GET_DESC(descType)  descType = {}

const uint32 SWAP_CHAIN_BUFFER_COUNT = 2;