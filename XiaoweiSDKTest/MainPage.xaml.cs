using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using sdkrtv2;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace XiaoweiSDKTest
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        ClassDeviceSDK classDeviceSDK;
        Boolean loginComplete = false;  
        public MainPage()
        {
            Trace.WriteLine("start test");
            this.InitializeComponent();
            CS_Init();
        }
        private void CS_Init()
        {
            Trace.WriteLine("Init sdk");
            classDeviceSDK = new ClassDeviceSDK();
            var v =  classDeviceSDK._device_get_sdk_version();
            Trace.Write("sdk version is ");
            Trace.WriteLine(v);
            ClassDeviceSDK.callBackOnLog += (level, tag, filename, line, funcname, data) =>
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("[").Append(level).Append("/").Append(tag).Append(" ").Append(filename).Append(" ").Append(line).
                Append(" ").Append(funcname).Append("] ").Append(data);
                Trace.WriteLine(sb.ToString());
            };
            Trace.WriteLine("init call");
            int ret = classDeviceSDK._device_init("890d1af6be6cc5455261c5cfa8a2453d",
                "MEYCIQDzD4ty1TMn4IQ5LuzUUzofWbm7m3mg73GmWBNu4/e7aQIhAPKqcNz5JKuDdmouS1oc/OZdzfOb9iLRZ5ajjpsvabNX",
               "ilinkapp_060000873458be",
               30,2,1,0);
            Trace.Assert(ret == 0);
            ClassDeviceSDK.callBackOnLoginComplete += (err) =>
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("callBackOnLoginComplete, err = ").Append(err);
                Trace.WriteLine(sb.ToString());
                Trace.Assert(err == 0);
                loginComplete = true;
                CS_TestTextReq();
            };
            ClassDeviceSDK.callBackOnNetStatusChange += (status) =>
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("callBackOnNetStatusChange, status = ").Append(status == 0 ? "offline" : "online");
                Trace.WriteLine(sb.ToString());
            };
            ClassDeviceSDK.callBackOnQrCodeRefresh += (error_code, qr_code_path, expire_time, expire_in) =>
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("callBackOnQrCodeRefresh, errcode = ").Append(error_code).Append(" ,qrCodePath = ").Append(qr_code_path)
                .Append(" expire unix time is ").Append(expire_time).Append(" , and will expire in ").Append(expire_in).Append(" seconds");
                Trace.WriteLine(sb.ToString());
            };
            ClassDeviceSDK.callBackOnRequest += (voice_id, evt, state_info, extend_state_info, extend_state_info_len) =>
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("callBackOnRequest called.").Append("\n");
                sb.Append("voiceid = ").Append(voice_id).Append("\n");
                sb.Append("errcode = ").Append(state_info.GetErrorCode()).Append("\n");
                sb.Append("voiceid = ").Append(voice_id).Append("\n");
                if (TXCA_EVENT_CS.txca_event_on_response == evt)
                {
                    for (int i = 0; i < state_info.Getresource_groups_size(); i++)
                    {
                        var grp = state_info.Getresource_groups().ElementAt(i);
                        for (int j = 0; j < grp.GetResourceSize(); j++)
                        {
                            sb.Append(grp.GetResource().ElementAt(j).Getformat()).Append(grp.GetResource().ElementAt(j).Getid()).Append(
                                grp.GetResource().ElementAt(j).Getcontent()).Append("\n");
                        }
                    }

                   Trace.WriteLine(sb.ToString());
                }
                else
                {
                    sb.Append("status is ").Append(evt);
                    if (extend_state_info_len > 0)
                    {
                        sb.Append(extend_state_info).Append(" \n");
                    }
                    else
                    {
                        sb.Append(" with empty extend_state_info").Append(" \n");
                    }
                    Trace.WriteLine(sb.ToString());
                }
                return true;   
            }; 
        }

        private void CS_TestTextReq()
        {
            Trace.WriteLine("test text request");
            VOICE_ID_CS voiceid = new VOICE_ID_CS();
            TXCA_PARAM_CONTEXT_CS context_cs = new TXCA_PARAM_CONTEXT_CS();
            context_cs.Setvoice_request_begin(true);
            String request = "放一首歌";
            byte[] request_byte = new byte[request.Length];
            for (int i = 0; i < request.Length; i++)
            {
                request_byte[i] = (byte)request[i];
            }
            var ret = classDeviceSDK._xiaowei_request(voiceid, TXCA_CHAT_TYPE_CS.txca_chat_via_text, request_byte,
               (uint)request.Length, context_cs);
            StringBuilder sb = new StringBuilder();
            Trace.WriteLine(sb.Append("test text request, result = ").Append(ret).Append("voiceid is ").Append(voiceid.Get()));
        }

        private int ClassDeviceSDK_callBackOnLog(int level, string tag, string filename, int line, string funcname, string data)
        {
            throw new NotImplementedException();
        }

        private void BtnClick_GetQrCode(object sender, RoutedEventArgs e)
        {
            classDeviceSDK._device_refresh_qr_code();
        }

        private void BtnClick_GetIotDevie(object sender, RoutedEventArgs e)
        {
            Trace.WriteLine($"BtnClick_GetIotDevie");
            classDeviceSDK._xiaowei_get_iot_device_list();
        }

        private void BtnClick_GetDin(object sender, RoutedEventArgs e)
        {
            var din = classDeviceSDK._device_get_uin();
            Trace.WriteLine($"din:{din}");
        }
    }
}
