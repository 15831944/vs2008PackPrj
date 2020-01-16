#include <Windows.h>
#include "GsHelper.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <array>
#include <cstring>
using namespace std;
CGsHelper& CGsHelper::getInstance()
{
    static CGsHelper gs;

    return gs;
}

CGsHelper::CGsHelper()
    : m_hdll(nullptr), m_bLoadOk(false)
{
    
}


CGsHelper::~CGsHelper()
{
    if (m_hdll != nullptr)
        FreeLibrary(m_hdll);
}

bool CGsHelper::loadDll(const std::string& dllURI)
{
    if (m_hdll == nullptr)
        m_hdll = LoadLibrary(dllURI.data());
    if (!m_hdll)
        return false;
    
    gsdll_revision = (PFN_gsdll_revision)GetProcAddress(m_hdll, "gsdll_revision");
    assert(gsdll_revision);
    gsdll_init = (PFN_gsdll_init)GetProcAddress(m_hdll, "gsdll_init");
    assert(gsdll_init);
    gsdll_execute_begin = (PFN_gsdll_execute_begin)GetProcAddress(m_hdll, "gsdll_execute_begin");
    assert(gsdll_execute_begin);
    gsdll_execute_cont = (PFN_gsdll_execute_cont)GetProcAddress(m_hdll, "gsdll_execute_cont");
    assert(gsdll_execute_cont);
    gsdll_execute_end = (PFN_gsdll_execute_end)GetProcAddress(m_hdll, "gsdll_execute_end");
    assert(gsdll_execute_end);
    gsdll_exit = (PFN_gsdll_exit)GetProcAddress(m_hdll, "gsdll_exit");
    assert(gsdll_exit);
    gsdll_lock_device = (PFN_gsdll_lock_device)GetProcAddress(m_hdll, "gsdll_lock_device");
    assert(gsdll_lock_device);


    gsapi_revision = (PFN_gsapi_revision)GetProcAddress(m_hdll, "gsapi_revision");
    assert(gsapi_revision);
    gsapi_new_instance = (PFN_gsapi_new_instance)GetProcAddress(m_hdll, "gsapi_new_instance");
    assert(gsapi_new_instance);
    gsapi_delete_instance = (PFN_gsapi_delete_instance)GetProcAddress(m_hdll, "gsapi_delete_instance");
    assert(gsapi_delete_instance);
    gsapi_set_stdio = (PFN_gsapi_set_stdio)GetProcAddress(m_hdll, "gsapi_set_stdio");
    assert(gsapi_set_stdio);
    gsapi_set_poll = (PFN_gsapi_set_poll)GetProcAddress(m_hdll, "gsapi_set_poll");
    assert(gsapi_set_poll);
    gsapi_set_display_callback = (PFN_gsapi_set_display_callback)GetProcAddress(m_hdll, "gsapi_set_display_callback");
    assert(gsapi_set_display_callback);
    gsapi_init_with_args = (PFN_gsapi_init_with_args)GetProcAddress(m_hdll, "gsapi_init_with_args");
    assert(gsapi_init_with_args);
    gsapi_run_string_begin = (PFN_gsapi_run_string_begin)GetProcAddress(m_hdll, "gsapi_run_string_begin");
    assert(gsapi_run_string_begin);
    gsapi_run_string_continue = (PFN_gsapi_run_string_continue)GetProcAddress(m_hdll, "gsapi_run_string_continue");
    assert(gsapi_run_string_continue);
    gsapi_run_string_end = (PFN_gsapi_run_string_end)GetProcAddress(m_hdll, "gsapi_run_string_end");
    assert(gsapi_run_string_end);
    gsapi_run_string_with_length = (PFN_gsapi_run_string_with_length)GetProcAddress(m_hdll, "gsapi_run_string_with_length");
    assert(gsapi_run_string_with_length);
    gsapi_run_string = (PFN_gsapi_run_string)GetProcAddress(m_hdll, "gsapi_run_string");
    assert(gsapi_run_string);
    gsapi_run_file = (PFN_gsapi_run_file)GetProcAddress(m_hdll, "gsdll_revision");
    assert(gsapi_run_file);
    gsapi_exit = (PFN_gsapi_exit)GetProcAddress(m_hdll, "gsapi_exit");
    assert(gsapi_exit);
    gsapi_set_visual_tracer = (PFN_gsapi_set_visual_tracer)GetProcAddress(m_hdll, "gsapi_set_visual_tracer");
    assert(gsapi_set_visual_tracer);

    gsdll_copy_dib = (PFN_gsdll_copy_dib)GetProcAddress(m_hdll, "gsdll_copy_dib");;
    assert(gsdll_copy_dib);
    gsdll_copy_palette = (PFN_gsdll_copy_palette)GetProcAddress(m_hdll, "gsdll_copy_palette");;
    assert(gsdll_copy_palette);
    gsdll_draw = (PFN_gsdll_draw)GetProcAddress(m_hdll, "gsdll_draw");;
    assert(gsdll_draw);
    gsdll_get_bitmap_row = (PFN_gsdll_get_bitmap_row)GetProcAddress(m_hdll, "gsdll_get_bitmap_row");;
    assert(gsdll_get_bitmap_row);

    m_bLoadOk = true;
    return true;
}

