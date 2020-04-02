#pragma once

#include "deviceSDK.h"
#include "deviceCommonDef.h"
#include "deviceStructDefine.h"
#include "xiaoweiSDK.h"
#include "xiaoweiAudioType.h"
#include "voipSDK.h"

#ifndef _OUTPUT_PARAM_
#define _OUTPUT_PARAM_
#else
static_assert(false);
#endif

using namespace Platform;
using namespace Windows::Foundation::Collections;
namespace sdkrtv2
{
    public enum class TXCA_RUN_MODE_CS
    {
        run_mode_default = 0,  // 接入正式环境
        run_mode_debug = 1,  // 接入正式环境,debug
        run_mode_test_env = 9, // 登录到测试环境
    };

    public enum class TXCA_WAKEUP_PROFILE_CS {
        txca_wakeup_profile_far = 0,  // 远场
        txca_wakeup_profile_near = 1, // 近场，例如遥控器
    };

    public enum class TXCA_WAKEUP_TYPE_CS {
        txca_wakeup_type_local = 0,
        txca_wakeup_type_cloud = 1, // deprecated
        txca_wakeup_type_local_with_text = 2,
        txca_wakeup_type_free = 3,
    };

    public enum class TXCA_PLAYLIST_ACTION_CS {
        txca_playlist_replace_all = 0,     // 中断当前播放，替换列表
        txca_playlist_enqueue_front = 1,   // 拼接到列表队头
        txca_playlist_enqueue_back = 2,    // 拼接到列表队尾
        txca_playlist_replace_enqueue = 3, // 不中断当前播放的资源，替换列表的详情
        txca_playlist_update_enqueue = 4,  // 不中断播放，更新列表中某些播放资源的url和quality字段信息
        txca_playlist_remove = 6, // remove resources in playlist
    };

    public enum class TXCA_RESOURCE_FORMAT_CS {
        txca_resource_url = 0,  //url资源
        txca_resource_text = 1, //纯文本(无TTS)
        txca_resource_tts_id = 2,  //TTS,推送类型，废弃
        txca_resource_tts_url = 3, //TTS URL
        txca_resource_location = 4,
        txca_resource_command = 5, //指令类型
        txca_resource_intent = 6,  //语义类型
        txca_resource_remind = 7,
        txca_resource_local = 89,
        txca_resource_unknown = 99,
    };

    public enum class TXCA_CHAT_TYPE_CS {
        txca_chat_via_voice = 0,  //语音请求
        txca_chat_via_text = 1,   //文本请求
        txca_chat_only_tts = 2,   //tts请求
        txca_chat_via_intent = 3, //暂未开放
        txca_chat_via_wakeup_check = 4, //唤醒校验请求
    };

    /** tts push使用，暂时没什么用了 */
    public enum class TXCA_AUDIO_DATA_FORMAT_CS {
        txca_audio_data_pcm = 0,
        txca_audio_data_silk = 1,
        txca_audio_data_opus = 2,
    };

    public enum class TXCA_EVENT_CS {
        txca_event_on_idle = 0,          // 空闲
        txca_event_on_request_start = 1, // 请求开始
        txca_event_on_speak = 2,         // 检测到说话
        txca_event_on_silent = 3,        // 检测到静音(only@device has not txca_device_local_vad)
        txca_event_on_recognize = 4,     // 识别文本实时返回
        txca_event_on_response = 5,      // 请求收到响应
        txca_event_on_tts = 6,           // 小微后台推送的TTS信息
        txca_event_on_response_intermediate = 7, //中间结果
        txca_event_on_iot_info = 8,      // iot info
    };

    public enum class TXCA_WORDS_TYPE_CS {
        txca_words_cmd = 0,
        txca_words_contact = 1,
        txca_words_user_json = 2,
    };

    /* struct define begin */
    public value struct TXCA_PARAM_SKILL_CS {
        String^ name; // name
        String^ id;   // ID
        int type;
    };

