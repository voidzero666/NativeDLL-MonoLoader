// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


#include <thread>
#include <chrono>

#include "mem_utils.hpp"
#include "game_structs.hpp"

#include <mono/jit/jit.h>

#define ASSEMBLY_PATH "cheat.dll"
#define PAYLOAD_NAMESPACE "CheatNamespace"
#define PAYLOAD_CLASS "Loaderclass"
#define PAYLOAD_MAIN  "Loaderfunction"
#define MONO_DLL L"mono-2.0-bdwgc.dll"


#include <mono/metadata/metadata.h>
typedef unsigned __int64 QWORD, * PQWORD;
using namespace std;


#define Mono(x) GetProcAddress(GetModuleHandle("mono-2.0-bdwgc.dll"), x)

typedef MonoDomain* (__cdecl* getRootDomain)();
typedef MonoClass* (__cdecl* getClassFromName)(MonoImage* image, PCHAR Namespace, PCHAR Name);
typedef MonoMethod* (__cdecl* getMethodFromName)(MonoClass* Class, PCHAR name, DWORD param_count);
typedef MonoDomain* (__cdecl* getDomain)();
typedef PVOID(__cdecl* threadAttach)(MonoDomain* domain);
typedef PVOID(__cdecl* threadDetach)(MonoDomain* domain);
typedef PVOID(__cdecl* runtimeInvoke)(MonoMethod* method, PVOID, PVOID, PVOID);
typedef MonoAssembly* (__cdecl* domainAssemblyOpen)(MonoDomain* domain, PCHAR file);
typedef void(__cdecl* securitySetMode)(DWORD Mode);
typedef MonoImage* (__cdecl* imageOpenFromDataFull)(char* data, uint32_t data_len, mono_bool need_copy, MonoImageOpenStatus* status, mono_bool refonly);
typedef MonoAssembly* (__cdecl* assemblyLoadFromFull)(MonoImage* image, const char* fname, MonoImageOpenStatus* status, mono_bool refonly);
typedef MonoImage* (__cdecl* assemblyGetImage) (MonoAssembly* assembly);

typedef MonoAssembly* (__cdecl* t_mono_assembly_open)(const char*, MonoImageOpenStatus*);
t_mono_assembly_open fnAssemblyOpen;
typedef MonoImage* (__cdecl* t_mono_assembly_get_image)(MonoAssembly*);
t_mono_assembly_get_image fnAssemblyGetImage;

typedef  MonoDomain* (__cdecl* t_mono_get_root_domain)(void);
t_mono_get_root_domain fnGetRootDomain;


typedef void (*mono_security_set_mode)(DWORD mode);
mono_security_set_mode do_mono_security_set_mode;

assemblyGetImage passemblyGetImage;
assemblyLoadFromFull passemblyLoadFromFull;
getRootDomain pgetRootDomain;
getClassFromName pgetClassFromName;
getMethodFromName pgetMethodFromName;
getDomain pgetDomain;
threadAttach pthreadAttach;
threadDetach pthreadDetach;
runtimeInvoke pruntimeInvoke;
domainAssemblyOpen pdomainAssemblyOpen;
securitySetMode psecuritySetMode;
imageOpenFromDataFull pimageOpenFromDataFull;

MonoAssembly* assembly;

typedef MonoClass* (__cdecl* t_mono_class_from_name)(MonoImage*, const char*, const char*);
t_mono_class_from_name fnClassFromName;
typedef MonoMethod* (__cdecl* t_mono_class_get_method_from_name)(MonoClass*, const char*, int);
t_mono_class_get_method_from_name fnMethodFromName;
typedef MonoObject* (__cdecl* t_mono_runtime_invoke)(MonoMethod*, void*, void**, MonoObject**);
t_mono_runtime_invoke fnRuntimeInvoke;


typedef const char* (__cdecl* t_mono_assembly_getrootdir)(void);
t_mono_assembly_getrootdir fnGetRootDir;

