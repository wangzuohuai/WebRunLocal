<template>
	<div class="mainContainer">
		<div class="tool">
			<div class="item">窗口宽度</div>
			<div class="item">
				<el-input v-model="width" size="small" class="input" @blur="resize(0)"></el-input>
			</div>
			<div class="item">窗口高度</div>
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
			<div class="item">分屏样式</div>
			<div class="item">
				<el-input-number v-model="ShowType" :step="1" size="small" @change="setType()" :min="1">
				</el-input-number>
			</div>
			<el-button size="small" v-if="StartSecond" @click="CloseSecondPlayer">
				关闭多进程播放器
			</el-button>
			<el-button size="small" @click="openSecondPlayer" v-else>
				启动多进程播放器
			</el-button>
		</div>
			<div class="tool">
				<div class="item">子窗口</div>
				<div class="item">
					<el-input-number v-model="win" :step="1" size="small" @change="SetSelWnd()" :min="1"></el-input-number>
				</div>
				<div class="item">
					<el-button size="small" @click="getCapture">抓图</el-button>
				</div>
				<el-button size="small" @click="watermask">设置水印</el-button>
				<el-button size="small" @click="RecordToFile">
					<span v-if="isRecordFile">停止录像</span>
					<span v-else>本地录像</span>
				</el-button>
				<div class="item" style="margin-left: 10px;">
					<el-button size="small" @click="NextFrame()" type="primary">下一帧</el-button>
				</div>
				<div class="item">
					<el-button size="small" @click="PlayPause()" type="primary">暂停播放</el-button>
				</div>
				<div class="item">
					<el-button size="small" @click="PlayContinue()" type="primary">继续播放</el-button>
				</div>
				<div class="item">
					<el-button size="small" @click="PlaySpeed()" type="primary">加速播放</el-button>
				</div>
				<div class="item">
					<el-button size="small" @click="PlayMute()" type="primary">播放静音</el-button>
				</div>
				<div class="item">
					<el-button size="small" @click="PlayFullScreen()" type="primary">全屏播放</el-button>
				</div>

			</div>
			<div class="video-container" ref="player">
			播放器区域
			</div>
			<div class="urlbox">
				<div class="item">视频地址</div>
				<div class="item input">
					<el-input v-model="newrtsp" placeholder="这里演示动态切换播放源" size="small"></el-input>
				</div>
				<div class="item">旋转</div>
				<div class="item">
					<el-select size="small" v-model="Transform" placeholder="请选择" style="width: 100PX;">
						<el-option label="none" value="none">
						</el-option>
						<el-option label="90" value="90">
						</el-option>
						<el-option label="180" value="180">
						</el-option>
						<el-option label="270" value="270">
						</el-option>
						<el-option label="hflip" value="hflip">
						</el-option>
						<el-option label="vflip" value="vflip">
						</el-option>
						<el-option label="transpose" value="transpose">
						</el-option>
						<el-option label="antitranspose" value="antitranspose">
						</el-option>
					</el-select>
				</div>
				<div class="item" style="margin-left: 10px;">
					<el-button type="primary" @click="changUrl()" size="small">切换源播放</el-button>
				</div>
		</div>
		<div class="urlbox">
			<div class="item">叠加透明网页地址</div>
				<div class="item input">
					<el-input v-model="newfloatweb" placeholder="这里演示叠加网页内容显示" size="small"></el-input>
				</div>
				<div class="item" style="margin-left: 10px;">
					<el-button type="primary" @click="FloatWebInfo()" size="small">叠加显示</el-button>
				</div>
			</div>
			<div class="tool">
				<div class="item">字幕</div>
				<div class="item" style="flex: 1;">
					<el-input size="small" placeholder="请输入内容" v-model="danmu.text"></el-input>
				</div>
				<div class="item">
					<el-select size="small" v-model="danmu.position" placeholder="请选择" style="width: 100PX;">
						<el-option label="顶部" value="TOP">
						</el-option>
						<el-option label="底部" value="BOTTOM">
						</el-option>
					</el-select>
				</div>
				<div class="item">位置（x）</div>
				<div class="item">
					<el-input v-model="danmu.x" size="small" class="input"></el-input>
				</div>
				<div class="item">位置（y）</div>
				<div class="item">
					<el-input v-model="danmu.y" size="small" class="input"></el-input>
				</div>
				<div class="item">透明度</div>
				<div class="item">
					<el-input v-model="danmu.opacity" size="small" class="input"></el-input>
				</div>
				<div class="item">大小</div>
				<div class="item">
					<el-input v-model="danmu.size" size="small" class="input"></el-input>
				</div>
				<div class="item">
					<el-button size="small" @click="setText()" type="primary">设置</el-button>
				</div>
			</div>
		<div class="tool">
			<div class="item">多源播放JSON</div>
			<el-input v-model="WebCfg2" size="small"></el-input>
			<div class="item">
				<el-button size="small" @click="RePlayFirst()" type="primary">重新启动播放</el-button>
			</div>
		</div>
		<div class="tool">
			<div class="item" style="margin-left: 10px;">
				<el-button type="primary" @click="StopAllPlay()" size="small">停止所有播放</el-button>
			</div>
			<div class="item" style="margin-left: 10px;">
				<el-button type="primary" @click="CloseAllPlayer()" size="small">关闭所有播放器</el-button>
			</div>
			<div class="item">最新版本号</div>
			<div class="item">
				<el-input v-model="version" size="small"></el-input>
			</div>
			<el-button size="small" @click="CheckUpdate" type="primary">校验升级</el-button>
		</div>
		<el-input type="textarea" :rows="5" placeholder="调试日志" v-model="DebugLog" class="DebugLog">
		</el-input>
	</div>