    public ref struct VOICE_ID_CS sealed{
    public:
        Platform::String^ Get() {
            return val;
        }
        void Set(Platform::String^ v) {
            val = v;
        }
    private:
        String^ val;
    };

    //state for report
    public ref struct TXCA_PARAM_STATE_CS sealed
    {
    public:
        void Setavailability(unsigned int val) {
            availability = val;
        }
        unsigned int Getavailability() {
            return availability;
        }
        void Setplay_id(Platform::String^ val) {
            play_id = val;
        }
        String^ Getplay_id() {
            return play_id;
        }
        void Setplay_content(Platform::String^ val) {
            play_content = val;
        }
        String^ Getplay_content() {
            return play_content;
        }
        void Setplay_state(unsigned int  val) {
            play_state = val;
        }
        unsigned int Getplay_state() {
            return play_state;
        }
        void Setplay_mode(unsigned int  val) {
            play_mode = val;
        }
        unsigned int Getplay_mode() {
            return play_mode;
        }
        void Setplay_offset(unsigned long long val) {
            play_offset = val;
        }
        unsigned long long Getplay_offset() {
            return play_offset;
        }
        void Setresource_type(unsigned int  val) {
            resource_type = val;
        }
        unsigned int Getresource_type() {
            return resource_type;
        }
        void Setskill_info(TXCA_PARAM_SKILL_CS  val) {
            skill_info = val;
        }
        TXCA_PARAM_SKILL_CS Getskill_info() {
            return skill_info;
        }
        void Setresource_name(Platform::String^ val) {
            resource_name = val;
        }
        String^ Getresource_name() {
            return resource_name;
        }
        void Setperformer(Platform::String^ val) {
            performer = val;
        }
        String^ Getperformer() {
            return performer;
        }
        void Setcollection(Platform::String^ val) {
            collection = val;
        }
        String^ Getcollection() {
            return collection;
        }
        void Setcover_url(Platform::String^ val) {
            cover_url = val;
        }
        String^ Getcover_url() {
            return cover_url;
        }
        void Setvolume(int  val) {
            volume = val;
        }
        int Getvolume() {
            return volume;
        }
        void Setbrightness(int  val) {
            resource_type = val;
        }
        int Getbrightness() {
            return brightness;
        }

    private:
        TXCA_PARAM_SKILL_CS skill_info; //场景信息
        unsigned int play_state; //txca_playstate
        unsigned int availability;
        String^ play_id;
        String^ play_content;
        unsigned long long play_offset; // 单位是s
        unsigned int play_mode;
        unsigned int resource_type;
        String^ resource_name;
        String^ performer;
        String^ collection;
        String^ cover_url;
        int volume;
        int brightness;
    };

    public ref struct TXCA_PARAM_CONTEXT_CS sealed {

    public:
        void Setid(Platform::String^ val) {
            id = val;
        }
        String^ Getid() {
            return id;
        }
        void Setwakeup_word(Platform::String^ val) {
            wakeup_word = val;
        }
        String^ Getwakeup_word() {
            return wakeup_word;
        }
        void Setspeak_timeout(unsigned int val) {
            speak_timeout = val;
        }
        unsigned int Getspeak_timeout() {
            return speak_timeout;
        }
        void Setsilent_timeout(unsigned int val) {
            silent_timeout = val;
        }
        unsigned int Getsilent_timeout() {
            return silent_timeout;
        }

        void Setvoice_request_begin(bool val) {
            voice_request_begin = val;
        }
        bool Getvoice_request_begin() {
            return voice_request_begin;
        }
        void Setvoice_request_end(bool val) {
            voice_request_end = val;
        }
        bool Getvoice_request_end() {
            return voice_request_end;
        }
        void Setrequest_param(unsigned long long val) {
            request_param = val;
        }
        unsigned long long Getrequest_param() {
            return request_param;
        }
        void Setlocal_tts_version(int val) {
            local_tts_version = val;
        }
        int Getlocal_tts_version() {
            return local_tts_version;
        }
        void Setwakeup_profile(TXCA_WAKEUP_PROFILE_CS val) {
            wakeup_profile = val;
        }
        TXCA_WAKEUP_PROFILE_CS Getwakeup_profile() {
            return wakeup_profile;
        }
        void Setwakeup_type(TXCA_WAKEUP_TYPE_CS val) {
            wakeup_type = val;
        }
        TXCA_WAKEUP_TYPE_CS Getwakeup_type() {
            return wakeup_type;
        }
        void Setskill_info(TXCA_PARAM_SKILL_CS val) {
            skill_info = val;
        }
        TXCA_PARAM_SKILL_CS Getskill_info() {
            return skill_info;
        }

    private:
        String^ id;              //context id
        unsigned int speak_timeout;  //wait speak timeout time(ms)
        unsigned int silent_timeout; //speak to silent timeout time(ms)
        bool voice_request_begin;
        bool voice_request_end;
        TXCA_WAKEUP_PROFILE_CS wakeup_profile; //远场or近场
        TXCA_WAKEUP_TYPE_CS wakeup_type;
        String^ wakeup_word; // 唤醒词文本
        unsigned long long request_param;
        int local_tts_version;
        TXCA_PARAM_SKILL_CS skill_info;
    };

    

