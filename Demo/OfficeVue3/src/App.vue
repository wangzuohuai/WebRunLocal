<template>
  <div id="app">
    <div class="mainContainer">
      <div class="tool">
        <div class="item">显示宽度</div>
        <div class="item">
          <el-input v-model="width" size="small" class="input" @blur="resize(0)"></el-input>
        </div>
        <div class="item">显示高度</div>
        <div class="item">
          <el-input v-model="height" size="small" class="input" @blur="resize(0)"></el-input>
        </div>
        <div class="item">左边距</div>
        <div class="item">
          <el-input v-model="left" size="small" class="input" @blur="resize(1)"></el-input>
        </div>
        <div class="item">上边距</div>
        <div class="item">
          <el-input v-model="top" size="small" class="input" @blur="resize(1)"></el-input>
        </div>
        <div class="item">
          <el-button size="small" @click="AddMark">添加书签</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="MarkRePlace">替换书签内容</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="ConvertFirstPage">转首页为图片</el-button>
        </div>
        <el-button size="small" v-if="StartSecond" @click="CloseSecondApplet">
          关闭Excel网页组件
        </el-button>
        <el-button size="small" @click="openSecondApplet" v-else>
          启动Excel网页组件
        </el-button>
      </div>
      <div class="tool">
        <div class="item">
          <el-button size="small" @click="InsertContent">插入内容</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="SaveFile">保存文档</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="SaveAsFile">另存文档</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="ExpportFile">导出文档</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="InsertImg">插入图片印章</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="GetFirstImg">提取首图</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="PrintFile">打印文档</el-button>
        </div>
        <el-button size="small" v-if="IsRevision" @click="DisableRevision">
          关闭留痕
        </el-button>
        <el-button size="small" @click="EnableRevision" v-else>
          启用留痕
        </el-button>
        <div class="item">
          <el-button size="small" @click="ShowRevision">显示留痕</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="AcceptRevision">接受留痕</el-button>
        </div>
        <div class="item">
          <el-button size="small" @click="DocRed">公文套红</el-button>
        </div>
      </div>
      <div class="urlbox">
        <div class="item">文档路径</div>
        <div class="item input">
          <el-input v-model="MyOpenDoc" placeholder="这里演示切换文档" size="small"></el-input>
        </div>
        <div class="item" style="margin-left: 10px;">
          <el-button type="primary" @click="changeOpen()" size="small">切换打开</el-button>
        </div>
        <div class="item" style="margin-left: 10px;">
          <el-button size="small" @click="ReLoadFirst()" type="primary">完整嵌入</el-button>
        </div>
      </div>
      <div class="video-container" ref="player">
        办公网页组件区域
      </div>
      <div class="tool">
        <div class="item" style="margin-left: 10px;">
          <el-button size="small" @click="AppletFullEdit()" type="primary">全屏编辑</el-button>
          <el-button type="primary" @click="CloseAllApplet()" size="small">关闭所有网页组件</el-button>
        </div>
        <div class="item">最新版本号</div>
        <div class="item">
          <el-input v-model="version" size="small"></el-input>
        </div>
        <el-button size="small" @click="CheckUpdate" type="primary">校验升级</el-button>
      </div>
      <div class="urlbox">
        <div class="item">叠加透明网页</div>
				  <div class="item input">
					  <el-input v-model="newfloatweb" placeholder="这里演示叠加透明网页内容显示" size="small"></el-input>
				  </div>
				  <div class="item" style="margin-left: 10px;">
					  <el-button type="primary" @click="FloatWebInfo()" size="small">叠加显示</el-button>
				  </div>
			  </div>
      <el-input type="textarea" :rows="5" placeholder="调试日志" v-model="DebugLog" class="DebugLog">
      </el-input>
    </div>
  </div>
</template>
<script setup>
//集成开发说明：所有交互都是通过JS建立websocket连接后，发送或接收JSON包进行，都是异步请求，发送和接收包都携带rid用来标识请求返回包对应是哪个请求的结果
//所有发送的msg中用到的长整形rid 可以自己指定 不同的发送请求定义唯一的rid 这样可以在回调用轻松判断
//针对一些特殊请求，可以根据自己的实际情况把rid固定，如启动网页组件时指定固定rid获得启动网页组件的实例ID
//首先建立第一个websocket连接到中间件服务端口启动办公网页组件 然后获得启动网页组件的侦听端口建立第二个websocket连接 主要用来进行控制保存文档、提取图片、切换打开文档等操作
//释放网页组件，直接关闭建立的第一个websocket连接即可
import GetDefaultConn from "./common/base.js"
//加载websoket类
import websocket from './common/websocket'
import {ref,reactive, computed,onMounted,onUnmounted} from 'vue'
import { ElMessage,ElMessageBox } from 'element-plus'