</template>

<script setup>
//集成开发说明：所有交互都是通过JS建立websocket连接后，发送或接收JSON包进行，都是异步请求，发送和接收包都携带rid用来标识请求返回包对应是哪个请求的结果
//所有发送的msg中用到的长整形rid 可以自己指定 不同的发送请求定义唯一的rid 这样可以在回调用轻松判断
//针对一些特殊请求，可以根据自己的实际情况把rid固定，如启动网页播放器时指定固定rid获得启动网页播放器的实例ID
//首先建立第一个websocket连接到中间件服务端口启动RTSP网页播放器 然后获得启动RTSP网页播放器的侦听端口建立第二个websocket连接 主要用来进行控制播放、抓图、录像等操作
//释放网页播放器，直接关闭建立的第一个websocket连接即可
import GetDefaultConn from "./common/base.js"
//加载websoket类
import websocket from './common/websocket'
import {ref,reactive, computed,onMounted,onUnmounted} from 'vue'
import { ElMessage,ElMessageBox } from 'element-plus'

const player = ref()
let version = ref('2.2.15.2') //中间件版本信息
let aid = 0		              // 第一个播放器实例ID
let aid2 = 0		          // 第二个播放器实例ID
let curID = 0 		          // 当前操作播放器实例ID
let rid = 10 		          // 其它请求起始序号
let runInfo = 1    	          // 获取本机VLC安装信息 一般只需要在启动时获取一次
let RunFirst = 2  	          // 播放器启动实例1序号
let RunSecond = 3   	      // 播放器启动实例2序号
let win = 1 	              // 播放实例中当前操作的分屏窗口序号ID
let newrtsp     = 'http://devimages.apple.com.edgekey.net/streaming/examples/bipbop_4x3/gear2/prog_index.m3u8'
let newfloatweb = ref('https://output.jsbin.com/dopavun')
let WebCfg      = ref('[{"ID":2,"Uri":"https://vjs.zencdn.net/v/oceans.mp4"},{"ID":1,"Uri":"https://media.w3.org/2010/05/sintel/trailer.mp4","Option":":rtsp-tcp"},{"ID":4,"Uri":"http://www.zorrosoft.com/Files/PluginOKBrowserApplet.mp4","Option":":file-caching=300"},{"ID":3,"Uri":"http://www.zorrosoft.com/Files/h265.mkv","Option":":network-caching=500"}]')
let WebCfg2     = ref('[{"ID":1,"Uri":"https://vjs.zencdn.net/v/oceans.mp4"},{"ID":2,"Uri":"https://media.w3.org/2010/05/sintel/trailer.mp4","Option":":rtsp-tcp"},{"ID":3,"Uri":"http://www.zorrosoft.com/Files/PluginOKBrowserApplet.mp4","Option":":file-caching=300"},{"ID":4,"Uri":"rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp","Option":":network-caching=500"}]')
let RecordFilePath = ref('C:/Zorro/test.mp4')
let Transform = 'none'
let recordid = 0 			   //录像时返回的录像进程PID
let isRecordFile = false       //记录录像的状态
let danmu = {
					//需要发送的字幕配置信息
					text: '您好呀，O(∩_∩)O哈哈~',
					position: 'TOP',
					color: "#ff0000",
					opacity: 128,
					size: 50,
					x: 0,
					y: 0
				}
