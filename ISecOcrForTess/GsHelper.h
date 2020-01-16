#pragma once
#include <string>

#include <gs/gsdll.h>
#include <gs/gsdllwin.h>


/// Ghostscript dll 帮助类
///
/// dll 动态加载帮助, 集成了 Ghostscript 一些有用的功能
/// @author zhoumanman888@126.com
/// @date 2017.8.17
class CGsHelper
{
public:
    static CGsHelper& getInstance();
    inline bool isLoadOk();
    bool loadDll(const std::string& dllURI);
    ~CGsHelper();

    void printGsRevision();

    bool Convert2Img(std::string inputFile, std::string outputFile, int firstPage, int lastPage, std::string deviceFormat = "jpeg", int width = 0, int height = 0);
private:
    CGsHelper();
    // gsdll.h
    PFN_gsdll_revision gsdll_revision;
    PFN_gsdll_init gsdll_init;
    PFN_gsdll_execute_begin gsdll_execute_begin;
    PFN_gsdll_execute_cont gsdll_execute_cont;
    PFN_gsdll_execute_end gsdll_execute_end;
    PFN_gsdll_exit gsdll_exit;
    PFN_gsdll_lock_device gsdll_lock_device;
    // iapi.h
    PFN_gsapi_revision gsapi_revision;
    PFN_gsapi_new_instance gsapi_new_instance;
    PFN_gsapi_delete_instance gsapi_delete_instance;
    PFN_gsapi_set_stdio gsapi_set_stdio;
    PFN_gsapi_set_poll gsapi_set_poll;
    PFN_gsapi_set_display_callback gsapi_set_display_callback;
    PFN_gsapi_init_with_args gsapi_init_with_args;
    PFN_gsapi_run_string_begin gsapi_run_string_begin;
    PFN_gsapi_run_string_continue gsapi_run_string_continue;
    PFN_gsapi_run_string_end gsapi_run_string_end;
    PFN_gsapi_run_string_with_length gsapi_run_string_with_length;
    PFN_gsapi_run_string gsapi_run_string;
    PFN_gsapi_run_file gsapi_run_file;
    PFN_gsapi_exit gsapi_exit;
    PFN_gsapi_set_visual_tracer gsapi_set_visual_tracer;
    // gsdllwin.h
    PFN_gsdll_copy_dib gsdll_copy_dib;
    PFN_gsdll_copy_palette gsdll_copy_palette;
    PFN_gsdll_draw gsdll_draw;
    PFN_gsdll_get_bitmap_row gsdll_get_bitmap_row;
private:
    HINSTANCE m_hdll;
    bool m_bLoadOk;
};


bool CGsHelper::isLoadOk()
{
    return m_bLoadOk;
}