const player = ref()
let aid = 0		// 第一个办公网页组件实例ID
let aid2 = 0		// 第二个办公网页组件实例ID
let curID = 0 		// 当前操作实例ID
let rid = 10 		// 其它请求起始序号
let runInfo = 1    	// 获取办公软件安装信息 一般只需要在启动时获取一次
let RunFirst = 2  	// 办公网页组件启动实例1序号
let RunSecond = 3   	// 办公网页组件启动实例2序号
let version = ref('2.2.17.6')   //中间件版本信息
let ServerOpenFile = 'http://local.zorrosoft.com/Files/template.doc'
let MyOpenDoc = ref('c:/OfficeDoc/test.docx')
let MyOpenExcel = 'c:/OfficeDoc/test.xlsx'
let SaveAsDoc = 'c:/OfficeDoc/SaveAs.doc'
let ExportDoc = 'c:/OfficeDoc/test.pdf'
let newfloatweb = ref('https://output.jsbin.com/dopavun')
let InsertImgFile = 'http://zorrosoft.com/wp-content/uploads/2021/07/2021072709255099-1024x576.png'
let PrintPathFile = 'c:/OfficeDoc/testprint.pdf'
let StartSecond = ref(0)  // 是否启动了第二个网页组件实例
let ReStartLoad = 0   // 第一个网页组件实例是否重新打开状态
let width = ref(960) 		// 网页组件窗口显示宽度
let height = ref(480)	// 网页组件窗口显示高度
let left = ref(0)		// 网页组件窗口以网页左上角为基点显示的X位置
let top = ref(0)		// 网页组件窗口以网页左上角为基点显示的Y位置
let IframeX = -10   	// 网页组件窗口模拟iFrame用的X坐标
let IframeY = 0  		// 网页组件窗口模拟iFrame用的Y坐标
// 加载类型 可先通过请求Wrl_OfficeInfo获得当前需要设置的值
// 1启动微软Word 2启动微软Excel 3启动微软PPT 5启动Adobe Pdf Reader或福昕OFD版式办公套件 8启动福昕OFD版式办公套件的OCX控件打开ofd
// 11启动金山文字 12启动金山表格 13启动金山演示 15启动Adobe Pdf Reader或福昕OFD版式办公套件 16启动WPS PDF程序打开pdf(不支持OLE嵌入方式，必须用WPS专业版)   18启动福昕OFD版式办公套件的OCX控件打开ofd
// 21启动永中文字 22启动永中表格 23启动永中简报 25启动Adobe Pdf Reader或福昕OFD版式办公套件  28启动福昕OFD版式办公套件的OCX控件打开ofd
let OpenType = 1
let SelectDlgType = 0   // 选择对话框类型 0另存 1导出 2插入图片 3提取首张图片 4转首页为图片
let IsRevision = ref(false)  	// 是否留痕
let isConnService = false //是否连接的中间件服务
let isDisConnect = false // 是否处于断开连接过程
let socket =[] 	//websocket对象数组，采用数组需要注意维护索引号，建议改为根据连接的SID参数存到对应的集合，需要用到连接时根据SID提取
let result = reactive([])  		//日志结果数组

const DebugLog = computed(() => {
  return result.join("\n")
})

onMounted(()=>{
  //初始化配置
  init()
  /// 响应改变浏览器窗口大小
  window.onresize = ()=>{
    pageResize()
  }
})

onUnmounted(()=>{
  //关闭所有websocket链接以及浏览器监听
  close()
  window.onresize = null
})

function init() {
  //监听浏览器切换标签页面
  if(document.addEventListener)
    document.addEventListener('visibilitychange', handleVisiable,false)
  else
    document.attachEvent('visibilitychange',handleVisiable,false)	/// 老版本浏览器
  //侦听滚动条
  window.addEventListener('scroll', windowScroll, true)
  //监听当页面离开时候
  window.addEventListener('unload', unloadHandler,false)
  //获取网页组件位置节点信息
  GetAppletPosition()
  //先获取本机Office软件安装信息
  GetOfficeInfo()
}

function windowScroll() {
  // 滚动条距离页面顶部的距离
  let scrollTop = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop
  if(aid > 0)
    appScroll(0, aid, scrollTop)
  if (aid2 > 0)
    appScroll(2, aid2, scrollTop)
}

function appScroll(si, id, scrollTop) {
  if(id)
  {
    // 默认纵向滚动网页组件实例，如需要横向滚动，Code设置为1，修改Left的值
    // NoLog指示服务日志不输出相关日志，因为时间比较多，输出日志导致日志文件信息过多
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletScroll",
      "rid": rid,
      "para": {
        "ID": id,
        "NoLog": 1,
        "Code": 2,
        "Left": 0,
        "Top": Math.round(scrollTop)
      }
    }

    socket[si].sendObj(msg)
  }
}

function GetAppletPosition() {
  //获取网页组件位置节点信息
  let nScrollTop = 0
  let nScrollLeft = 0
  let bHorizontalBar =hasHorizontalScrollbar()
  let bVerticalBar = hasVerticalScrollbar()
  if(bHorizontalBar){
    if(window.pageXOffset != undefined)
      nScrollLeft = Math.round(window.pageXOffset)
    else
    {
      //获取页面的scrollLeft(兼容写法)
      nScrollLeft = Math.round(document.documentElement.scrollLeft || document.body.scrollLeft)
    }
  }
  if(bVerticalBar){
    if(window.pageYOffset != undefined)
      nScrollTop = Math.round(window.pageYOffset)
    else
    {
      //获取页面的scrollTop(兼容写法)
      nScrollTop = Math.round(document.documentElement.scrollTop || document.body.scrollTop)
    }
  }

  let react = player.value.getBoundingClientRect()
  console.log(react)
  left.value = Math.round(react.left + nScrollLeft)
  top.value = Math.round(react.top + nScrollTop)
  width.value = Math.round(react.width)
  height.value = Math.round(react.height)
}