    public ref struct TXCA_PARAM_RESOURCE_CS sealed {

    public:

        void Setformat(TXCA_RESOURCE_FORMAT_CS val) {
            format = val;
        }
        TXCA_RESOURCE_FORMAT_CS Getformat() {
            return format;
        }
        void Setid(String^ val) {
            id = val;
        }
        String^ Getid() {
            return id;
        }
        void Setcontent(String^ val) {
            content = val;
        }
        String^ Getcontent() {
            return content;
        }
        void Setextend_buffer(String^ val) {
            extend_buffer = val;
        }
        String^ Getextend_buffer() {
            return extend_buffer;
        }
        void Setplay_count(int val) {
            play_count = val;
        }
        int Getplay_count() {
            return play_count;
        }
        void Setoffset(unsigned int val) {
            offset = val;
        }
        unsigned int Getoffset() {
            return offset;
        }
    private:
        TXCA_RESOURCE_FORMAT_CS format; // url/tts/notify/text/command
        String^ id;                    // resource's id, if format == text or notify, id is null
        String^ content;               // url(url/notify) or text(tts/text) or command value
        int play_count;              // play count, if value == -1, means no limit
        unsigned int offset;         // url's seek
        String^ extend_buffer;         // json
    };

    public ref class TXCA_PARAM_RES_GROUP_CS sealed
    {
    private:
        property Windows::Foundation::Collections::IVector <TXCA_PARAM_RESOURCE_CS^>^ resources;  // todo was pointer
        unsigned int resources_size;
    public:
        TXCA_PARAM_RES_GROUP_CS() {
            resources = ref new Platform::Collections::Vector<TXCA_PARAM_RESOURCE_CS^>();
        }

        void SetResourceSize(unsigned int sz) {
            resources_size = sz;
        }
        unsigned int GetResourceSize() {
            return resources_size;
        }

        void SetResource(Windows::Foundation::Collections::IVector <TXCA_PARAM_RESOURCE_CS^>^ res) {
            resources = res;
        }
        Windows::Foundation::Collections::IVector<TXCA_PARAM_RESOURCE_CS^>^ GetResource() {
            return resources;
        }
    };

