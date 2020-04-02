#include "pch.h"
#include "ClassDeviceSDK.h"
#include <cvt/wstring>
#include <codecvt>
#include <iostream>
#include <fstream>
#include "WS2tcpip.h"
#include "ws2def.h"
#include <mutex>
#include <string>

using namespace std;
using namespace sdkrtv2;
using namespace Platform;
using namespace Windows::UI::Core;

String^ StringFromAscIIChars(const char* chars)
{
    if (chars == nullptr || strnlen_s(chars, INT_MAX) == 0) {
        return "";
    }
    std::string s_str = std::string(chars);
    std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
    const wchar_t* w_char = wid_str.c_str();
    Platform::String^ p_string = ref new Platform::String(w_char);
    /*size_t newsize = strnlen_s(chars, INT_MAX) + 1;
    wchar_t* wcstring = new wchar_t[newsize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
    String^ str = ref new Platform::String(wcstring);
    delete[] wcstring;*/
    return p_string;
}

std::string StringUtf8ToAscIIChars(String^ text) {
    //size_t   i;
    const wchar_t* wide_chars = text->Data();
    //char chars[512];
    /*int ret = wcstombs_s(&i, chars, 512, wide_chars, 512);
    printf(" convert   character: %d, %d\n\n", i, ret);*/
    stdext::cvt::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    std::string stringUtf8 = convert.to_bytes(wide_chars);
    //strcpy_s(chars, 512, stringUtf8.c_str());
    return stringUtf8;
}

std::string StringToAscIIChars(String^ text) {
    size_t   i;
    const wchar_t* wide_chars = text->Data();
    char chars[512];
    int ret = wcstombs_s(&i, chars, 512, wide_chars, 512);
    //printf("convert character: %d, %d\n\n", i, ret);
    if (i > 0) {
        return std::string(chars);
    }
    else {
        return "";
    }
}

std::string UTF8_To_Std_Str(const std::string& str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr = pBuf;

    delete[] pBuf;
    delete[] pwBuf;

    pBuf = NULL;
    pwBuf = NULL;

    return retStr;
}

Platform::String^ UTF8_To_Managed_Str(const char* chars/*const std::string& str*/)
{
    if (chars == nullptr /*|| chars == "" || chars == " " || strnlen_s(chars, INT_MAX) <= 1*/) { // todo what is  0x20
        return "";
    }
    std::string str = std::string(chars);
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);

    Platform::String^ pStr = ref new Platform::String(pwBuf);

    delete[] pwBuf;
    pwBuf = NULL;

    return pStr;
}

Platform::String^ UTF8_To_Managed_Str2(const std::string& str)
{
    int size = MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), (int)str.length(), nullptr, 0);
    std::wstring utf16_str(size, '\0');
    MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), (int)str.length(), &utf16_str[0], size);

    Platform::String^ pStr = ref new Platform::String(utf16_str.c_str());

    return pStr;
}

std::once_flag once_flag_;

void ClassDeviceSDK::InitNetwork() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        OutputDebugStringA("WSAStartup failed with error\n");
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        OutputDebugStringA("Could not find a usable version of Winsock.dll\n");
        WSACleanup();

    }
    else {
        OutputDebugStringA("The Winsock 2.2 dll was found okay\n");
    }
}

ClassDeviceSDK::ClassDeviceSDK()    
{
    LogMessage("ClassDeviceSDK called");
    std::call_once(once_flag_, [] {InitNetwork(); });
}

void ClassDeviceSDK::LogMessage(Object^ parameter)
{
    auto paraString = parameter->ToString();
    auto formattedText = std::wstring(paraString->Data()).append(L"\r\n");
    auto chrs = formattedText.c_str();
    OutputDebugString(chrs);
}