function handleVisiable(e) {
  //浏览器页面切换侦听回调函数
  if (e.target.visibilityState == 'hidden') {
    //切离该页面时执行 当前页自己实现标签切换或需要在办公程序窗口区域临时显示内容时，设置32强制隐藏
    hideApp(4)
  } else if (e.target.visibilityState == 'visible') {
    //切换到该页面时执行
    showApp()
  }
}

function hasVerticalScrollbar(){
  if(document.documentElement.clientHeight)
    return document.body.scrollHeight > document.documentElement.clientHeight
  return document.body.scrollHeight > window.innerHeight
}

function hasHorizontalScrollbar(){
  if(document.documentElement.clientWidth)
    return document.body.scrollWidth > document.documentElement.clientWidth
  return document.body.scrollWidth > window.innerWidth
}

function pageResize(){
  if(aid > 0)
    SendScrollInfo(0,aid)
  if(aid2 > 0)
    SendScrollInfo(2,aid2)
}

function SendScrollInfo(si,id){
  let nScrollTop = 0
  let nScrollLeft = 0
  let BarCode = 0
  let bHorizontalBar = hasHorizontalScrollbar()
  let bVerticalBar = hasVerticalScrollbar()
  if(bHorizontalBar){
    if(window.pageXOffset != undefined)
      nScrollLeft = Math.round(window.pageXOffset)
    else
    {
      //获取页面的scrollLeft(兼容写法)
      nScrollLeft = Math.round(document.documentElement.scrollLeft || document.body.scrollLeft)
    }
  }
  if(bVerticalBar){
    if(window.pageYOffset != undefined)
      nScrollTop = Math.round(window.pageYOffset)
    else
    {
      //获取页面的scrollTop(兼容写法)
      nScrollTop = Math.round(document.documentElement.scrollTop || document.body.scrollTop)
    }
  }
  if(bHorizontalBar)
    BarCode = 1
  if(bVerticalBar)
    BarCode += 2
  rid++ // 增加序号
  /// 设置页码滚动信息，BarW BarH分别为预留右侧宽度和底部高度
  let msg = {
    "req": "Wrl_ScrollBar",
    "rid": rid,
    "para": {
      "ID": id,
      "BarW": 0,
      "BarH": 0,
      "Code": BarCode,
      "Left": nScrollLeft,
      "Top": nScrollTop
    }
  }
  console.log(msg)
  socket[si].sendObj(msg)
}

function unloadHandler() {
  //关闭所有websoket链接
  close()
}

function close() {
  //关闭网页组件实例
  CloseAllApplet()
  isDisConnect = true
  //关闭第一个websoket链接
  socket[0].disconnect()
  socket.pop()
  isDisConnect = false
  //关闭侦听浏览器页面切换
  if(document.addEventListener)
    document.removeEventListener('visibilitychange', handleVisiable,false)
  else
    document.detachEvent('visibilitychange',handleVisiable,false)	/// 老版本浏览器
  //关闭侦听滚动条
  window.removeEventListener('scroll', windowScroll), true
  //关闭侦听滚动条
  window.removeEventListener('unload', unloadHandler,false)
}


function GetOfficeInfo()
{
  isConnService = true
  let Protocol = location.protocol;
  if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
  {
    // HTTPS网站，连接WSS侦听端口
    openWebsocket(453,0)
  }
  else
  {
    openWebsocket(83,0)
    //openWebsocket(83,1) 支持安全校验机制
  }
  let msg = {
    "req": "Wrl_OfficeInfo",
    "rid": runInfo,
    "para": {}
  }
  socket[0].sendObj(msg)
}