    public ref class TXCA_PARAM_AUDIO_DATA_CS sealed {
    public:
        void Setid(String^ val) {
            id = val;
        }
        String^ Getid() {
            return id;
        }
        void Setseq(unsigned int val) {
            seq = val;
        }
        unsigned int Getseq() {
            return seq;
        }
        void Setis_end(unsigned int val) {
            is_end = val;
        }
        unsigned int Getis_end() {
            return is_end;
        }
        void Setpcm_sample_rate(unsigned int val) {
            pcm_sample_rate = val;
        }
        unsigned int Getpcm_sample_rate() {
            return pcm_sample_rate;
        }
        void Setsample_rate(unsigned int val) {
            sample_rate = val;
        }
        unsigned int Getsample_rate() {
            return sample_rate;
        }
        void Setchannel(unsigned int val) {
            channel = val;
        }
        unsigned int Getchannel() {
            return channel;
        }
        void Setformat(TXCA_AUDIO_DATA_FORMAT_CS val) {
            format = val;
        }
        TXCA_AUDIO_DATA_FORMAT_CS Getformat() {
            return format;
        }
        void Setraw_data(const Array<byte>^ val) {
            raw_data = ref new Array<byte>(val->Length);
            for (int i = 0; i < (int)val->Length; i++)
            {
                raw_data[i] = val[i];
            }
        }
        Array<byte>^ Getraw_data() {
            return raw_data;
        }
        void Setraw_data_len(unsigned int val) {
            raw_data_len = val;
        }
        unsigned int Getraw_data_len() {
            return raw_data_len;
        }
    private:
        String^ id;                // 资源id
        unsigned int seq;              // 序号
        unsigned int is_end;           // 最后一包了
        unsigned int pcm_sample_rate;  // pcm采样率
        unsigned int sample_rate;      // 音频数据的(例如:opus)采样率
        unsigned int channel;          // 声道
        TXCA_AUDIO_DATA_FORMAT_CS format; // 格式(例如:opus)
        Array<byte>^ raw_data;          // 数据内容
        unsigned int raw_data_len;     // 数据长度
    };