int ClassDeviceSDK::_device_init(
    Platform::String^ sn,
    Platform::String^ license,
    Platform::String^ appid,
    int product_id,
    int key_version,
    int hardware_version,
    int run_mode
) {
    LogMessage("init called");
    if (sn == nullptr || license == nullptr || appid == nullptr) return error_param_invalid;

    DEVICE_INFO info = { 0 };
    info.os_platform = (const char*)"UWP";  // 平台名
    string str_sn = StringToAscIIChars(sn);
    string str_licence = StringToAscIIChars(license);
    string str_appid = StringToAscIIChars(appid);
    info.device_serial_number = str_sn.c_str();
    info.device_license = str_licence.c_str();     // 设备名称
    info.product_id = product_id;
    info.key_version = key_version;
    info.user_hardware_version = hardware_version;
    info.run_mode = run_mode;
    info.xiaowei_appid = str_appid.c_str();

    DEVICE_NOTIFY notify = { 0 };
    notify.on_net_status_changed = on_net_status_changed;
    notify.on_login_complete = on_login_complete;
    notify.on_logout_complete = on_logout_complete;
    notify.on_nickname_update = on_nickname_update;
    notify.on_avatar_update = on_avatar_update;
    notify.on_binder_list_change = on_binder_list_change;
    notify.on_binder_verify = on_binder_verify;
    notify.on_qr_code_refresh = on_qr_code_refresh;

    Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
    LogMessage("debug dir " + localfolder);
    DEVICE_INIT_PATH init_path = { 0 };
    string str_path = StringUtf8ToAscIIChars(localfolder);
    init_path.system_path = str_path.c_str();
    init_path.system_path_capacity = 100 * 1024 * 1024;

    SERVICE_CALLBACK service_callback = {0};
    service_callback.xiaowei_callback.on_net_delay_callback = on_net_delay_callback;
    service_callback.xiaowei_callback.on_request_callback = on_request_callback;
    LogMessage("set log default function");
    device_set_log_func(on_device_log, info.run_mode == 0? 3 : 0, false);
    //int ret = device_set_log_func(on_device_log, 0, false);
    
    return device_init(&info, &notify, &init_path, 1,&service_callback);
}

/* device interface */
int ClassDeviceSDK::_device_uninit() {
    return device_uninit();
}

String^ ClassDeviceSDK::_device_get_sdk_version() {
    unsigned int m, s, f, b;
    device_get_sdk_version(&m, &s, &f, &b);
    std::string st;
    st += std::to_string(m);
    st += ".";
    st += std::to_string(s);
    st += ".";
    st += std::to_string(f);
    st += ".";
    st += std::to_string(b);
    return StringFromAscIIChars(st.c_str());
}

int ClassDeviceSDK::_device_refresh_qr_code() {
    return device_refresh_dynamic_qr_code();
}

unsigned long long ClassDeviceSDK::_device_get_uin() {
    return device_get_uin();
}

int ClassDeviceSDK::_device_get_binder_list(_OUTPUT_PARAM_ IVector<TX_BINDER_INFO_CS^>^ binderList) {
    if (binderList == nullptr) return error_param_invalid;
    binderList->Clear();
    int count = 10;
    DEVICE_BINDER_INFO p_info[10] = {0};
    auto ret = device_get_binder_list(p_info, &count);
    if (error_null == ret) {
        for (int i = 0; i < count; i++)
        {
            TX_BINDER_INFO_CS^ tx_binder_info = ref new TX_BINDER_INFO_CS();
            tx_binder_info->Settype(p_info[i].type);
            tx_binder_info->Setuser_name(StringFromAscIIChars(p_info[i].username));
            tx_binder_info->Setnick_name(UTF8_To_Managed_Str(p_info[i].nickname));
            tx_binder_info->Setgender(p_info[i].sex);
            tx_binder_info->Sethead_url(StringFromAscIIChars(p_info[i].head_img));
            tx_binder_info->Settype(p_info[i].type);
            binderList->Append(tx_binder_info);
        }
        return error_null;
    }
    else if (error_memory_not_enough == ret && count < 100) {
        DEVICE_BINDER_INFO* p_info2 = new DEVICE_BINDER_INFO[count];
        if (device_get_binder_list(p_info2, &count) != error_null) return error_failed;
        for (int i = 0; i < count; i++)
        {
            TX_BINDER_INFO_CS^ tx_binder_info = ref new TX_BINDER_INFO_CS();
            tx_binder_info->Settype(p_info2[i].type);
            tx_binder_info->Setuser_name(StringFromAscIIChars(p_info2[i].username));
            tx_binder_info->Setnick_name(UTF8_To_Managed_Str(p_info2[i].nickname));
            tx_binder_info->Setgender(p_info2[i].sex);
            tx_binder_info->Sethead_url(StringFromAscIIChars(p_info2[i].head_img));
            tx_binder_info->Settype(p_info2[i].type);
            binderList->Append(tx_binder_info);
            return error_null;
        }
        delete[] p_info2;
    }
}