function StartOfficeApplet() {
  //启动第一个办公网页组件
  // Type为浏览器类型，传0自动判断(前提是当前浏览器已启动并显示在最前端，Flag指定当前页加载时必须是0) 可强制指定浏览器类型Type(1代表IE 2代表Chrome 4代表Firefox 5代表Brave(英文版) 8代表Opera 9代表Vivaldi(英文版) 10代表Yandex 16代表Edge(Chromium内核) 20代表Electron 32代表360极速浏览器 33代表360安全浏览器 34代表360极速浏览器 35代表360企业安全浏览器 36代表360游戏浏览器 37代表360AI浏览器 40代表联想浏览器 45代表双核浏览器 50代表QQ浏览器 51代表微信网页 55代表齐安信浏览器 57代表红莲花浏览器 60代表搜狗浏览器 61代表夸克 62代表遨游浏览器 63代表猎豹浏览器 66代表豆包 70代表华为浏览器)
  // Type需要指定浏览器类型时，可先通过TestWrl.txt文档中的接口Wrl_BrowserInfo获取到当前浏览器的类型Type、浏览器主窗口句柄BrowserWnd、浏览器绘制窗口句柄DrawWnd及当前网页标题Title，前端可通过判断Title是否为当前网页需要内嵌决定是否继续启动，如果继续启动，把获取到的参数Type、BrowserWnd、DrawWnd设置到当前请求参数里，可以加快启动速度
	// Title：网页标题中的关键词
  // Flag掩码：1指定新标签加载(1和16都不指定时为当前页加载) 2显示标题栏 4不自动裁剪越界窗口 8自动适配网页高度和宽度显示 64启用Web参数 128防截屏 256强制显示到副屏 512允许同一网页加载多实例
  // Version：0:OLE嵌入方式加载 1:完整加载(支持文档对比操作) 2:OLE嵌入方式加载(与0差别在于WPS专业版及微软Office高版本提前启动Office进程以支持多文档及加快启动性能) Option为5时Version自动默认为1
  // Option：对应变量openType的说明
  // Open：为需要打开的文档路径，磁盘目录斜杠用/ 支持服务器HTTP协议的文件下载路径(请确保无需登录即可下载)，下载地址中如果没有包含文件名，请在Content-Disposition中指定filename
  // 此参数如非全路径，包括服务器路径，除非启动时指定了数据文件目录DataPath，否则默认使用中间件程序Data子目录作为根目录使用 也可在Web中指定
  // 注意：Open、Url中如果有特殊字符= & 双引号或中文等，需要用URL编码处理后传递
  // BarW和BarH分别是网页右侧和底部预留区域，ScrollTop为顶部滚动预留高度
  // Web节点中参数可自行配置，目前支持这些参数：
  // 		Edit代表编辑权限(除只读1、512、1024都支持外，其它只有WORD才支持，4、8和1024只有在OLE嵌入方式下生效 其中1、2、16、32、64、128互斥) 1只读方式打开 2打开后自动处于修订模式 4禁止另存 8禁止打印 16禁止复制内容 32只能修订 64只能批注 128预览模式 256禁止粘贴内容出编辑窗口 512在PageHi菜单支持上传下载文档 1024安全文档(载入后和上传后自动删除本地文档) 2048禁用全屏编辑
  // 		Hide代表隐藏的界面元素 1隐藏菜单 2隐藏标准工具栏 4隐藏格式工具栏 8隐藏状态栏 16隐藏右键菜单
  // 		User代表用户名
  // 		PW代表打开文档需要的密码
  // 		DataPath代表文档查找和保存默认路径
  //    WaterMark 打开或新建文件后自动加水印节点信息，内容和接口Office_InsertWaterMark一致
  let msg = {
    "req": "Wrl_OfficeApplet",
    "rid": RunFirst,
    "para": {
      "Type": "0",
      "Title": "办公网页组件(Word)",
      "Version": 2,//0使用OLE嵌入不先启动OLE Server 1是完整嵌入(有文件菜单和文档对比等更多功能可用) 2使用OLE嵌入，预先启动OLE Server，加快启动速度 尽可能支持多文档操作
      "Flag": 578,
      "Left": left.value,
      "Top": top.value,
      "Width": width.value,
      "Height": height.value,
      "IframeX": IframeX,
      "IframeY": IframeY,
      "BarW": 0,
      "BarH": 0,
      "ScrollTop": 0,
      "Web": {"Edit":0,"Hide":0,"User":"test","Cookie":"","DataPath":"c:/OfficeDoc"},
      "Option": OpenType,
      "Open": encodeURIComponent(ServerOpenFile)
    }
  }
  console.log(msg)
  socket[0].sendObj(msg)
}

function openSecondApplet() {
  if(aid)
  {
    //演示加载第二个办公网页组件实例打开表格程序 先改变第一个网页组件位置 Flag值需要在原来基础上+512以支持多实例加载
    width.value = 475
    resize(0)
    isConnService = true
    let Protocol = location.protocol;
    if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
    {
      // HTTPS网站，连接WSS侦听端口
      openWebsocket(453,0)
    }
    else
    {
      openWebsocket(83,0)
      //openWebsocket(83,1) 支持安全校验机制
    }

    let msg = {
      "req": "Wrl_OfficeApplet",
      "rid": RunSecond,
      "para": {
        "Type": "0",
        "Title": "办公网页组件(Excel)",
        "Version": 2,
        "Flag": 578,
        "Left": left.value + 485,
        "Top": top.value,
        "Width": width.value,
        "Height": height.value,
        "IframeX": -10,
        "IframeY": 0,
        "BarW": 0,
        "BarH": 0,
        "ScrollTop": 0,
        "Web": {"Edit":0,"Hide":0,"User":"zorro","Cookie":"","DataPath":"c:/OfficeDoc"},
        "Open": encodeURIComponent(MyOpenExcel),
        "Option": OpenType+1
      }
    }
    socket[2].sendObj(msg)
    StartSecond.value = 1
  }
  else{
    ElMessage('请先启动第一个网页组件')
  }
}

function CloseSecondApplet() {
  if (StartSecond.value) {
    isDisConnect = true
    /// 每个网页组件实例占用2个连接，一个到中间件、一个到网页组件，分别断开连接并释放
    socket[3].disconnect()
    socket[2].disconnect()
    socket.pop()
    socket.pop()
    isDisConnect = false
    StartSecond.value = 0
    aid2 = 0
    //还原第一个网页组件位置
    width.value = 960
    height.value = 480
    //获取网页组件位置节点信息
    let react = player.value.getBoundingClientRect()
    left.value = react.left
    top.value = react.top
    resize(0)
  }
}

