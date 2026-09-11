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
#define COMMAND_SUB_ALLOCATOR	GRAPHICS->GetSubAllocator()
#define COMMAND_SUB_LIST		GRAPHICS->GetSubList()
#define INPUT				GET_SINGLE(InputManager)
#define TIME				GET_SINGLE(TimeManager)
#define DT					TIME->GetDeltaTime()
#define RESOURCES			GET_SINGLE(ResourceManager)
#define INSTANCING			GET_SINGLE(InstancingManager)
#define GUI					GET_SINGLE(ImGuiManager)
#define DESC_POOL			GET_SINGLE(DescHeapAllocatorManager)
#define CPU_MEM_POOL		GET_SINGLE(CpuMemoryPoolManager)
#define GPU_MEM_POOL		GET_SINGLE(GpuMemoryPoolManager)
//#define SCENE		GET_SINGLE(SceneManager)
//#define CUR_SCENE	SCENE->GetCurrentScene()

#define SCREEN_WIDTH	GAME->GetGameDesc().width
#define SCREEN_HEIGHT	GAME->GetGameDesc().height

#define GET_DESC(descType)  descType = {}

const UINT32 SWAP_CHAIN_BUFFER_COUNT = 2;

// 64KB (64 * 1024)
const UINT32 GPU_HEAP_MINIMUM_SIZE = 65536;

const static UINT64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
const static UINT64 FNV_PRIME = 1099511628211;
