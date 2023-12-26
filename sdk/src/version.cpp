namespace tc::sdk::info
{
extern const char* const name = "teiacore_sdk";
extern const char* const version = "1.0.0";

extern const char* const project_description = "TeiaCoreSDK is a collection of reusable C++ components";
extern const char* const project_url = "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK";

extern const char* const build_type = "Debug";
extern const char* const compiler_name = "MSVC";
extern const char* const compiler_version = "19.38.33130.0";

extern const char* const cxx_flags = "/DWIN32 /D_WINDOWS /EHsc";
extern const char* const cxx_flags_debug = "/Zi /Ob0 /Od /RTC1";
extern const char* const cxx_flags_release = "/O2 /Ob2 /DNDEBUG";
extern const char* const cxx_standard = "20";

extern const char* const os_name = "Windows";
extern const char* const os_version = "10.0.22000";
extern const char* const os_processor = "AMD64";

}
