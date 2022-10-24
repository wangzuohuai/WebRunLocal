using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

using ZbaEngine;
using ZbaBase;

[Guid("FCADA8A7-D975-42c6-B5D3-FA40F6FE1286")]
[TypeLibType(4160)]
[ComVisible(true)]
public interface ITestWrl
{
}

namespace PluginNetDll
{
    [Guid("5DDA3CFF-6AD1-4836-B378-628B43748626")]
    [TypeLibType(4160)]
    [ClassInterface(ClassInterfaceType.None)]
    [ComDefaultInterface(typeof(ITestWrl))]
    [ComSourceInterfaces(typeof(ITestWrl))]
    public class TestWrl : ITestWrl, IWrlConn
    {
        /// <summary>
        ///  Web Socket连接对象
        /// </summary>
        IWebSocketConnect m_WebSocketConnect;

        #region IWrlConn Members
        void IWrlConn.Load(object piDispatch, string bstrAuthInfo, string bstrLang)
        {
            m_WebSocketConnect = piDispatch as IWebSocketConnect;
            if (null != m_WebSocketConnect)
                WriteLog("PluginNetDll", "The WS interface was successfully obtained");
            else
                WriteLog("PluginNetDll", "Failed to get the WS interface");
        }

        void IWrlConn.Unload(EWrlCloseConnType eCloseConnType, string bstrReason)
        {
            m_WebSocketConnect = null;
            WriteLog("PluginNetDll", "Request a release of the applet");
        }

        string IWrlConn.HttpRequst(string bstrUrl, string bstrPara)
        {
            WriteLog(bstrUrl, bstrPara);
            /// 收到HTTP协议请求，主要用于前端同步请求，比如前端需要等待请求完成浏览器才能继续操作
            /// 可根据传递的参数分别执行不同的功能，在DLL小程序中，如需要弹窗执行，请先修改中间件配置文件参数NoService为1后再重新安装中间件生效！
            return "{\"ret\":0,\"data\":{\"Ret\":0,\"Code\":1}}";
        }

        void IWrlConn.RecByte(ref byte pContent, uint nLen)
        {
            /// 收到二进制数据流，特殊用途
            WriteLog("PluginNetDll", "A binary data stream is received");
        }
        /// <summary>
        /// 重点处理此函数
        /// </summary>
        /// <param name="nReqID"></param>
        /// <param name="bstrPushName"></param>
        /// <param name="bstrContent"></param>
        void IWrlConn.RecJson(uint nReqID, string bstrPushName, string bstrContent)
        {
            /// 创建JSON解析器
            JsonServiceClass JsonService = new JsonServiceClass();

            bool bRet = JsonService.ParseString(bstrContent);
             /// 根据请求名称bstrPushName和请求参数bstrContent分别定义自己的协议进行处理。
            string strReturn = "";
            if(bstrPushName == "Demo_Return")
            {
                strReturn = "Requested:";
                strReturn += bstrPushName;
                strReturn += " content:";
                strReturn += bstrContent;
                strReturn += " number:";
                strReturn += nReqID.ToString();
            }
            else if(bstrPushName == "Demo_WriteFile")
            {
                string strName = JsonService.GetStringValue("Name");
                string strContent = JsonService.GetStringValue("Content");

                string strAppPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data");
                if (!System.IO.Directory.Exists(strAppPath))
                    System.IO.Directory.CreateDirectory(strAppPath);
                StreamWriter swFile = new StreamWriter(strAppPath + "\\" + strName, true, Encoding.Unicode);
                swFile.WriteLine(strContent);
                swFile.Close();
                swFile.Dispose();
                strReturn = strAppPath + "\\" + strName + ",The contents of the file are written";
            }
            else
                strReturn = "Receive an unknown request:" + bstrPushName;

            /// 给前端回复请求
            m_WebSocketConnect.AsynSendText(strReturn);
        }

        void IWrlConn.RecText(string bstrContent)
        {
            /// 收到文本请求，一般都是JSON包
            m_WebSocketConnect.AsynSendText("Are you sure you didn't make a mistake?");
        }

        /// <summary>
        ///  框架调用发送通知
        /// </summary>
        /// <param name="bstrContent"></param>
        void IWrlConn.SendJson(string bstrContent)
        {
            if (null != m_WebSocketConnect)
                m_WebSocketConnect.AsynSendText(bstrContent);
         }

        void IWrlConn.UsbChanged(string bstrDisk, uint nStatus, string bstrName)
        {
            /// USB存储设备插拔通知
        }

     #endregion

        public void WriteLog(string documentName, string msg)
        {
            string LogFilePath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data");
            if (!System.IO.Directory.Exists(LogFilePath))
                System.IO.Directory.CreateDirectory(LogFilePath);
            string logFile = System.IO.Path.Combine(LogFilePath, documentName + "@" + DateTime.Today.ToString("yyyy-MM-dd") + ".txt");
            bool writeBaseInfo = System.IO.File.Exists(logFile);
            StreamWriter swLogFile = new StreamWriter(logFile, true, Encoding.Unicode);
            swLogFile.WriteLine(DateTime.Now.ToString("HH:mm:ss") + "\t" + msg);
            swLogFile.Close();
            swLogFile.Dispose();
        }
    }
}