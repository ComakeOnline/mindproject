#pragma once
#include <cstring>
#include <string>
#include <mutex>
#include "logger.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"
#include "rapidjson/error/en.h"

#include "timing.hpp"
#include "displayout.hpp"

#define CONFIG_FILE_NAME "nvr_cfg.json"
#define CONFIG_PRODUCT_NAME "nvr_product.json"
#define CONFIG_FILE_MAXSIZE 0xFFFF

#define CONVERT_X(x) config::Config::GetInstance()->ConvertX(x)
#define CONVERT_Y(x) config::Config::GetInstance()->ConvertY(x)

namespace config
{

//随产品信息固定的参数
struct ProductConfig_st
{
    //最大支持IPC数量
    uint32_t MAX_IPC_NUMBER = 4;
    //最大支持Video Decoder数量
    uint32_t MAX_VIDEO_DCODER_NUMBER = 4;
    //最大支持Video输入分辨率
    uint16_t MAX_INPUT_VIDEO_WIDTH = 1920;
    uint16_t MAX_INPUT_VIDEO_HEIGHT = 1080;
};

//可实时调整的参数
struct Config_st
{
    //分辨率修改以下参数
    ETiming eTiming = E_CONFIG_TIMING_1024_600_LCD;
    uint16_t resolution_width = 1024;
    uint16_t resolution_height = 600;
    //画面布局
    ELayout eLayout = E_LAYOUT_4PIC_SAMPLE;
};

using DefValue = rapidjson::Value;

class Config
{
public:
    //--legacy
    //可实时调整的参数--legacy
    Config_st m_allcfg;
    //随产品信息固定的参数--legacy
    ProductConfig_st m_productCfg;
    //--end
    //产品参数(不可更改)
    rapidjson::Document m_product_json;
    //配置(保存在配置文件，可更改)
    rapidjson::Document m_config_json;
    //缓存配置(重启程序清空，可更改)
    rapidjson::Document m_cache_json;
private:
    std::mutex m_mtx;
private:
    Config(/* args */) {
        ReadToDom(CONFIG_PRODUCT_NAME,m_product_json);
        ReadToDom(CONFIG_FILE_NAME,m_config_json);
    }
    ~Config() {}
public:
    static Config* GetInstance(){
        static Config ins;
        return &ins;
    }
    Config_st GetAllConfig(){
        return m_allcfg;
    }

    //read json to dom
    void ReadToDom(const char* filename, rapidjson::Document &_out_dom)
    {
        FILE *fp = fopen(filename,"rb");
        if(!fp)
        {
            LOGGER_ERROR("Can not open file:%s",filename);
            return;
        }
        char *buff = new char[CONFIG_FILE_MAXSIZE];
        memset(buff,0,CONFIG_FILE_MAXSIZE);
        auto ret = fread(buff,1,CONFIG_FILE_MAXSIZE,fp);
        if(ret <= 0)
        {
            LOGGER_ERROR("Can not read file:%s",filename);
            return;
        }
        _out_dom.Parse(buff);
        delete[] buff;
        fclose(fp);
        if(_out_dom.HasParseError())
        {
            LOGGER_ERROR("CONFIG %s Parse Error, ERROR_CODE=%u,%s",filename,_out_dom.GetErrorOffset(),rapidjson::GetParseError_En(_out_dom.GetParseError()));
            return;
        }
        return;
    }

    //save config_json to file
    bool SaveCfg()
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        FILE *fp = fopen(CONFIG_FILE_NAME,"wb");
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        m_config_json.Accept(writer);
        auto ret = fwrite(buffer.GetString(),1,buffer.GetSize(),fp);
        fflush(fp);
        fclose(fp);
        if(ret == buffer.GetSize())
        {
            return true;
        }
        return false;
    }