int ClassDeviceSDK::_device_erase_all_binders() {
    return device_erase_all_binders();
}

int ClassDeviceSDK::_device_erase_binder(Platform::String^ binder_user_name) {
    if (binder_user_name == nullptr) return error_param_invalid;
    string str_ = StringToAscIIChars(binder_user_name);
    return device_erase_binders(str_.c_str());
}

int ClassDeviceSDK::_device_get_all_unverify_bind_request(_OUTPUT_PARAM_ IVector<TX_BINDER_INFO_CS^>^ binderList) {
    if (binderList == nullptr) return error_param_invalid;
    binderList->Clear();
    int count = 20;
    DEVICE_BINDER_INFO p_info[20] = { 0 };
    auto ret = device_get_binder_list(p_info, &count);
    if (error_null == ret) {
        for (int i = 0; i < count; i++)
        {
            TX_BINDER_INFO_CS^ tx_binder_info = ref new TX_BINDER_INFO_CS();
            tx_binder_info->Settype(p_info[i].type);
            tx_binder_info->Setuser_name(StringFromAscIIChars(p_info[i].username));
            tx_binder_info->Setnick_name(UTF8_To_Managed_Str(p_info[i].nickname));
            tx_binder_info->Setgender(p_info[i].sex);
            tx_binder_info->Sethead_url(StringFromAscIIChars(p_info[i].head_img));
            tx_binder_info->Settype(p_info[i].type);
            binderList->Append(tx_binder_info);
        }
        return error_null;
    }
    return error_failed;
}

int ClassDeviceSDK::_device_handle_binder_verify_request(Platform::String^ user_name, bool is_accept) {
    if (user_name == nullptr) return error_param_invalid;
    string str = StringToAscIIChars(user_name);
    return device_handle_binder_verify_request(str.c_str(), is_accept);
}

int ClassDeviceSDK::_device_update_binder_alias(Platform::String^ user_name, Platform::String^ alias) {
    if (user_name == nullptr) return error_param_invalid;
    string str = StringToAscIIChars(user_name);
    string str2 = StringToAscIIChars(alias);
    return device_update_binder_alias(str.c_str(), str2.c_str());
}

int ClassDeviceSDK::_device_update_device_nickname(Platform::String^ nickname) {
    if (nickname == nullptr) return error_param_invalid;
    string str = StringToAscIIChars(nickname);
    return device_update_device_nickname(str.c_str());
}

int ClassDeviceSDK::_device_update_device_avatar(Platform::String^ jpg_file_path) {
    if (jpg_file_path == nullptr) return error_param_invalid;
    string str = StringToAscIIChars(jpg_file_path);
    return device_update_device_nickname(str.c_str());
}

int ClassDeviceSDK::_device_get_device_profile(_OUTPUT_PARAM_ TX_DEVICE_PROFILE_CS^ profile) {
    if (profile == nullptr) return error_param_invalid;
    DEVICE_PROFILE device_profile = { 0 };
    if (device_get_device_profile(&device_profile) != error_null) return error_failed;
    profile->Setdefault_device_name(UTF8_To_Managed_Str(device_profile.default_device_name));
    profile->Setdevice_name(UTF8_To_Managed_Str(device_profile.device_name)); 
    profile->Setdefault_head_url(StringFromAscIIChars(device_profile.default_device_head));
    profile->Sethead_url(StringFromAscIIChars(device_profile.device_head));
}

/* xiaowei core interface */
int ClassDeviceSDK::_xiaowei_start_service() {
    XIAOWEI_CALLBACK cb = { 0 };
    cb.on_net_delay_callback = on_net_delay_callback;
    cb.on_request_callback = on_request_callback;
    XIAOWEI_NOTIFY notify = { 0 };
    return xiaowei_service_start(&cb,&notify);
}