    public ref class TXCA_PARAM_RESPONSE_CS sealed
    {
    public:
        TXCA_PARAM_RESPONSE_CS() {
            resource_groups = ref new Platform::Collections::Vector<TXCA_PARAM_RES_GROUP_CS^>();
        }
        TXCA_PARAM_SKILL_CS GetSkillInfo() {
            return skill_info;
        }
        void SetSkillInfo(TXCA_PARAM_SKILL_CS info) {
            skill_info = info;
        }
        TXCA_PARAM_SKILL_CS GetLastSkillInfo() {
            return last_skill_info;
        }
        void SetLastSkillInfo(TXCA_PARAM_SKILL_CS info) {
            last_skill_info = info;
        }
        int GetErrorCode() {
            return error_code;
        }
        void SetErrorCode(int val) {
            error_code = val;
        }
        String^ GetVoiceId() {
            return voice_id;
        }
        void SetVoiceId(String^ val) {
            voice_id = val;
        }
        TXCA_PARAM_CONTEXT_CS^ Getcontext() {
            return context;
        }
        void Setcontext(TXCA_PARAM_CONTEXT_CS^ val) {
            context = val;
        }
        String^ Getrequest_text() {
            return request_text;
        }
        void Setrequest_text(String^ val) {
            request_text = val;
        }
        String^ Getresponse_data() {
            return response_data;
        }
        void Setresponse_data(String^ val) {
            response_data = val;
        }
        unsigned int Getresponse_type() {
            return response_type;
        }
        void Setresponse_type(unsigned int val) {
            response_type = val;
        }
        String^ Getintent_info() {
            return intent_info;
        }
        void Setintent_info(String^ val) {
            intent_info = val;
        }
        unsigned int Getresource_groups_size() {
            return resource_groups_size;
        }
        void Setresource_groups_size(unsigned int val) {
            resource_groups_size = val;
        }
        Windows::Foundation::Collections::IVector<TXCA_PARAM_RES_GROUP_CS^>^ Getresource_groups() {
            return resource_groups;
        }
        void Setresource_groups(Windows::Foundation::Collections::IVector<TXCA_PARAM_RES_GROUP_CS^>^ val) {
            resource_groups = val;
        }
        bool Gethas_more_playlist() {
            return has_more_playlist;
        }
        void Sethas_more_playlist(bool val) {
            has_more_playlist = val;
        }
        bool Gethas_history_playlist() {
            return has_history_playlist;
        }
        void Sethas_history_playlist(bool val) {
            has_history_playlist = val;
        }
        bool Getis_recovery() {
            return is_recovery;
        }
        void Setis_recovery(bool val) {
            is_recovery = val;
        }
        bool Getis_notify() {
            return is_notify;
        }
        void Setis_notify(bool val) {
            is_notify = val;
        }
        void Setwakeup_flag(unsigned int val) {
            wakeup_flag = val;
        }
        unsigned int Getwakeup_flag() {
            return wakeup_flag;
        }
        void Setresource_list_type(int val) {
            resource_list_type = val;
        }
        int Getresource_list_type() {
            return resource_list_type;
        }
        void Setplay_behavior(TXCA_PLAYLIST_ACTION_CS val) {
            play_behavior = val;
        }
        TXCA_PLAYLIST_ACTION_CS Getplay_behavior() {
            return play_behavior;
        }
        void Setcontrol_value(String^ val) {
            control_value = val;
        }
        String^ Getcontrol_value() {
            return control_value;
        }
        void Setrsp_text(String^ val) {
            rsp_text = val;
        }
        String^ Getrsp_text() {
            return rsp_text;
        }
        bool Gethas_more_playlist_up() {
            return has_more_playlist_up;
        }
        void Sethas_more_playlist_up(bool val) {
            has_more_playlist_up = val;
        }
        void Setcontrol_id(unsigned int val) {
            control_id = val;
        }
        unsigned int Getcontrol_id() {
            return control_id;
        }

    private:
        TXCA_PARAM_SKILL_CS skill_info; //场景信息
        TXCA_PARAM_SKILL_CS last_skill_info; //之前的场景信息
        int error_code; //请求错误码
        String^ voice_id;          //请求id
        TXCA_PARAM_CONTEXT_CS^ context; //上下文信息
        String^ request_text;   //ASR结果文本
        unsigned int response_type; //用于信息展示的json数据type
        String^ response_data;  //用于信息展示的json数据
        String^ intent_info; //定制的意图信息
        unsigned int resource_groups_size;     //资源集合列表size
        property Windows::Foundation::Collections::IVector<TXCA_PARAM_RES_GROUP_CS^>^ resource_groups; //资源集合列表  // todo was pointer
        bool has_more_playlist;                //是否可以加载更多
        bool has_history_playlist;
        bool is_recovery;                      //是否可以恢复播放
        bool is_notify;                        //是通知
        unsigned int wakeup_flag;              //TXCA_WAKEUP_FLAG 云端校验唤醒请求带下来的结果，0表示非该类结果，1表示唤醒失败，2表示唤醒成功并且未连续说话，3表示说的指令唤醒词，4可能为中间结果，表示唤醒成功了，还在继续检测连续说话或者已经在连续说话了
        TXCA_PLAYLIST_ACTION_CS play_behavior;    //列表拼接类型
        int resource_list_type;
        String^ rsp_text;
        unsigned int control_id;
        String^ control_value;
        bool has_more_playlist_up;
    };

    //设备绑定者信息
    public ref struct TX_BINDER_INFO_CS sealed
    {
    public:
        int Gettype() {
            return type;
        }
        void Settype(int val) {
            type = val;
        }
        String^ Getnick_name() {
            return nick_name;
        }
        void Setnick_name(String^ val) {
            nick_name = val;
        }

        String^ Gethead_url() {
            return head_url;
        }
        void Sethead_url(String^ val) {
            head_url = val;
        }

        String^ Getuser_name() {
            return user_name;
        }
        void Setuser_name(String^ val) {
            user_name = val;
        }

        int Getgender() {
            return gender;
        }
        void Setgender(int val) {
            gender = val;
        }

        String^ Getremark() {
            return remark;
        }
        void Setremark(String^ val) {
            remark = val;
        }

    private:
        Platform::String^ user_name;  // binder ID
        Platform::String^ nick_name;  // 绑定者昵称
        int gender;                //绑定者性别
        Platform::String^ head_url;       //绑定者头像url
        int type;           //1:manager 0:normal
        Platform::String^ remark;    
    };