function CloseFirstApplet()
{
  if (aid > 0)
  {
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletControl",
      "rid": rid,
      "para": {
        "ID": aid,
        "Code": 1
      }
    }
    isDisConnect = true
    /// 释放网页组件的连接
    socket[1].disconnect()
    socket[0].sendObj(msg)
    aid = 0
    ReStartLoad = 0
    if(StartSecond.value)
      socket.splice(1,1)
    else
      socket.pop()
    isDisConnect = false
  }
}

function FloatWebInfo() 
{
	rid++ // 增加请求序号
	let msg = {
		"req": "Office_FloatWebInfo",
		"rid": rid,
		"para": {
			"Url": encodeURIComponent(newfloatweb.value),
			"Rect":{"P":4,"W":300,"H":300}
		}
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function AppletFullEdit()
{
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_SwitchFullScreen",
    "rid": rid,
    "para": {}
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function ReLoadFirst()
{
  CloseFirstApplet()
  ReStartLoad = 1
  let Msg = {
    "req": "Wrl_OfficeApplet",
    "rid": RunFirst,
    "para": {
      "Type": "0",
      "Title": "办公网页组件(Word)",
      "Version": 1,
      "Flag": 578,
      "Left": left.value,
      "Top": top.value,
      "Width": width.value,
      "Height": height.value,
      "IframeX": IframeX,
      "IframeY": IframeY,
      "BarW": 0,
      "BarH": 0,
      "ScrollTop": 0,
      "Web": {"Edit":0,"Hide":0,"User":"zorro","Cookie":"","DataPath":"c:/OfficeDoc"},
      "Open": encodeURIComponent(MyOpenDoc.value),
      "Option": OpenType
    }
  }
  socket[0].sendObj(Msg)
}

function CloseAllApplet()
{
  /// 先关闭第二个实例，否则socket中保存的连接序号会不正常
  CloseSecondApplet()
  CloseFirstApplet()
}

function openWebsocket(port,type) {
  //打开websocket服务
  let ws = GetDefaultConn(port,type)
  const socketClient = new websocket(ws, {
    reconnectEnabled: false
  })
  socketClient.connect()

  if(1 == ReStartLoad && StartSecond.value)
  {
    ReStartLoad = 2
    socket.splice(1,0,socketClient) // 重新加载时已经启动第二个实例，采用插入方式，避免WS下标错误，确保第一个实例的WS连接下标是0和1，第二个实例是3和4
  }
  else
  {
    if(1 == ReStartLoad)
      ReStartLoad = 2
    socket.push(socketClient)
  }
  socketClient.onMessage = (msg) =>
  {

    let res = JSON.parse(msg.data)
    console.log(res)
    if(res.rid == runInfo)
    {
      // 解析当前Office安装情况
      for(var OfficeItem in res.data.Info)
      {
        if(1 == res.data.Info[OfficeItem].Cur)
        {
          OpenType = res.data.Info[OfficeItem].Type
        }
      }
      result.push("当前Office类型 " + OpenType)
      StartOfficeApplet()
    }
    if (res.event == 'Wrl_AppletOK') {
      //网页组件创建成功后根据事件名称获取当前网页组件id
      if (res.rid == RunFirst) {
        aid = res.aid
        SendScrollInfo(0,aid)
        console.log(aid)
      }
      if (res.rid == RunSecond) {
        aid2 = res.aid
        SendScrollInfo(2,aid2)
        console.log(aid2)
      }
    }
    if (res.event == 'Wrl_Listen') {
      //网页组件建立侦听成功 这里得到当前网页组件返回的端口
      //这里创建另外一个websocket到办公网页组件端口 来实现重新打开文档、提取图片、保存、书签等操作

      setTimeout( ()=>{
        isConnService = false
        openWebsocket(res.data.port,0)
      }, 200);
      if(res.aid > 0)
      {
        curID = res.aid
        //result.push("当前操作网页组件实例 " + res.aid)
      }
    }
    // 设置文件保存位置结果通知
    if (res.event == 'Wrl_SelectFile') {
      if(0 == SelectDlgType)
      {
        if(res.data.length)
        {
          BeginSaveAsFile(res.data[0].File) /// 使用返回的文件位置
        }
        else
          BeginSaveAsFile(SaveAsDoc) // 取消指定，使用默认位置文件
      }
      else if(1 == SelectDlgType)
      {
        if(res.data.length)
        {
          BeginExpportFile(res.data[0].File) /// 使用返回的文件位置
        }
        else
          BeginExpportFile(ExportDoc) // 取消指定，使用默认位置文件
      }
      else if(2 == SelectDlgType)
      {
        if(res.data.length)
        {
          BeginInsertImg(res.data[0].File) /// 使用返回的文件位置
        }
        else
          BeginInsertImg(InsertImgFile) // 取消指定，使用默认位置文件
      }
      else if(3 == SelectDlgType)
      {
        if(res.data.length)
        {
          BeginGetFirstImg(res.data[0].File) /// 使用返回的文件位置
        }
        else
          BeginGetFirstImg('') // 取消指定，直接返回图片BASE64编码，这种情况返回JPG格式图片数据
      }
      else if(4 == SelectDlgType)
      {
        if(res.data.length)
        {
          BeginConvertFirstPage(res.data[0].File) /// 使用返回的文件位置
        }
        else
          BeginConvertFirstPage('') // 取消指定，直接返回图片BASE64编码，这种情况返回JPG格式图片数据
      }
    }
    if (res.err)
    {
      // 请求返回错误
      ElMessage(res.err)
    }
    if (res.req == 'Wrl_Version')
    {
      if (res.data.Update == 1) {
        /// 先强制隐藏办公网页组件，避免弹框显示不出来
        hideApp(32);
        ElMessageBox.confirm('有新版本发布, 是否马上升级?', '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          /// 先提前关闭网页组件
          CloseAllApplet()
          SendUpdateJson()
        }).catch(() => {
          showApp()
        })
      } else {
        ElMessage('已经是最新版本！')
        /// 说明：即使版本号是最新的，也是可以发起升级请求SendUpdateJson()的，相当于覆盖安装，用于更新比如中间件或网页组件的配置文件等
      }
    }
    //记录日志
    result.push(msg.data)
  }
  socketClient.onClose = (msg) => {
    console.log(msg)
  }
  socketClient.onError = (msg) => {
    // 避免IE中点击重复播放及firefox断开连接提示等问题
    if(!ReStartLoad && isConnService && !isDisConnect)
    {
      // 连接不上，认为还没有安装PageHiOffice—文档在线编辑 没有安装时提示安装
      ElMessageBox.confirm('PageHiOffice—文档在线编辑组件 服务端口连接失败，可能是尚未安装，是否马上下载安装？', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        window.open('http://local.zorrosoft.com/Files/PageHiOfficeIns.exe') // 建议打包为zip等格式下载，解压后安装，EXE文件下载浏览器会提示是否保留
      }).catch(() => {
      })
    }
  }
}

function resize(position) {
  //请求改变网页组件实例显示位置或大小，如不需要改变显示位置，不传X和Y
  if(aid2 > 0 && aid2 == curID){
    rid++ // 增加请求序号
    if(position){
      let msg = {
        "req": "Wrl_AppletResize",
        "rid": rid,
        "para": {
          "ID": aid2,
          "X": left.value,
          "Y": top.value,
          "Width": width.value,
          "Height": height.value
        }
      }
      socket[2].sendObj(msg)
    }
    else{
      let msg = {
        "req": "Wrl_AppletResize",
        "rid": rid,
        "para": {
          "ID": aid2,
          "Width": width.value,
          "Height": height.value
        }
      }
      socket[2].sendObj(msg)
    }
  }
  else
  {
    if(aid > 0){
      rid++ // 增加请求序号
      if(position){
        let msg = {
          "req": "Wrl_AppletResize",
          "rid": rid,
          "para": {
            "ID": aid,
            "X": left.value,
            "Y": top.value,
            "Width": width.value,
            "Height": height.value
          }
        }
        socket[0].sendObj(msg)
      }
      else{
        let msg = {
          "req": "Wrl_AppletResize",
          "rid": rid,
          "para": {
            "ID": aid,
            "Width": width.value,
            "Height": height.value
          }
        }
        socket[0].sendObj(msg)
      }
    }
  }
}

function InsertContent()
{
  // 请求插入指定的文字或链接
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_Insert",
    "rid": rid,
    "para": [{"Paragraph":1},
      {"FontSize":"16","FontName":"宋体","Color":"0","Bold":"8","Text":"详情请访问成都佐罗软件网站："},
      {"Paragraph":1},
      {"Address":"http://www.zorrosoft.com","Display":"佐罗软件官方网站"}]
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function AddMark()
{
  // 请求插入书签
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_InsertMark",
    "rid": rid,
    "para": {"Name":"TestMark","Content":"[InsertMark]","Hidden":0,"Sort":0}
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function MarkRePlace()
{
  // 请求替换书签内容
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_PutMarkText",
    "rid": rid,
    "para": {"Marks":[{"Name":"Caption","Text":"公文标题"},{"Name":"TestMark","Text":"TestMarkContent"}]}
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function InsertDJSign()
{
  // 请求电子签章 Type默认0 支持北京点聚签章系统
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_SealInsert",
    "rid": rid,
    "para": {"Type":0}
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function SaveFile()
{
  // 请求保存文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_Save",
    "rid": rid,
    // 如果需要上传服务器保存，这里需要设置上传服务器Url地址及Auth权限等参数，Para中设置后台需要的自定义参数
    "para": {"Auth": "","Cookie": "","Para": {"DocID": 1},"Url": "http://www.yuanofficer.com/upload.php"}
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function SaveAsFile()
{
  rid++ // 增加请求序号
  SelectDlgType = 0
  /// 先设置保存位置再另存
  let msg = {
    "req": "Wrl_SelectFile",
    "rid": rid,
    "para": {
      "Type": 1,
      "Title" : "请设置另存文件位置",
      "Ext" : "另存文件(*.doc;*.docx;*.pdf;*.html)\r*.doc;*.docx;*.pdf;*.html"
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[2].sendObj(msg)
  else
    socket[0].sendObj(msg)
}

function BeginSaveAsFile(LocalFilePath)
{
  // 请求开始另存文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_SaveAs",
    "rid": rid,
    "para": {
      "NewFile": encodeURIComponent(LocalFilePath)
    }
  }
  if(aid2 > 0 && aid2 == curID)
    sockest[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function ExpportFile()
{
  // 请求导出文档
  rid++ // 增加请求序号
  SelectDlgType = 1
  /// 先设置导出位置
  let msg = {
    "req": "Wrl_SelectFile",
    "rid": rid,
    "para": {
      "Type": 1,
      "Title" : "请设置导出文件位置",
      "Ext" : "导出文件(*.pdf)\r*.pdf"
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[2].sendObj(msg)
  else
    socket[0].sendObj(msg)
}

function BeginExpportFile(LocalFilePath)
{
  // 请求开始另存文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_Export",
    "rid": rid,
    "para": {
      "NewFile": encodeURIComponent(LocalFilePath)
    }
  }
  if(aid2 > 0 && aid2 == curID)
    sockest[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function InsertImg()
{
  // 请求当前光标位置插入图片 先让用户选择图片文件
  rid++ // 增加请求序号
  SelectDlgType = 2
  /// 先选择本地图片文件
  let msg = {
    "req": "Wrl_SelectFile",
    "rid": rid,
    "para": {
      "Type": 0,
      "Title" : "请选择需要插入的图片",
      "Ext" : "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[2].sendObj(msg)
  else
    socket[0].sendObj(msg)
}

function BeginInsertImg(LocalFilePath)
{
  // Save为1时自动保存文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_InsertImg",
    "rid": rid,
    "para": {
      "ImgFile": encodeURIComponent(LocalFilePath),
      "Order": 5,	// 插入后图片在文字之上
      "Save": 1	// 插入后自动保存文档
    }
  }
  if(aid2 > 0 && aid2 == curID)
    sockest[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function GetFirstImg()
{
  // 提取文档中页码序号Index的图片，先设置图片保存位置
  rid++ // 增加请求序号
  SelectDlgType = 3
  let msg = {
    "req": "Wrl_SelectFile",
    "rid": rid,
    "para": {
      "Type": 1,
      "Title" : "请设置图片保存位置",
      "Ext" : "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[2].sendObj(msg)
  else
    socket[0].sendObj(msg)
}

function BeginGetFirstImg(LocalFilePath)
{
  // 提取文档中序号Index的Base64编码数据，如指定本地保存文件名File，则保存到本地文件中
  rid++ // 增加请求序号
  let msg = {}
  if(LocalFilePath.length)
  {
    msg = {
      "req": "Office_GetImage",
      "rid": rid,
      "para": {
        "File": encodeURIComponent(LocalFilePath),
        "Index":1
      }
    }
  }
  else
  {
    msg = {
      "req": "Office_GetImage",
      "rid": rid,
      "para": {
        "Index":1
      }
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function ConvertFirstPage()
{
  // 转换文档中序号为Index页码内容成图片，先设置转换图片存放位置
  rid++ // 增加请求序号
  SelectDlgType = 4
  let msg = {
    "req": "Wrl_SelectFile",
    "rid": rid,
    "para": {
      "Type": 1,
      "Title" : "请设置图片保存位置",
      "Ext" : "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[2].sendObj(msg)
  else
    socket[0].sendObj(msg)
}

function BeginConvertFirstPage(LocalFilePath)
{
  // 转换文档中页码序号Index的Base64编码数据，如指定本地保存文件名File，则保存到本地文件中
  rid++ // 增加请求序号
  let msg = {}
  if(LocalFilePath.length)
  {
    msg = {
      "req": "Office_ConvertImage",
      "rid": rid,
      "para": {
        "File": encodeURIComponent(LocalFilePath),
        "Index":1
      }
    }
  }
  else
  {
    msg = {
      "req": "Office_ConvertImage",
      "rid": rid,
      "para": {
        "Index":1
      }
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function DocRed()
{
  // 请求对当前文档进行套红操作
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_RedTemplate",
    "rid": rid,
    "para": {
      "Position": "Content", // 指定的模版文件需要有书签Content，用于定位公文正文的开始位置，也可以用模版文件指定行数，比如"Position": "10" 代表模版文件的第10行开始是正文
			"File": "http://local.zorrosoft.com/Files/template.doc" // 模版文件，也可以是本地路径
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function PrintFile()
{
  // 请求打印当前文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_PrintOut",
    "rid": rid,
    "para": {
      "ImgFile": PrintPathFile,
      "Copies": 1
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function EnableRevision()
{
  // 请求留痕，就是修订模式
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_PutTrackRevisions",
    "rid": rid,
    "para": {
      "TrackRevisions": 1
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
  IsRevision.value = true
}

function DisableRevision()
{
  // 关闭留痕，就是关闭修订模式
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_PutTrackRevisions",
    "rid": rid,
    "para": {
      "TrackRevisions": 0
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
  IsRevision.value = false
}

function ShowRevision()
{
  // 显示留痕信息，就是显示修订内容
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_PutShowRevisions",
    "rid": rid,
    "para": {
      "ShowRevisions": 1
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function AcceptRevision()
{
  // 接受留痕，就是接受修订内容
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_TrackRevisions",
    "rid": rid,
    "para": {
      "Type": 1
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function changeOpen() {
  //重新打开文档
  rid++ // 增加请求序号
  let msg = {
    "req": "Office_Open",
    "rid": rid,
    "para": {
      "Open":encodeURIComponent(MyOpenDoc.value)
    }
  }
  if(aid2 > 0 && aid2 == curID)
    socket[3].sendObj(msg)
  else
    socket[1].sendObj(msg)
}

function showApp() {
  //显示办公网页组件
  if (aid > 0) {
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletControl",
      "rid": rid,
      "para": {
        "ID": aid,
        "Code": 8
      }
    }
    socket[0].sendObj(msg)
  }
  //如果启动了第二个网页组件 也显示
  if (aid2 > 0) {
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletControl",
      "rid": rid,
      "para": {
        "ID": aid2,
        "Code": 8
      }
    }
    socket[2].sendObj(msg)
  }
}

function hideApp(code) {
  //隐藏办公网页组件 Code设置4是自动隐藏，如需强制隐藏，设置为32
  if (aid > 0)
  {
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletControl",
      "rid": rid,
      "para": {
        "ID": aid,
        "Code": code
      }
    }
    socket[0].sendObj(msg)
  }
  //如果启动了第二个网页组件 也同时隐藏
  if (aid2 > 0)
  {
    rid++ // 增加请求序号
    let msg = {
      "req": "Wrl_AppletControl",
      "rid": rid,
      "para": {
        "ID": aid2,
        "Code": code
      }
    }
    socket[2].sendObj(msg)
  }
}

function CheckUpdate() {
  //校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的网页组件，Wrl_Version功能多
  rid++ // 增加请求序号
  let msg = {
    "req": "Wrl_Version",
    "rid":rid,
    "para": {
      "Version": version.value
    }
  }
  socket[0].sendObj(msg)
}

function SendUpdateJson() {
  // 发送中间件的升级命令，实现自动升级，同时升级微软及金山办公等网页组件
  // 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成，此处举例的升级包是在线公测版的，正式版需要自己制作
  rid++ // 增加请求序号
  let msg = {
    "req":"Wrl_Update",
    "rid":rid,
    "para":{
      "Name": "PageHiOffice—文档在线编辑组件升级包",
			"Date": "2025-05-31",
			"Desc": "1、解决高级版内嵌网页小程序个别情况下启动可能被误判为启动超时导致启动失败问题；2、优化抓图工具截图过程体验及效果；3、PageHiOffice网页组件解决个别情况下禁止内容复制无效问题，解决微软Word的OLE嵌入后菜单栏和工具栏可能无法点击问题，解决微软Excel完整嵌入后输入焦点问题，优化OLE嵌入模式代理进程处理逻辑，解决完整嵌入时可能出现“服务器正在运行中”的弹窗...",
			"DownAddr": "http://local.zorrosoft.com/Files/Update/Office_Update.pid",
			"Open": "http://local.zorrosoft.com/office3",
			"MD5": "DDBD5CA8A1A432D4DD253F2A5016D2C9",
			"Version": "2.2.17.6",
			"Size": 36634624,
			"HideIns": 0,
			"Cookie": "",
			"Auth": "",
			"TK": "466E45D4E7272B9297F6EBFCEFC13BA1E5EC412C846EF0B2125A2BD6FA2B7C4C4B3C79A7E1F9A10F5BE21CBC139FC1590CCD5B864BBF3DA87E1D396043870BA9242542BCE0B0CE099452CE0970477B5E9AFF10C7DFA3D9D722A6737914F5A65DC81654CCB6C11381099A31C0C9B15384C01839450F8A0F87864004A791E9D15EE26A5C29D56661BBDE76C9A4EF7CA72D71345EFC26C40FB4DDCD689E1A1584B352F9534A22E7AA70FA819F2E9E3F580248C8F498B74B7893624AB21D8FC43B3CA23CCA6364C1527CF2E0C6CA81CBEBE2689C189324BFE9C2BE0DBD3BB2FA4B25C28B0552AD859D202BE15DF2227C8C2C03BB390CA86D1608CE23B6D09A62F005"
    }
  }
  socket[0].sendObj(msg)
}

</script>
<style scoped>
.mainContainer {
  display: block;
  width: 960px;
  margin-left: 20px;
  margin-right: auto;
  box-sizing: border-box;
}

.urlbox {
  display: flex;
  display: -ms-flexbox;
  align-items: center;
  -ms-flex-align: center;
  justify-content: space-between;
  margin: 15px 0;
}

.urlbox .input {
  flex: 1;
  margin: 0 15px;
}

.tool {
  display: flex;
  display: -ms-flexbox;
  align-items: center;
  -ms-flex-align: center;
  margin: 15px 0;
}

.tool .item {
  margin-right: 15px;
  font-size: 12px;
}

.tool .item:last-child {
  margin-right: 0;
}

.tool .input {
  width: 60px;
}

.video-container {
  position: relative;
  margin-top: 8px;
  height: 480px;
  border: #ddd 1px dashed;
  display: flex;
  display: -ms-flexbox;
  align-items: center;
  -ms-flex-align: center;
  justify-content: center;
  font-size: 24px;
  color: #ddd;
}
</style>