int ClassDeviceSDK::_xiaowei_request(_OUTPUT_PARAM_ VOICE_ID_CS^ voice_id, TXCA_CHAT_TYPE_CS type, const Array<byte>^ raw_data,
    unsigned int char_data_len, TXCA_PARAM_CONTEXT_CS^ context) {
    if (raw_data == nullptr || context == nullptr) return error_param_invalid;
    char req_voice_id[100];
    const int MAX_BUF_SIZE = 2048;
    XIAOWEI_CHAT_TYPE req_type = (XIAOWEI_CHAT_TYPE)type;

    char* req_chat_data;
    char buff[MAX_BUF_SIZE] = { 0 };
    if (char_data_len > MAX_BUF_SIZE) {
        req_chat_data = new char[char_data_len];
    }
    else {
        req_chat_data = buff;
    }
    for (int i = 0; i < (int)char_data_len; i++)
    {
        req_chat_data[i] = raw_data[i];
    }
  

    XIAOWEI_PARAM_CONTEXT req_context = { 0 };
    string str_context_id = StringToAscIIChars(context->Getid());
    req_context.id = str_context_id.c_str();
    req_context.speak_timeout = context->Getspeak_timeout();
    req_context.silent_timeout = context->Getsilent_timeout();
    req_context.voice_request_begin = context->Getvoice_request_begin();
    req_context.voice_request_end = context->Getvoice_request_end();
    XIAOWEI_WAKEUP_PROFILE wakeup_profile = (XIAOWEI_WAKEUP_PROFILE)context->Getwakeup_profile();
    XIAOWEI_WAKEUP_TYPE wakeup_type = (XIAOWEI_WAKEUP_TYPE)context->Getwakeup_type();
    req_context.wakeup_profile = wakeup_profile;
    req_context.wakeup_type = wakeup_type;
    string str_utf8_wakeup_word = StringUtf8ToAscIIChars(context->Getwakeup_word());
    req_context.wakeup_word = str_utf8_wakeup_word.c_str();
    req_context.request_param = context->Getrequest_param();
    req_context.local_tts_version = context->Getlocal_tts_version();
    auto skill_info = context->Getskill_info();
    string str_utf8_skill_info_id = StringUtf8ToAscIIChars(skill_info.id);
    req_context.skill_info.id = str_utf8_skill_info_id.c_str();
    string str_utf8_skill_info_name = StringUtf8ToAscIIChars(skill_info.name);
    req_context.skill_info.name = str_utf8_skill_info_name.c_str();
    req_context.skill_info.type = skill_info.type;
    int ret = xiaowei_request(req_voice_id, req_type, req_chat_data, char_data_len, &req_context);

    if (char_data_len > MAX_BUF_SIZE) {
        delete[] req_chat_data;
    }
    
    voice_id->Set(StringFromAscIIChars(req_voice_id));
    return ret;
}

int ClassDeviceSDK::_xiaowei_request_cancel(Platform::String^ voice_id) {
    string str = StringToAscIIChars(voice_id);
    return xiaowei_request_cancel(str.c_str());
}

int ClassDeviceSDK::_xiaowei_report_state(TXCA_PARAM_STATE_CS^ state) {
    if (state == nullptr) return error_param_invalid;
    XIAOWEI_PARAM_STATE xw_state = { 0 };
    auto skill_info = state->Getskill_info();
    string str_utf8_skill_info_id = StringUtf8ToAscIIChars(skill_info.id);
    string str_utf8_skill_info_name = StringUtf8ToAscIIChars(skill_info.name);
    string str_utf8_play_id = StringUtf8ToAscIIChars(state->Getplay_id());
    string str_utf8_play_content = StringUtf8ToAscIIChars(state->Getplay_content());
    string str_utf8_resource_name = StringUtf8ToAscIIChars(state->Getresource_name());
    string str_utf8_performer = StringUtf8ToAscIIChars(state->Getperformer());
    string str_utf8_collection = StringUtf8ToAscIIChars(state->Getcollection());
    string str_utf8_cover_url = StringUtf8ToAscIIChars(state->Getcover_url());
    xw_state.skill_info.id = str_utf8_skill_info_id.c_str();
    xw_state.skill_info.name = str_utf8_skill_info_name.c_str();
    xw_state.skill_info.type = skill_info.type;
    xw_state.play_state = (XIAOWEI_PLAY_STATE)state->Getplay_state();
    xw_state.availability = (XIAOWEI_AVAILABILITY)state->Getavailability();
    xw_state.play_id = str_utf8_play_id.c_str();
    xw_state.play_content = str_utf8_play_content.c_str();
    xw_state.play_offset = state->Getplay_offset();
    xw_state.play_mode = (XIAOWEI_PLAY_MODE)state->Getplay_mode();
    xw_state.resource_type = (XIAOWEI_RESOURCE_FORMAT)state->Getresource_type();
    xw_state.resource_name = str_utf8_resource_name.c_str();
    xw_state.performer = str_utf8_performer.c_str();
    xw_state.collection = str_utf8_collection.c_str();
    xw_state.cover_url = str_utf8_cover_url.c_str();
    xw_state.volume = state->Getvolume();
    xw_state.brightness = state->Getbrightness();
    
    return xiaowei_report_state(&xw_state);
}