void CGsHelper::printGsRevision()
{
    const char* product;
    const char* copyright;
    long revision;
    long revisionDate;
    gsdll_revision(&product, &copyright, &revision, &revisionDate);

    cout << "product : " << product << ", copy right: " << copyright << ", revision = " << revision << ", revisionDate = " << revisionDate << endl;
}


bool CGsHelper::Convert2Img(std::string inputFile, std::string outputFile, int firstPage, int lastPage, std::string deviceFormat, int width, int height)
{
    // http://blog.csdn.net/ljsspace/article/details/6530046
    if (inputFile.empty() || outputFile.empty())
        return false;


    gs_main_instance *ins;
    int intReturn = gsapi_new_instance(&ins, nullptr);
    if (intReturn != 0)
        return false;

    string _sDeviceFormat = deviceFormat;
    int _iWidth = width;
    int _iHeight = height;
    int _iResolutionX = 300;
    int _iResolutionY = 0;
    int _iJPEGQuality = 95;
    bool _bFitPage = true;

    vector<string> lstExtraArgs;
    lstExtraArgs.push_back("pdf2img");//this parameter have little real use  
    lstExtraArgs.push_back("-dNOPAUSE");// Do not prompt and pause for each page
    lstExtraArgs.push_back("-dBATCH");//Keep gs from going into interactive mode
    lstExtraArgs.push_back("-dPARANOIDSAFER");//-dSAFER, Run this command in safe mode
    lstExtraArgs.push_back("-sDEVICE=" + _sDeviceFormat);//what kind of export format i should provide  
    lstExtraArgs.push_back("-q");
    lstExtraArgs.push_back("-dQUIET");
    lstExtraArgs.push_back("-dNOPROMPT");// Disable prompts for user interaction
    lstExtraArgs.push_back("-dMaxBitmap=500000000");// Set high for better performance
    lstExtraArgs.push_back("-dFirstPage=" + std::to_string(firstPage));
    lstExtraArgs.push_back("-dLastPage=" + std::to_string(lastPage));
    // Configure the output anti-aliasing, resolution, etc
    lstExtraArgs.push_back("-dAlignToPixels=0");
    lstExtraArgs.push_back("-dGridFitTT=0");
    lstExtraArgs.push_back("-dTextAlphaBits=4");
    lstExtraArgs.push_back("-dGraphicsAlphaBits=4");
    if (_sDeviceFormat == "jpeg" && _iJPEGQuality > 0 && _iJPEGQuality < 101)
        lstExtraArgs.push_back("-dJPEGQ=" + std::to_string(_iJPEGQuality));
    if (_iWidth > 0 && _iHeight > 0)
        lstExtraArgs.push_back("-g" + std::to_string(_iWidth) + "x" + std::to_string(_iHeight));
    if (_bFitPage)
        lstExtraArgs.push_back("-dPDFFitPage");
    if (_iResolutionX > 0)
    {
        if (_iResolutionY > 0)
            lstExtraArgs.push_back("-r" + std::to_string(_iResolutionX) + "x" + std::to_string(_iResolutionY));
        else
            lstExtraArgs.push_back("-r" + std::to_string(_iResolutionX));
    }
    lstExtraArgs.push_back("-sOutputFile=" + outputFile);
    lstExtraArgs.push_back(inputFile);

    int argc = lstExtraArgs.size();
    char** args = new char*[argc];
    for (int i = 0; i < argc; i++)
    {
        args[i] = new char[lstExtraArgs[i].size()];
        std::strcpy(args[i], lstExtraArgs[i].data());
    }

    //for (int i = 0; i < argc; ++i)
    //    cout << args[i] << endl;

    intReturn = gsapi_init_with_args(ins, argc, args);
    if (intReturn != 0)
        return false;
    intReturn = gsapi_exit(ins);
    if (intReturn != 0)
        return false;
    gsapi_delete_instance(ins);
    if (intReturn != 0)
        return false;

    for (int i = 0; i < argc; ++i)
        delete[] args[i];
    delete[] args;

    return true;
}