    public ref struct TX_LOG_REPORT_CS sealed
    {
    public:
        unsigned long long Gettime_stamp_ms() {
            return time_stamp_ms;
        }
        void Setime_stamp_ms(unsigned long long val) {
            time_stamp_ms = val;
        }
        String^ Getvoice_id() {
            return voice_id;
        }
        void Setvoice_id(String^ val) {
            voice_id = val;
        }

        String^ Getlog_data() {
            return log_data;
        }
        void Setlog_data(String^ val) {
            log_data = val;
        }

        String^ Getevent() {
            return event;
        }
        void Setevent(String^ val) {
            event = val;
        }

    private:
        Platform::String^ voice_id;
        Platform::String^ log_data;
        Platform::String^ event;
        unsigned long long time_stamp_ms;
    };

    public ref struct TX_DEVICE_PROFILE_CS sealed
    {
    public:
        String^ Getdefault_device_name() {
            return default_device_name;
        }
        void Setdefault_device_name(String^ val) {
            default_device_name = val;
        }

        String^ Getdefault_head_url() {
            return default_head_url;
        }
        void Setdefault_head_url(String^ val) {
            default_head_url = val;
        }

        String^ Getdevice_name() {
            return device_name;
        }
        void Setdevice_name(String^ val) {
            device_name = val;
        }

        String^ Gethead_url() {
            return head_url;
        }
        void Sethead_url(String^ val) {
            head_url = val;
        }

    private:
        Platform::String^ default_device_name;
        Platform::String^ default_head_url;
        Platform::String^ device_name;
        Platform::String^ head_url;
    };

    /* callbacks */
    // devices
    public delegate void CallBackOnNetStatusChange(int status); //1 online, 0 offline 
    public delegate void CallBackOnLoginComplete(int error_code);
    public delegate void CallBackOnLogOutComplete(int error_code);
    public delegate void CallBackOnNicknameUpdate(int error_code, Platform::String^ nickname);
    public delegate void CallBackOnAvatarUpdate(int error_code, Platform::String^ avatar_url);
    public delegate void CallBackOnBinderListChange(int error_code, IVector<TX_BINDER_INFO_CS^>^ binderList);
    public delegate void CallBackOnBinderVerify(TX_BINDER_INFO_CS^ binderInfo);
    public delegate void CallBackOnQrCodeRefresh(int error_code, Platform::String^ qr_code_path, unsigned int expire_time, unsigned int expire_in);
    //log
    public delegate void CallBackOnLog(int level, Platform::String^ tag, Platform::String^ filename, int line, Platform::String^ funcname, Platform::String^ data);
    //xiaowei
    public delegate bool CallBackOnRequest(Platform::String^ voice_id, TXCA_EVENT_CS event, TXCA_PARAM_RESPONSE_CS^ state_info, Platform::String^ extend_info, uint32 extend_info_len);
    public delegate bool CallBackOnNetDelay(Platform::String^ voice_id, unsigned long long time);
    public delegate void CallBackOnRequestCMD(Platform::String^ voice_id, int error_code, Platform::String^json_data);
    /* callbacks end */