int ClassDeviceSDK::_xiaowei_request_cmd(_OUTPUT_PARAM_ VOICE_ID_CS^ voice_id, Platform::String^ cmd, Platform::String^ sub_cmd, Platform::String^ param) {
    if (cmd == nullptr) return error_param_invalid;
    char req_voice_id[100];
    string str_cmd = StringToAscIIChars(cmd);
    string str_sub_cmd = sub_cmd ? StringToAscIIChars(sub_cmd):"";
    string str_param = param ? StringUtf8ToAscIIChars(param):"";
    auto ret = xiaowei_request_cmd(req_voice_id, str_cmd.c_str(),
        str_sub_cmd.c_str(),
        str_param.c_str(),
        nullptr);
    voice_id->Set(StringFromAscIIChars(req_voice_id));
    return ret;
}

int ClassDeviceSDK::_xiaowei_enable_V2A(bool enable) {
    return xiaowei_enable_v2a(enable);
}

int ClassDeviceSDK::_xiaowei_set_words_list(TXCA_WORDS_TYPE_CS type, IVector<Platform::String^>^ words) {
    if (words == nullptr || words->Size == 0) return error_param_invalid;
    const char** words_list = new const char*[words->Size];
    for(int i = 0; i < words->Size; i++)
    {
        string str = StringUtf8ToAscIIChars(words->GetAt(i));
        const char* word = str.c_str();
        words_list[i] = word;
    }

    auto ret = xiaowei_set_wordslist((XIAOWEI_WORDS_TYPE)type, (char**)words_list, (int)words->Size, nullptr);
    delete[] words_list;
    return ret;
}

int ClassDeviceSDK::_xiaowei_get_iot_device_list() {
    return xiaowei_get_iot_device_list();
}

int ClassDeviceSDK::_xiaowei_set_device_availability(int availability) {
    return xiaowei_set_device_availability(availability);
}




/* callbacks impl*/
void ClassDeviceSDK::on_net_status_changed(int status) {
    LogMessage("on_net_status_changed");
    callBackOnNetStatusChange(status);
}

void ClassDeviceSDK::on_login_complete(int error_code) {
    LogMessage("on_login_complete");
    callBackOnLoginComplete(error_code);
}

void ClassDeviceSDK::on_logout_complete(int error_code) {
    LogMessage("on_logout_complete");
    callBackOnLogOutComplete(error_code);
}

void ClassDeviceSDK::on_nickname_update(int error_code, const char* nickname, int nickname_length) {
    LogMessage("on_nickname_update");
    callBackOnNicknameUpdate(error_code, StringFromAscIIChars(nickname));
}

void ClassDeviceSDK::on_avatar_update(int error_code, const char* avatar_url, int url_length) {
    LogMessage("on_avatar_update");
    callBackOnAvatarUpdate(error_code, StringFromAscIIChars(avatar_url));
}

void ClassDeviceSDK::on_binder_list_change(int error_code, const DEVICE_BINDER_INFO* p_binder_list, int n_count) {
    LogMessage("on_binder_list_change");
    IVector<TX_BINDER_INFO_CS^>^ binderList = ref new Platform::Collections::Vector<TX_BINDER_INFO_CS^>();
    fill_binder_list(binderList, p_binder_list, n_count);
    callBackOnBinderListChange(error_code, binderList);
}