let StartSecond = ref(0)       // 是否启动了第二个网页播放器实例
let ReStartLoad = ref(0)       // 第一个网页播放器实例是否重新打开状态
let width = ref(960) 		   // 网页播放器窗口显示宽度
let height = ref(480)	       // 网页播放器窗口显示高度
let left = ref(0)		       // 网页播放器窗口以网页左上角为基点显示的X位置
let top = ref(0)		       // 网页播放器窗口以网页左上角为基点显示的Y位置
let IframeX = -10   	       // 网页播放器窗口模拟iFrame用的X坐标
let IframeY = 0  		       // 网页播放器窗口模拟iFrame用的Y坐标
let ShowType = 1 	           //分屏样式
let isConnService = false      //是否连接的中间件服务
let isDisConnect = false       // 是否处于断开连接过程
let socket = [] 	           //websocket对象数组，采用数组需要注意维护索引号，可以改为根据连接的唯一SID参数存到对应的集合，需要用到连接时根据SID提取，这样可支持启动更多实例
let result = reactive([])  	   //日志结果数组

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
  //获取网页播放器位置节点信息
  GetAppletPosition()
  //先获取本机VLC软件安装信息
  GetPlayerInfo()
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
    // 默认纵向滚动网页播放器实例，如需要横向滚动，Code设置为1，修改Left的值
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
  //获取网页播放器位置节点信息
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
    //切离该页面时执行 当前页自己实现标签切换或需要在RTSP网页播放器程序窗口区域临时显示内容时，设置32强制隐藏
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
  //关闭网页播放器实例
  CloseAllPlayer()
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

function GetPlayerInfo()
{
  isConnService = true
  let Protocol = location.protocol;
  if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
  {
    // HTTPS网站，连接WSS侦听端口
    openWebsocket(453,0)
    //openWebsocket(453,1) 支持安全校验机制
  }
  else
  {
    openWebsocket(83,0)
    //openWebsocket(83,1) 支持安全校验机制
  }
  let msg = {
    // 这里如果没有用到LibVLC引擎播放，也可以调用指令 Wrl_GetVideoCard 获取本机显卡的信息，返回包括是否有独立显卡，显卡名称及型号等，用于多路播放时决策可以播多少路
    "req": "Wrl_PlayerInfo",
    "rid": runInfo,
    "para": {}
  }
  socket[0].sendObj(msg)

}