typedef bool(__cdecl* mono_assembly_close_except_image_pools)(MonoAssembly* assembly);
mono_assembly_close_except_image_pools fnmono_assembly_close_except_image_pools;

typedef MonoAssembly* (__cdecl* mono_assembly_loaded)(MonoAssemblyName* aname);
mono_assembly_loaded fnmono_assembly_loaded;

typedef MonoAssemblyName* (__cdecl* mono_assembly_get_name_internal)(MonoAssembly* assembly);
mono_assembly_get_name_internal fnmono_assembly_get_name_internal;



DWORD WINAPI CheatEntryPoint(LPVOID PARAMS) {
{

	while ((GetModuleHandle("mono-2.0-bdwgc.dll")) == NULL) Sleep(10);

	fnmono_assembly_close_except_image_pools = (mono_assembly_close_except_image_pools)Mono("mono_assembly_close_except_image_pools");
	fnmono_assembly_loaded = (mono_assembly_loaded)Mono("mono_assembly_loaded");

	passemblyGetImage = (assemblyGetImage)Mono("mono_assembly_get_image");
	fnGetRootDomain = (t_mono_get_root_domain)Mono("mono_get_root_domain");
	pgetClassFromName = (getClassFromName)Mono("mono_class_from_name");
	pgetMethodFromName = (getMethodFromName)Mono("mono_class_get_method_from_name");
	pgetDomain = (getDomain)Mono("mono_domain_get");
	pthreadAttach = (threadAttach)Mono("mono_thread_attach");
	pthreadDetach = (threadDetach)Mono("mono_thread_detach");
	pruntimeInvoke = (runtimeInvoke)Mono("mono_runtime_invoke");
	pdomainAssemblyOpen = (domainAssemblyOpen)Mono("mono_domain_assembly_open");
	psecuritySetMode = (securitySetMode)Mono("mono_security_set_mode");
	pimageOpenFromDataFull = (imageOpenFromDataFull)Mono("mono_image_open_from_data_full");
	passemblyLoadFromFull = (assemblyLoadFromFull)Mono("mono_assembly_load_from_full");
	do_mono_security_set_mode = (mono_security_set_mode)Mono("mono_security_set_mode");

	fnAssemblyOpen = (t_mono_assembly_open)Mono("mono_assembly_open");
	fnAssemblyGetImage = (t_mono_assembly_get_image)Mono("mono_assembly_get_image");


	fnClassFromName = (t_mono_class_from_name)Mono("mono_class_from_name");
	fnMethodFromName = (t_mono_class_get_method_from_name)Mono("mono_class_get_method_from_name");
	fnRuntimeInvoke = (t_mono_runtime_invoke)Mono("mono_runtime_invoke");


	fnGetRootDir = (t_mono_assembly_getrootdir)Mono("mono_assembly_getrootdir");

	pthreadAttach(fnGetRootDomain());

	mono_security_set_mode(NULL);

	MonoImageOpenStatus Status;

	std::string assemblyDir;

	assemblyDir.append("c:\\service\\");
	assemblyDir.append(ASSEMBLY_PATH);

	assembly = fnAssemblyOpen(assemblyDir.c_str(), NULL);

	MonoImage* rawImage = fnAssemblyGetImage(assembly);

	MonoClass* imageClass = fnClassFromName(rawImage, PAYLOAD_NAMESPACE, PAYLOAD_CLASS);
	MonoMethod* imageMethod = fnMethodFromName(imageClass, PAYLOAD_MAIN, 0);

	fnRuntimeInvoke(imageMethod, NULL, NULL, NULL);

	return 0;

}


BOOL APIENTRY DllMain(HMODULE hMOD, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		AllocConsole();
		freopen("conin$", "r+t", stdin);
		freopen("conout$", "w+t", stdout);
		freopen("conout$", "w+t", stderr);
		CreateThread(0, 0, &CheatEntryPoint, 0, 0, 0);
	};
	}
	return TRUE;
}