void ClassDeviceSDK::on_binder_verify(DEVICE_BINDER_INFO info) {
    LogMessage("on_binder_verify");
    TX_BINDER_INFO_CS^ tx_binder_info = ref new TX_BINDER_INFO_CS();
    tx_binder_info->Settype(info.type);
    tx_binder_info->Setuser_name(StringFromAscIIChars(info.username));
    tx_binder_info->Setnick_name(UTF8_To_Managed_Str(info.nickname));
    tx_binder_info->Setgender(info.sex);
    tx_binder_info->Sethead_url(StringFromAscIIChars(info.head_img));
    tx_binder_info->Settype(info.type);
    callBackOnBinderVerify(tx_binder_info);
}

void ClassDeviceSDK::on_qr_code_refresh(int err_code, const char* qr_code_path, unsigned int expire_time, unsigned int expire_in) {
    LogMessage("on_qr_code_refresh");
    callBackOnQrCodeRefresh(err_code, StringFromAscIIChars(qr_code_path), expire_time, expire_in);
}

bool ClassDeviceSDK::on_request_callback(const char* voice_id, XIAOWEI_EVENT event, const char* state_info, const char* extend_info, unsigned int extend_info_len) {
    LogMessage("on_request_callback");
    auto eventcs = (TXCA_EVENT_CS)event;
    TXCA_PARAM_RESPONSE_CS^ state_info_response_cs = ref new TXCA_PARAM_RESPONSE_CS();
    if (state_info != nullptr) {
        auto param_rsp = (XIAOWEI_PARAM_RESPONSE*)(state_info);
        state_info_response_cs->SetVoiceId(StringFromAscIIChars(param_rsp->voice_id));
        state_info_response_cs->SetErrorCode(param_rsp->error_code);

        TXCA_PARAM_SKILL_CS skill_cs;
        skill_cs.name = UTF8_To_Managed_Str(param_rsp->skill_info.name);
        skill_cs.id = StringFromAscIIChars(param_rsp->skill_info.id);
        skill_cs.type = param_rsp->skill_info.type;
        state_info_response_cs->SetSkillInfo(skill_cs);

        TXCA_PARAM_SKILL_CS last_skill_cs;
        last_skill_cs.name = UTF8_To_Managed_Str(param_rsp->last_skill_info.name);
        last_skill_cs.id = StringFromAscIIChars(param_rsp->last_skill_info.id);
        last_skill_cs.type = param_rsp->last_skill_info.type;
        state_info_response_cs->SetLastSkillInfo(last_skill_cs);

        auto& tpc = param_rsp->context;
        auto tpc_cs = ref new TXCA_PARAM_CONTEXT_CS();
        tpc_cs->Setid(StringFromAscIIChars(tpc.id));
        tpc_cs->Setspeak_timeout(tpc.speak_timeout);
        tpc_cs->Setsilent_timeout(tpc.silent_timeout);
        tpc_cs->Setvoice_request_begin(tpc.voice_request_begin);
        tpc_cs->Setvoice_request_end(tpc.voice_request_end);
        tpc_cs->Setwakeup_profile((TXCA_WAKEUP_PROFILE_CS)tpc.wakeup_profile);
        tpc_cs->Setwakeup_type((TXCA_WAKEUP_TYPE_CS)tpc.wakeup_type);
        tpc_cs->Setwakeup_word(UTF8_To_Managed_Str(tpc.wakeup_word));
        tpc_cs->Setrequest_param(tpc.request_param);
        tpc_cs->Setlocal_tts_version(tpc.local_tts_version);
        state_info_response_cs->Setcontext(tpc_cs);

        state_info_response_cs->Setrequest_text(UTF8_To_Managed_Str(param_rsp->request_text));
        state_info_response_cs->Setresponse_type(param_rsp->response_type);
        state_info_response_cs->Setresponse_data(UTF8_To_Managed_Str(param_rsp->response_data));
        state_info_response_cs->Setintent_info(UTF8_To_Managed_Str(param_rsp->intent_info));
        for (unsigned int i = 0; i < param_rsp->resource_groups_size; i++) {
            auto& grp = param_rsp->resource_groups[i];
            TXCA_PARAM_RES_GROUP_CS^ tprgcs = ref new TXCA_PARAM_RES_GROUP_CS();
            tprgcs->SetResourceSize(grp.resources_size);
            for (unsigned int j = 0; j < grp.resources_size; j++) {
                TXCA_PARAM_RESOURCE_CS^ tprcs = ref new TXCA_PARAM_RESOURCE_CS();
                tprcs->Setformat((TXCA_RESOURCE_FORMAT_CS)(grp.resources[j].format));
                tprcs->Setid(StringFromAscIIChars(grp.resources[j].id));
                tprcs->Setcontent(UTF8_To_Managed_Str(grp.resources[j].content));
                tprcs->Setplay_count(grp.resources[j].play_count);
                tprcs->Setoffset(grp.resources[j].offset);
                tprcs->Setextend_buffer(UTF8_To_Managed_Str(grp.resources[j].extend_buffer));
                tprgcs->GetResource()->Append(tprcs);
            }
            state_info_response_cs->Getresource_groups()->Append(tprgcs);
        }
        state_info_response_cs->Setresource_groups_size(param_rsp->resource_groups_size);
        state_info_response_cs->Setrsp_text(UTF8_To_Managed_Str(param_rsp->response_text));
        state_info_response_cs->Sethas_more_playlist(param_rsp->has_more_playlist);
        state_info_response_cs->Setis_recovery(param_rsp->is_recovery);
        state_info_response_cs->Setis_notify(param_rsp->is_notify);
        state_info_response_cs->Setwakeup_flag(param_rsp->wakeup_flag);
        state_info_response_cs->Setplay_behavior((TXCA_PLAYLIST_ACTION_CS)param_rsp->play_behavior);
        state_info_response_cs->Setresource_list_type((int)(param_rsp->resource_list_type));
        state_info_response_cs->Setcontrol_id(param_rsp->control_id);
        state_info_response_cs->Setcontrol_value(UTF8_To_Managed_Str(param_rsp->control_value));
        state_info_response_cs->Sethas_more_playlist_up(param_rsp->has_more_playlist_up);
    }

    return callBackOnRequest(StringFromAscIIChars(voice_id), eventcs, state_info_response_cs, UTF8_To_Managed_Str(extend_info), extend_info_len);
}