function StartPlayerApplet() {
  //启动第一个VLC网页播放器 Open为播放源，播放源也可以放到Web节点中，参考RePlayFirst中实现，差异在于，Open中指定对所有分屏有效，而在Web节点中可指定更多播放参数
  let msg = {
    "req": "Wrl_VLCWebPlayer",
    "rid": RunFirst,
    "para": {
      "Type": "0",
      "Title": "VLC网页播放器",
      "Version": 0,// 设置1时代表启用独立进程来播放，独立进程播放好处是播放时如果出现进程崩溃，不会导致播放组件进程退出
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
      "Web": [{"ID":1,"Uri":"http%3A%2F%2Fwww.zorrosoft.com%2FFiles%2FPluginOKBrowserApplet.mp4","Option":":rtsp-tcp :network-caching=500"}],
	  "ShowType": ShowType
    }
  }
  console.log(msg)
  socket[0].sendObj(msg)
}

function openSecondPlayer() {
  if(aid)
  {
    //演示加载第二个RTSP网页播放器实例打开表格程序 先改变第一个网页播放器位置 Flag值需要在原来基础上+512以支持多实例加载
    width.value = 475
    resize(0)
    isConnService = true
    let Protocol = location.protocol;
    if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
    {
      // HTTPS网站，连接WSS侦听端口
      openWebsocket(453,0)
      //openWebsocket(453,1) 支持安全校验机制
    }
    else
    {
      openWebsocket(83,0)
      //openWebsocket(83,1) 支持安全校验机制
    }

    let msg = {
      "req": "Wrl_VLCWebPlayer",
      "rid": RunSecond,
      "para": {
        "Type": "0",
        "Title": "VLC网页播放器2",
        "Version": 1,
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
        "Web": JSON.parse(WebCfg.value),
		"ShowType": 4
      }
    }
    socket[2].sendObj(msg)
    StartSecond.value = 1
  }
  else{
    ElMessage('请先启动第一个网页播放器')
  }
}

function CloseSecondPlayer() {
  if (StartSecond.value) {
    isDisConnect = true
    /// 每个网页播放器实例占用2个连接，一个到中间件、一个到网页播放器，分别断开连接并释放
    socket[3].disconnect()
    socket[2].disconnect()
    socket.pop()
    socket.pop()
    isDisConnect = false
    StartSecond.value = 0
    aid2 = 0
    //还原第一个网页播放器位置
    width.value = 960
    height.value = 480
    //获取网页播放器位置节点信息
    let react = player.value.getBoundingClientRect()
    left.value = react.left
    top.value = react.top
    resize(0)
  }
}

function CloseFirstPlayer()
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
    /// 释放网页播放器的连接
    socket[1].disconnect()
    socket[0].sendObj(msg)
    aid = 0
    ReStartLoad.value = 0
    if(StartSecond.value)
      socket.splice(1,1)
    else
      socket.pop()
    isDisConnect = false
  }
}

function RePlayFirst()
{
  	CloseFirstPlayer()
	ReStartLoad.value = 1
	//重新启动播，Web节点配置播放源，设置分屏风格4，播放引擎从VLC改为FFPlayer
    ShowType = 4
	let Msg = {
		"req": "Wrl_VLCWebPlayer",
		"rid": RunFirst,
		"para": {
		"Type": "0",
		"Title": "VLC网页播放器",
		"Version": 0,
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
		"Web": JSON.parse(WebCfg2.value),
		"ShowType": ShowType
		}
	}
	socket[0].sendObj(Msg)
}

function CloseAllPlayer()
{
  /// 先关闭第二个实例，否则socket中保存的连接序号会不正常
  CloseSecondPlayer()
  CloseFirstPlayer()
}

function StopSecondPlayer()
{
	if (StartSecond.value)
	{
		rid++ // 增加序号
		let msg = {
			"req": "VLC_Control",
			"rid": rid,
			"para": [{
				"ID": 1,
				"Type": "Stop"
			},{
				"ID": 2,
				"Type": "Stop"
			},{
				"ID": 3,
				"Type": "Stop"
			},{
				"ID": 4,
				"Type": "Stop"
			}]
		}
		console.log(msg)
		socket[3].sendObj(msg)
	}
}