    public ref class ClassDeviceSDK sealed
    {
    public:
        ClassDeviceSDK();
        int _device_init(
            Platform::String^ sn, 
            Platform::String^ license, 
            Platform::String^ appid,
            int product_id, 
            int key_version, 
            int hardware_version,
            int run_mode
            );

        /* device interface */
        int _device_uninit();

        String^ _device_get_sdk_version();

        int _device_refresh_qr_code();

        unsigned long long _device_get_uin();

        int _device_get_binder_list(_OUTPUT_PARAM_ IVector<TX_BINDER_INFO_CS^>^ binderList);

        int _device_erase_all_binders();

        int _device_erase_binder(Platform::String^ binder_user_name);

        int _device_get_all_unverify_bind_request(_OUTPUT_PARAM_ IVector<TX_BINDER_INFO_CS^>^ binderList);

        int _device_handle_binder_verify_request(Platform::String^ user_name, bool is_accept);

        int _device_update_binder_alias(Platform::String^ user_name, Platform::String^ alias);

        int _device_update_device_nickname(Platform::String^ nickname);

        int _device_update_device_avatar(Platform::String^ jpg_file_path);

        int _device_get_device_profile(_OUTPUT_PARAM_ TX_DEVICE_PROFILE_CS^ profile);

        /* xiaowei core interface */
        int _xiaowei_start_service();

        // if req_str is nullptr, will use rwa_data, this can avoid utf-8 probelm
        int _xiaowei_request(_OUTPUT_PARAM_ VOICE_ID_CS^ voice_id, TXCA_CHAT_TYPE_CS type, const Array<byte>^ raw_data,
            unsigned int char_data_len, TXCA_PARAM_CONTEXT_CS^ context);

        int _xiaowei_request_cancel(Platform::String^ voice_id);

        int _xiaowei_report_state(TXCA_PARAM_STATE_CS^ state);

        int _xiaowei_request_cmd(_OUTPUT_PARAM_ VOICE_ID_CS^ voice_id, Platform::String^ cmd, Platform::String^ sub_cmd, Platform::String^ param);

        int _xiaowei_enable_V2A(bool enable);

        int _xiaowei_set_words_list(TXCA_WORDS_TYPE_CS type, IVector<Platform::String^>^ words);

        int _xiaowei_get_iot_device_list();

        int _xiaowei_set_device_availability(int availability);

        static event CallBackOnNetStatusChange^ callBackOnNetStatusChange;
        static event CallBackOnLoginComplete^ callBackOnLoginComplete;
        static event CallBackOnLogOutComplete^ callBackOnLogOutComplete;
        static event CallBackOnNicknameUpdate^ callBackOnNicknameUpdate;
        static event CallBackOnAvatarUpdate^ callBackOnAvatarUpdate;
        static event CallBackOnBinderListChange^ callBackOnBinderListChange;
        static event CallBackOnBinderVerify^ callBackOnBinderVerify;
        static event CallBackOnQrCodeRefresh^ callBackOnQrCodeRefresh;
        static event CallBackOnLog^ callBackOnLog;
        static event CallBackOnRequest^ callBackOnRequest;
        static event CallBackOnNetDelay^ callBackOnNetDelay;
        static event CallBackOnRequestCMD^ callBackOnRequestCMD;

    private:
        // debug print
        static void LogMessage(Object^ parameter);

        //xiaowei callbacks
        static void on_net_status_changed(int status);
        static void on_login_complete(int error_code);
        static void on_logout_complete(int error_code);
        static void on_nickname_update(int error_code, const char* nickname, int nickname_length);
        static void on_avatar_update(int error_code, const char* avatar_url, int url_length);
        static void on_binder_list_change(int error_code, const DEVICE_BINDER_INFO* p_binder_list, int n_count);
        static void on_binder_verify(DEVICE_BINDER_INFO info);
        static void on_qr_code_refresh(int err_code, const char* qr_code_path, unsigned int expire_time, unsigned int expire_in);
        static bool on_request_callback(const char* voice_id, XIAOWEI_EVENT event, const char* state_info, const char* extend_info, unsigned int extend_info_len);
        static bool on_net_delay_callback(const char* voice_id, unsigned long long time);
        static void on_cmd_request_callback(const char* voice_id, int xiaowei_err_code, const char* json);
        static void on_device_log(int level, const char* tag, const char* filename, int line, const char* funcname, const char* data, unsigned int len);

        // help func
        static void fill_binder_list(IVector<TX_BINDER_INFO_CS^>^ binderList, const DEVICE_BINDER_INFO* pBinderList, int nCount);

        static bool initialized;
        static void InitNetwork();
    };
}