bool ClassDeviceSDK::on_net_delay_callback(const char* voice_id, unsigned long long time) {
    LogMessage("on_net_delay_callback");
    return callBackOnNetDelay(StringFromAscIIChars(voice_id), time);
}

void ClassDeviceSDK::on_cmd_request_callback(const char* voice_id, int xiaowei_err_code, const char* json) {
    LogMessage("on_cmd_request_callback");
    callBackOnRequestCMD(StringFromAscIIChars(voice_id), xiaowei_err_code, UTF8_To_Managed_Str(json));
}

void ClassDeviceSDK::on_device_log(int level, const char* tag, const char* filename, int line, const char* funcname, const char* data, unsigned int len) {
    //char msg[20000];
    //_snprintf_s(msg, 20000, 20000, "[%d] %s %s[%d] %s %s", level, tag, filename, line, funcname, data);
    //OutputDebugStringA(msg);
    callBackOnLog(level, StringFromAscIIChars(tag), StringFromAscIIChars(filename), line, StringFromAscIIChars(funcname), StringFromAscIIChars(data));
}


/* help functions */
void ClassDeviceSDK::fill_binder_list(IVector<TX_BINDER_INFO_CS^>^ binderList, const DEVICE_BINDER_INFO* pBinderList, int nCount) {
    //binderList = ref new Platform::Collections::Vector<TX_BINDER_INFO_CS^>();
    for (int i = 0; i < nCount; ++i)
    {
        TX_BINDER_INFO_CS^ tx_binder_info = ref new TX_BINDER_INFO_CS();
        tx_binder_info->Settype(pBinderList[i].type);
        tx_binder_info->Setuser_name(StringFromAscIIChars(pBinderList[i].username));
        tx_binder_info->Setnick_name(UTF8_To_Managed_Str(pBinderList[i].nickname));
        tx_binder_info->Setgender(pBinderList[i].sex);
        tx_binder_info->Sethead_url(StringFromAscIIChars(pBinderList[i].head_img));
        tx_binder_info->Settype(pBinderList[i].type);
        binderList->Append(tx_binder_info);
    }
}