    template<class _Type, typename CharType, size_t N>
    _Type GetCfg(const CharType(&_uri)[N], _Type _default)
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        rapidjson::Value& hello = rapidjson::Pointer(_uri).GetWithDefault(m_config_json, _default);
        //rapidjson::Value& hello = rapidjson::GetValueByPointerWithDefault(m_config_json,_point,_default);
        return hello.Get<_Type>();
    }
    template<class _Type, typename CharType, size_t N>
    _Type GetProduct(const CharType(&_uri)[N], _Type _default)
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        rapidjson::Value& hello = rapidjson::Pointer(_uri).GetWithDefault(m_product_json, _default);
        return hello.Get<_Type>();
    }
    template<class _Type, typename CharType, size_t N>
    _Type GetCache(const CharType(&_uri)[N], _Type _default)
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        rapidjson::Value& hello = rapidjson::Pointer(_uri).GetWithDefault(m_cache_json, _default);
        return hello.Get<_Type>();
    }

    template<class _Type, typename CharType, size_t N>
    void SetCfg(const CharType(&_uri)[N], _Type _Value)
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        rapidjson::Pointer(_uri).Set(m_config_json, _Value);
    }

    template<class _Type, typename CharType, size_t N>
    void SetCache(const CharType(&_uri)[N], _Type _Value)
    {
        std::lock_guard<std::mutex> __lock_32423yyy6y6{m_mtx};
        rapidjson::Pointer(_uri).Set(m_cache_json, _Value);
    }
    


    //--legacy

    void SetAllConfig(const Config_st & _stcfg){
        memcpy(&m_allcfg, &_stcfg, sizeof(m_allcfg));
    }

    
    //获取画面布局的详细参数
    std::vector<LayerWnd> ConvertLayerCfg(){
        //默认设置1080p参数 然后等比转换为其他分辨率
        LayerWnd lw;
        LayerWnd lw2;
        std::vector<LayerWnd> verLw;
        auto fScale = [this](LayerWnd &lw){
            //转换为当前分辨率
            lw.vsize_width *= m_allcfg.resolution_width/1920.0;
            lw.vsize_height *= m_allcfg.resolution_height/1080.0;
            lw.x *= m_allcfg.resolution_width/1920.0;
            lw.y *= m_allcfg.resolution_height/1080.0;
            lw.width *= m_allcfg.resolution_width/1920.0;
            lw.height *= m_allcfg.resolution_height/1080.0;
            for (auto &&i : lw.vecPort)
            {
                i.x *= m_allcfg.resolution_width/1920.0;
                i.y *= m_allcfg.resolution_height/1080.0;
                i.width *= m_allcfg.resolution_width/1920.0;
                i.hetght *= m_allcfg.resolution_height/1080.0;
            }
        };
        switch (m_allcfg.eLayout)
        {
        case E_LAYOUT_FULLSCREEN:
            lw.vsize_width = 1920;
            lw.vsize_height = 1080;
            lw.x = 0;
            lw.y = 0;
            lw.width = 1920;
            lw.height = 1080;
            lw.vecPort.push_back({0,0,1920,1080});
            fScale(lw);
            verLw.push_back(lw);
            break;
        case E_LAYOUT_4PIC_SAMPLE:
            lw.vsize_width = 1920;
            lw.vsize_height = 1080;
            lw.x = 0;
            lw.y = 0;
            lw.width = 1920;
            lw.height = 1080;
            lw.vecPort.push_back({0,0,960,540});
            lw.vecPort.push_back({960,0,960,540});
            lw.vecPort.push_back({0,540,960,540});
            lw.vecPort.push_back({960,540,960,540});
            fScale(lw);
            verLw.push_back(lw);
            break;
        
        default:
            std::runtime_error("unkown display layout!");
            break;
        }
        
        return verLw;
    }

    //转换各种分辨率的比例
    int16_t ConvertX(int16_t _x){
        _x *= m_allcfg.resolution_width/1920.0;
        return _x;
    }
    int16_t ConvertY(int16_t _y){
        _y *= m_allcfg.resolution_height/1080.0;
        return _y;
    }
};

}