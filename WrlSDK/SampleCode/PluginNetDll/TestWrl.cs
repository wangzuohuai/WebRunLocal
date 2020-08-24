using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

using WrlEngine;
using WrlBase;

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
                WriteLog("PluginNetDll", "成功获得WS接口");
            else
                 WriteLog("PluginNetDll", "获取WS接口失败");
        }

        void IWrlConn.Unload(WrlBase.EWrlCloseConnType eCloseConnType, string bstrReason)
        {
            m_WebSocketConnect = null;
            WriteLog("PluginNetDll", "请求卸小程序");
        }

        string IWrlConn.HttpRequst(string bstrUrl, string bstrPara)
        {
            /// 收到HTTP类型请求，暂未实现
            return "";
        }

        void IWrlConn.RecByte(ref byte pContent, uint nLen)
        {
            /// 收到二进制数据流，特殊用途
            WriteLog("PluginNetDll", "我不是机器人！");
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
                strReturn = "收到请求 ";
                strReturn += bstrPushName;
                strReturn += " 内容：";
                strReturn += bstrContent;
                strReturn += " 序号：";
                strReturn += nReqID.ToString();
            }
            else if(bstrPushName == "Demo_WriteFile")
            {
                string strName = JsonService.GetStringValue("Name");
                string strContent = JsonService.GetStringValue("Content");

                string strAppPath = this.GetType().Assembly.Location;
                strAppPath.Replace(".exe","");
                strAppPath += "\\Data\\";
                StreamWriter swFile = new StreamWriter(strAppPath + strName, true, Encoding.Unicode);
                swFile.WriteLine(strContent);
                swFile.Close();
                swFile.Dispose();
                strReturn = strAppPath + strName + "写入内容" ;
            }
            else
                strReturn = "收到未知请求:" + bstrPushName;

            WriteLog(bstrPushName, strReturn);
            /// 给前端回复请求
            m_WebSocketConnect.AsynSendText(strReturn);
        }

        void IWrlConn.RecText(string bstrContent)
        {
            /// 收到文本请求，一般都是JSON包
            m_WebSocketConnect.AsynSendText("你确定没发错？");
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