function StopFirstPlayer()
{
	if (aid > 0)
	{
		rid++ // 增加序号
		/// 支持批量停止播放 暂停播放等操作 para中具体传多少个分屏窗口数据自己定，分屏窗口数量由ShowType决定
		/// ID是分屏窗口序号，从1开始 序号原则是从左向右开始编号，然后从上到下开始顺序编号，如遇右侧多层排列窗口时，直到右侧窗口序号排序完成
		/// Type可以是这些值：Pause,Play,Stop,StopAsync,Next,Prev,Clear,TogglePause 分别对应暂停播放 播放 停止播放 异步停止播放 下一个播放源 上一个播放源 清理播放列表 切换暂停状态
		let msg = {}
		if(ReStartLoad.value)
		{
			msg = {
				"req": "VLC_Control",
				"rid": rid,
				"para": [{
				"ID": 1,
				"Type": "Stop"
				},{
					"ID": 2,
					"Type": "Stop"
				},{
					"ID": 3,
					"Type": "Stop"
				},{
					"ID": 4,
					"Type": "Stop"
				}]
			}
		}
		else{
			/// 不指定分屏信息时，停止所有播放
			msg = {
				"req": "VLC_Control",
				"rid": rid,
				"para": []
			}
		}
		console.log(msg)
		socket[1].sendObj(msg)
	}
}

function StopAllPlay()
{
	StopSecondPlayer()
	StopFirstPlayer()
}

function openWebsocket(port,type) {
  //打开websocket服务
  let ws = GetDefaultConn(port,type)
  const socketClient = new websocket(ws, {
    reconnectEnabled: false
  })
  socketClient.connect()

  if(1 == ReStartLoad.value && StartSecond.value)
  {
    ReStartLoad.value = 2
    socket.splice(1,0,socketClient) // 重新加载时已经启动第二个实例，采用插入方式，避免WS下标错误，确保第一个实例的WS连接下标是0和1，第二个实例是3和4
  }
  else
  {
    if(1 == ReStartLoad.value)
		ReStartLoad.value = 2
    socket.push(socketClient)
  }
  socketClient.onMessage = (msg) =>
  {

    let res = JSON.parse(msg.data)
    console.log(res)
    if(res.rid == runInfo)
    {
      // 解析当前VLC桌面软件安装情况
      if(res.data.Info.size)
      {
        result.push("VLC安装路径:" + res.data.Info[0].Path)
      }
      StartPlayerApplet()
    }
    if (res.event == 'Wrl_AppletOK') {
      //网页播放器创建成功后根据事件名称获取当前网页播放器id
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
      //网页播放器建立侦听成功 这里得到当前网页播放器返回的端口
      //这里创建另外一个websocket到RTSP网页播放器端口 来实现重新打开文档、提取图片、保存、书签等操作

      setTimeout( ()=>{
        isConnService = false
        openWebsocket(res.data.port,0)
      }, 200);
      if(res.aid > 0)
      {
        curID = res.aid
        //result.push("当前操作网页播放器实例 " + res.aid)
      }
    }
    //切换窗口的时候 设定当前窗口为此窗口
    if (res.event == 'VLC_Selected') {
		win = res.ID // 选中的分屏窗口序号
		if(res.aid > 0)
		{
			curID = res.aid
			//result.push("当前操作播放器实例 " + res.aid)
		}
	}
	// 播放窗口收到鼠标按下通知
	if (res.event == 'VLC_MouseDown') {
		if(res.aid > 0)
		{
			curID = res.aid
			//result.push("当前操作播放器实例 " + res.aid)
		}
	}
	// 定时录像结束通知
	if (res.event == 'VLC_StopRecord') {
		isRecordFile = false;
		recordid = 0;
		$message.success("定时录像成功\n" + res.data.File);
	}
	// 设置文件保存位置结果通知
	if (res.event == 'Wrl_SelectFile') {
		if(res.data.length)
		{
			BeginRecordFile(res.data[0].File) /// 使用返回的文件位置
		}
		else
			BeginRecordFile(RecordFilePath.value) // 取消指定，使用默认位置
	}
	//请求录像返回
	if (res.rid == 90000) {
		//记录录像的pid
		recordid = res.data.PID
	}
	//请求停止录像返回
	if (res.rid == 90001) {
		recordid = 0;
		$message.success("录像成功\n" + res.data.File)
		//console.log(res.data.File)
	}
	//请求截图返回
	if (res.rid == 90002) {
		$message.success("截图成功\n" + res.data.Img[0].File)
		//console.log(res.data.Img[0].File)
	}
	if (res.err)
	{
		// 请求返回错误
		$message.success(res.err)
	}
    if (res.req == 'Wrl_Version')
    {
      if (res.data.Update == 1) {
        /// 先强制隐藏网页播放器，避免弹框显示不出来
        hideApp(32);
        ElMessageBox.confirm('有新版本发布, 是否马上升级?', '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          /// 先提前关闭网页播放器
          CloseAllPlayer()
          SendUpdateJson()
        }).catch(() => {
          showApp()
        })
      } else {
        ElMessage('已经是最新版本！')
        /// 说明：即使版本号是最新的，也是可以发起升级请求SendUpdateJson()的，相当于覆盖安装，用于更新比如中间件或网页播放器的配置文件等
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
    if(!ReStartLoad.value && isConnService && !isDisConnect)
    {
      //连接不上，认为还没有安装 PageHiPlayer—VLC低延迟网页播放器 没有安装时提示安装
      ElMessageBox.confirm('PageHiPlayer—VLC低延迟网页播放器 服务端口连接失败，可能是尚未安装，是否马上下载安装？', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        window.open('http://local.zorrosoft.com/Files/RtspWebPlayerIns.exe') // 建议打包为zip等格式下载，解压后安装，EXE文件下载浏览器会提示是否保留
      }).catch(() => {
      })
    }
  }
}

function resize(position) {
  //请求改变网页播放器实例显示位置或大小，如不需要改变显示位置，不传X和Y
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

function getCapture() 
{
	//请求截图，固定请求ID处理，也可以是建立一个map记录每个rid对应的含义，请求返回里再处理
	let msg = {
		"req": "VLC_VideoSnapshot",
		"rid": 90002,
		"para": [{
			"ID": win,
			"Type": 4,
			"Count": 1,
			"Base64":0,
			"Delay": 1000,
			"Interval": 200,
			"PathType": 1
		}]
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function setText() 
{
	//发送字幕到指定的窗口ID
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_MarqueePut",
		"rid": rid,
		"para": [{
			"ID": win,
			"Text": danmu.text,
			"Position": danmu.position,
			"Timeout": 0,
			"Color": danmu.color,
			"Opacity": danmu.opacity,
			"Refresh": 1,
			"Size": danmu.size,
			"X": danmu.x,
			"Y": danmu.y
			}]
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function setType()
{
	//动态改变屏幕数量
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_ChangePlay",
		"rid": rid,
		"para": {
			"ShowType": ShowType
		}
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function NextFrame()
{
	//请求显示下一帧 请求后播放进入暂停状态，可继续请求下一帧，或请求播放恢复正常播放状态
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_NextFrame",
		"rid": rid,
		"para": [{
			"ID":win,
			"Count": 1
		}]
	}

 if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function PlayPause()
{
	//请求播放进入暂停状态
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_Control",
		"rid": rid,
		"para": [{
			"ID":win,
			"Type": "Pause"
		}]
	}

	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function PlayContinue()
{
	//请求播放继续，或请求播放恢复正常播放状态
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_Control",
		"rid": rid,
		"para": [{
			"ID":win,
			"Type": "Play"
		}]
	}

	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function PlayMute()
{
	// 设置选中分屏窗口视频静音,1静音 0不静音
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_AudioPut",
		"rid": rid,
		"para": [{
			"ID":win,
			"Mute": 1
		}]
	}

	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function PlaySpeed()
{
	// 调整播放速度,仅限于文件或回放流，支持批量操作，当前演示针对选中分屏窗口设置2倍数播放
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_PutInputInfo",
		"rid": rid,
		"para": [{
			"ID":win,
			"Rate": 2
		}]
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function PlayFullScreen()
{
	// 设置选中分屏窗口全屏显示 全屏后按ESC 字幕F\、双击或点击工具栏全屏图标可退出全屏状态
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_VideoToggleFullscreen",
		"rid": rid,
		"para": [{
			"ID":win
		}]
	}

	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function SetSelWnd() {
	//设置选中分屏子窗口
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_SetSelect",
		"rid": rid,
		"para": {
			"ID": win
		}
	}

	if(aid2 > 0 && aid2 == curID)
	{
		result.push("执行播放器实例 " + curID)
		socket[3].sendObj(msg)
	}
	else
		socket[1].sendObj(msg)
}

function AppletSnap()
{
	// 整个播放窗口抓图，包含所有分屏子窗口，发送到中间件侦听端口执行
	rid++ // 增加请求序号
	let msg = {
		"req": "Wrl_AppletSnap",
		"rid": rid,
		"para": {
			"ID": curID,
			"File": ".jpg",
			"Base64": 1
		}
	}

	if(aid2 > 0 && aid2 == curID)
		socket[2].sendObj(msg)
	else
		socket[0].sendObj(msg)
}

function FullApplet()
{
	// 整个播放窗口全屏，包含所有分屏子窗口，发送到中间件侦听端口执行
	rid++ // 增加请求序号
	let msg = {
		"req": "Wrl_AppletControl",
		"rid": rid,
		"para": {
			"ID": curID,
			"Code": 2
		}
	}

	if(aid2 > 0 && aid2 == curID)
		socket[2].sendObj(msg)
	else
		socket[0].sendObj(msg)
}

function RecordToFile()
{
	//对指定的窗口ID进行录像 固定请求ID处理，也可以是建立一个map记录每个rid对应的含义，请求返回里再处理
	if (isRecordFile) {
		let msg = {
			"req": "VLC_StopRecord",
			"rid": 90001,
			"para": {
				"ID": win
			}
		}
		isRecordFile = false
		if(aid2 > 0 && aid2 == curID)
			socket[3].sendObj(msg)
		else
			socket[1].sendObj(msg)
	} else {
		rid++ // 增加请求序号
		/// 先设置保存位置再录像
		let msg = {
			"req": "Wrl_SelectFile",
			"rid": rid,
			"para": {
				"Type": 1,
				"Title" : "请设置录像文件存放位置",
				"Ext" : "录像文件(*.mp4)\r*.mp4"
				}
		}
		if(aid2 > 0 && aid2 == curID)
			socket[2].sendObj(msg)
		else
			socket[0].sendObj(msg)
	}
}

function BeginRecordFile(LocalFilePath)
{
	let msg = {
		"req": "VLC_RecordFile",
			"rid": 90000,
			"para": {
				"ID": win,
				//不指定Url时取当前焦点分屏窗口源进行录像
				//"Url": encodeURIComponent(newrtsp),
				"File": encodeURIComponent(LocalFilePath),
				"Second": 30
			}
		}
		isRecordFile = true
		if(aid2 > 0 && aid2 == curID)
			socket[3].sendObj(msg)
		else
			socket[1].sendObj(msg)
}

function watermask() 
{
	//对指定的窗口ID发送水印，只支持VLC引擎播放，其它引擎播放，可采用叠加透明网页的方式，参考FloatWebInfo的实现
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_PutLogoShow",
		"rid": rid,
		"para": [{
			"ID": win,
			"File": "VLC.png",
			"Delay": 20,
			"Repeat": -1,
			"Opacity": 128,
			"X": 100,
			"Y": 100
		}]
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function changUrl() 
{
	//动态改变指定的窗口ID的播放源，VLC_ChangePlay本身支持同时改变多个分屏窗口播放源，Play支持传数组 ForceDestroy 指定是否先销毁原来播放引擎 默认不销毁
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_ChangePlay",
		"rid": rid,
		"para": {
			"ForceDestroy":0,
			"Play": [{
				"ID": win,
				"Uri": encodeURIComponent(newrtsp),
				"Name": "new rtsp",
				"Option": `:rtsp-tcp Transform=${Transform}`
			}]
		}
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function FloatWebInfo() 
{
	//对指定的窗口ID叠加网页内容显示
	rid++ // 增加请求序号
	let msg = {
		"req": "VLC_FloatWebInfo",
		"rid": rid,
		"para": [{
			"ID": win,
			"Url": encodeURIComponent(newfloatweb.value),
			"Rect":{"P":4,"W":300,"H":300}
		}]
	}
	if(aid2 > 0 && aid2 == curID)
		socket[3].sendObj(msg)
	else
		socket[1].sendObj(msg)
}

function showApp() {
  //显示RTSP网页播放器
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
  //如果启动了第二个网页播放器 也显示
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
  //隐藏网页播放器 Code设置4是自动隐藏，如需强制隐藏，设置为32
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
  //如果启动了第二个网页播放器 也同时隐藏
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
  //校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的网页播放器，Wrl_Version功能多
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
  // 发送中间件的升级命令，实现自动升级，同时升级网页播放器
  // 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成，此处举例的升级包是在线公测版的，正式版需要自己制作
  rid++ // 增加请求序号
  let msg = {
    "req":"Wrl_Update",
    "rid":rid,
    "para":{
    	"Name":"PageHiPlayer—VLC低延迟网页播放器升级包",
		"Date":"2024-05-27",
		"Desc":"1、中间件WebSocket侦听及连接服务正式支持传输二进制数据，优化服务进程状态检测机制，解决个别打开快速启动设置的电脑重新开机启动问题；2、优化PageHiPlayer异步打开播放的兼容支持，解决个别视频播放提示错误问题，解决切换播放清除播放地址时窗口有残留窗体问题；3、解决PageHiPlayer多引擎播放可能指定引擎无效及视频缓存设置无效问题，优化FFPlay播放效果，增加音频缓存设置解决声音卡顿问题...",
		"DownAddr":"http://local.zorrosoft.com/Files/Update/RTSP_Update.pid",
		"Open":"http://local.zorrosoft.com/VLC3",
		"MD5":"905A061E5FD5CCC82D3168208FE111CC",
		"Version":"2.2.15.2",
		"Size":65830912,
		"HideIns":0,
		"Cookie":"",
		"Auth":"",
		"TK":"60173AB44881D46E4437328368A24FF0D2F2EE4EE31EDA01771FCA5DFBE5078E856D543F776180CEC715CA73C33CED40043B3A00A0DA3372FFD4903FD8BEE76413BC5D7A7E1EEF8972F183AF7206BE80018BF9D6DD7AE5FDC3D929C0C32E1882685DED4532A10A7F2B30711B5829E84178474E88CC8586ED2D1C43B455A66318812FAD8A1EF746B5292E6426F02FD3C3C3DC5331C82659CB2B022928F2DB55AC65494BB38CA4D89DA404B9EE3A4DCD238E39F92215FD46E2492448603D9E97FA248E0BBB36C49C2E083EA7FA3E32415E312835779499EFC87B989A4DD26CE68873FDA7F73C38836BF7DEF01C560DA7558BA08AFFB65591CF37718DB2AA3E2C8B"
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
  height: 320px;
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