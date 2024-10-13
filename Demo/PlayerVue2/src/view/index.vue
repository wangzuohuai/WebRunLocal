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
				<div class="item">引擎</div>
				<div class="item">
					<el-select size="small" v-model="SetPlayerEngine" placeholder="请选择" style="width: 130PX;">
						<el-option label="LibVLC" value="LibVLC">
						</el-option>
						<el-option label="FFPlayer" value="FFPlayer">
						</el-option>
						<el-option label="海康私有协议" value="HKSDKPLAY">
						</el-option>
						<el-option label="大华私有协议" value="DHSDKPLAY">
						</el-option>
						<el-option label="海康MP4文件" value="HKMP4PLAY">
						</el-option>
					</el-select>
				</div>
				<div class="item" style="margin-left: 10px;">
					<el-button type="primary" @click="changePlay()" size="small">播放切换</el-button>
				</div>
		</div>
		<div class="urlbox">
			<div class="item">叠加透明网站地址</div>
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

<script>
	//集成开发说明：所有交互都是通过JS建立websocket连接后，发送或接收JSON包进行，都是异步请求，发送和接收包都携带rid用来标识请求返回包对应是哪个请求的结果
	//所有发送的msg中用到的长整形rid 可以自己指定 不同的发送请求定义唯一的rid 这样可以在回调用轻松判断
	//针对一些特殊请求，可以根据自己的实际情况把rid固定，如启动播放时指定固定rid获得启动播放的实例ID
	//首先建立第一个websocket连接到中间件服务端口启动vlc播放小程序 然后获得启动小程序的侦听端口建立第二个websocket连接 主要用来进行控制播放、截图、录像、设置字幕等操作
	//多路播放时分屏窗口id 是固定的参数 从1开始计数 计数顺序为 从左到右，从上到下
	//释放播放小程序，直接关闭建立的第一个websocket连接即可
	import GetDefaultConn from "../common/base.js"
	//加载websoket类
	import websocket from '../common/websocket'
	
	export default {
		components: {

		},

		data() 
		{
			return {
				aid: 0, 	// 第一个播放实例实例ID
				aid2: 0, 	// 保存启动的第二个播放实例ID
				curID:0,	// 当前操作实例ID
				rid: 10, 	// 起始请求序号
				run1:1, 	// 请求启动播放实例序号1
				run2:2, 	// 请求启动播放实例序号2
				win: 1, 	// 播放实例中当前操作的分屏窗口序号ID
				newrtsp: 'http://devimages.apple.com.edgekey.net/streaming/examples/bipbop_4x3/gear2/prog_index.m3u8',
				newfloatweb: 'https://output.jsbin.com/dopavun',
				StartSecond: false, // 是否启动了第二个播放实例
				ReStartPlay: 0, // 第一个播放实例是否重新播放状态
				width: 960,		// 播放窗口显示宽度
				height: 320,	 // 播放窗口显示高度
				left: 0,		// 播放窗口以网页左上角为基点显示的X位置
				top: 0,			// 播放窗口以网页左上角为基点显示的Y位置
				IframeX: -10, 	// 播放窗口模拟iFrame用的X坐标
				IframeY: 0,		// 播放窗口模拟iFrame用的Y坐标
				ShowType: 1, 	//分屏样式
				WebCfg:'[{"ID":2,"Uri":"https://vjs.zencdn.net/v/oceans.mp4","Option":"PlayEngine=2"},{"ID":1,"Uri":"https://media.w3.org/2010/05/sintel/trailer.mp4","Option":"Link=0 PlayEngine=2"},{"ID":4,"Server":"wzh:test123456@192.168.1.2:8000","Option":"Channel=1 Caching=1 Stream=1 Link=0"},{"ID":3,"Uri":"http://www.zorrosoft.com/Files/h265.mkv","Option":"Caching=5 PlayEngine=2"}]',
				WebCfg2:'[{"ID":1,"Uri":"https://vjs.zencdn.net/v/oceans.mp4","Option":"PlayEngine=3"},{"ID":2,"Uri":"https://media.w3.org/2010/05/sintel/trailer.mp4","Option":"Link=0 PlayEngine=3"},{"ID":3,"Server":"wzh:test123456@192.168.1.2:8000","Option":"Channel=33 Caching=3 Stream=0 Link=1"},{"ID":4,"Uri":"http://www.zorrosoft.com/Files/4K_H265.MP4","Option":"Caching=5 PlayEngine=3"}]',
				result: [], //日志结果数组
				socket: [], //websocket对象数组，采用数组需要注意维护索引号，建议改为根据连接的SID参数存到对应的集合，需要用到连接时根据SID提取
				isRecordFile: false, //记录录像的状态
				isConnService:false, //是否连接的中间件服务
				isDisConnect:false, // 是否处于断开连接过程
				RecordFilePath: 'C:/Zorro/test.mp4',
				SetPlayerEngine: 'LibVLC',
				Transform: 'none',
				recordid: 0, //录像时返回的录像进程PID
				danmu: {
					//需要发送的字幕配置信息
					text: '您好呀，O(∩_∩)O哈哈~',
					position: 'TOP',
					color: "#ff0000",
					opacity: 128,
					size: 50,
					x: 0,
					y: 0
				},
				version: '2.2.16.7' //版本信息
			}
		},

		computed: {
			DebugLog() {
				return this.result.join("\n")
			}
		},

		mounted(){
			//初始化配置
			this.init()
			/// 响应改变浏览器窗口大小
			let _this = this
			window.onresize = ()=>{
        		_this.pageResize()
     	 	}
    	},

   	 	destroyed(){
     		window.onresize = null
		},

		beforeDestroy() {
			//关闭所有websocket链接以及浏览器监听
			this.close()
		},

		methods: 
		{
			init() {
				//监听浏览器切换标签页面
				if(document.addEventListener)
					document.addEventListener('visibilitychange', this.handleVisiable,false)
				else
					document.attachEvent('visibilitychange',this.handleVisiable,false)	/// 老版本浏览器
				//侦听滚动条
				window.addEventListener('scroll', this.windowScroll, true)
				//监听当页面离开时候
				window.addEventListener('unload', this.unloadHandler,false)
				//获取播放器位置节点信息
				this.getPlayerPosition()
				//启动VLC小程序
				this.StartRtspPlayer()

			},

			windowScroll() {
				// 滚动条距离页面顶部的距离
				let scrollTop = window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop
				if(this.aid > 0)
					this.appScroll(0, this.aid, scrollTop)
				if (this.aid2 > 0)
					this.appScroll(2, this.aid2, scrollTop)
			},

			appScroll(si, id, scrollTop) {
				if(id)
				{
					// 默认纵向滚动小程序实例，如需要横向滚动，Code设置为1，修改Left的值
					// NoLog指示服务日志不输出相关日志，因为时间比较多，输出日志导致日志文件信息过多
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletScroll",
						"rid": this.rid,
						"para": {
							"ID": id,
							"NoLog": 1,
							"Code": 2,
							"Left": 0,
							"Top": Math.round(scrollTop)
						}
					}

					this.socket[si].sendObj(msg)
				}
			},

			getPlayerPosition() {
				//获取播放器位置节点信息
				let nScrollTop = 0
				let nScrollLeft = 0
				let bHorizontalBar =this.hasHorizontalScrollbar()
				let bVerticalBar = this.hasVerticalScrollbar()
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

				let react = this.$refs.player.getBoundingClientRect()
				this.left = Math.round(react.left) + nScrollLeft
				this.top = Math.round(react.top) + nScrollTop
			},

			handleVisiable(e) {
				//浏览器页面切换侦听回调函数
				if (e.target.visibilityState == 'hidden') {
					//切离该页面时执行
					this.hideApp(4)
				} else if (e.target.visibilityState == 'visible') {
					//切换到该页面时执行
					this.showApp()
				}
			},

			hasVerticalScrollbar(){
				if(document.documentElement.clientHeight)
					return document.body.scrollHeight > document.documentElement.clientHeight
   				return document.body.scrollHeight > window.innerHeight
			},

			hasHorizontalScrollbar(){
				if(document.documentElement.clientWidth)
					return document.body.scrollWidth > document.documentElement.clientWidth
    			return document.body.scrollWidth > window.innerWidth
			},

			pageResize(){
				if(this.aid > 0)
					this.SendScrollInfo(0,this.aid)
				if(this.aid2 > 0)
					this.SendScrollInfo(2,this.aid2)
			},

			SendScrollInfo(si,id){
				let nScrollTop = 0
				let nScrollLeft = 0
				let BarCode = 0
				let bHorizontalBar = this.hasHorizontalScrollbar()
				let bVerticalBar = this.hasVerticalScrollbar()
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
				this.rid++ // 增加序号
				/// 设置页码滚动信息，BarW BarH分别为预留右侧宽度和底部高度
				let msg = {
					"req": "Wrl_ScrollBar",
					"rid": this.rid,
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
				this.socket[si].sendObj(msg)
			},

			unloadHandler() {
				//关闭所有websoket链接
				this.close()
			},

			close() {
				//关闭播放器
				this.CloseAllPlayer()
				this.isDisConnect = true
				//关闭第一个websoket链接
				this.socket[0].disconnect()
				this.socket.pop()
				this.isDisConnect = false
				//关闭侦听浏览器页面切换
				if(document.addEventListener)
					document.removeEventListener('visibilitychange', this.handleVisiable,false)
				else
					document.detachEvent('visibilitychange',this.handleVisiable,false)	/// 老版本浏览器
				//关闭侦听滚动条
				window.removeEventListener('scroll', this.windowScroll, true)
				//关闭侦听滚动条
				window.removeEventListener('unload', this.unloadHandler,false)
			},

			StartRtspPlayer() {
				this.isConnService = true
				//启动第一个多引擎网页小程序 Open为播放源，播放源也可以放到Web节点中，参考RePlayFirst中实现，差异在于，Open中指定对所有分屏有效，而在Web节点中可指定更多播放参数
				let Protocol = location.protocol;
				if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
				{
					// HTTPS网站，连接WSS侦听端口
					this.openWebsocket(453,0)
				}
				else
				{
					this.openWebsocket(83,0)
					//this.openWebsocket(83,1) 支持安全校验机制
				}

				let msg = {
					"req": "Wrl_VideoWebPlayer",
					"rid": this.run1,
					"para": {
						"Type": "0",
						"Title": "RTSP多引擎网页播放器1",
						"Version": 0,
						"Flag": 578,
						"Left": this.left,
						"Top": this.top,
						"Width": this.width,
						"Height": this.height,
						"IframeX": this.IframeX,
						"IframeY": this.IframeY,
						"BarW": 0,
						"BarH": 0,
						"ScrollTop": 0,
						"Web": [{"ID":1,"Uri":"http%3A%2F%2Fwww.zorrosoft.com%2FFiles%2FPluginOKBrowserApplet.mp4","Option":"PlayEngine=2"}],
						"ShowType": this.ShowType
					}
				}
				this.socket[0].sendObj(msg)
			},

			openSecondPlayer() {
				if(this.aid)
				{
					//演示打开第二个播放小程序实例 先改变第一个小程序位置 Flag值需要在原来基础上+512以支持多实例播放，Version设置1启用多进程播放模式
					this.width = 475
					this.resize(0)
					this.isConnService = true
					let Protocol = location.protocol;
					if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
					{
						// HTTPS网站，连接WSS侦听端口
						this.openWebsocket(453,0)
					}
					else
					{
						this.openWebsocket(83,0)
						//this.openWebsocket(83,1) 支持安全校验机制
					}

					let msg = {
						"req": "Wrl_VideoWebPlayer",
						"rid": this.run2,
						"para": {
							"Type": "0",
							"Title": "RTSP多引擎网页播放器2",
							"Version": 1,
							"Flag": 578,
							"Left": this.left + 485,
							"Top": this.top,
							"Width": this.width,
							"Height": this.height,
							"IframeX": -10,
							"IframeY": 0,
							"BarW": 0,
							"BarH": 0,
							"ScrollTop": 0,
							"Web": JSON.parse(this.WebCfg),
							"ShowType": 4
						}
					}
					this.socket[2].sendObj(msg)
					this.StartSecond = true
				}
				else{
					this.$message.success('请先启动第一个播放器')
				}
			},

			CloseSecondPlayer() {
				if (this.StartSecond) {
					this.isDisConnect = true
					/// 每个播放实例占用2个连接，一个到中间件、一个到播放小程序，分别断开连接并释放
					this.socket[3].disconnect()
					this.socket[2].disconnect()
					this.socket.pop()
					this.socket.pop()
					this.isDisConnect = false
					this.StartSecond = false
					this.aid2 = 0
					//还原第一个小程序位置
					this.width = 960
					this.height = 320
					//获取播放器位置节点信息
					let react = this.$refs.player.getBoundingClientRect()
					this.left = react.left
					this.top = react.top
					this.resize(0)
				}
			},

			CloseFirstPlayer()
			{
				if (this.aid > 0) 
				{
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletControl",
						"rid": this.rid,
						"para": {
							"ID": this.aid,
							"Code": 1
						}
					}
					this.isDisConnect = true
					/// 释放到播放小程序的连接
					this.socket[1].disconnect()
					this.socket[0].sendObj(msg)
					this.aid = 0
					this.ReStartPlay = 0
					if(this.StartSecond)
						this.socket.splice(1,1)
					else
						this.socket.pop()
					this.isDisConnect = false
				}
			},

			RePlayFirst()
			{
				this.CloseFirstPlayer()
				this.ReStartPlay = 1
				//重新启动播，Web节点配置播放源，设置分屏风格，播放引擎从VLC改为FFPlayer
				this.ShowType = 4
				let Msg = {
					"req": "Wrl_VideoWebPlayer",
					"rid": this.run1,
					"para": {
						"Type": "0",
						"Title": "RTSP多引擎网页播放器",
						"Version": 0,
						"Flag": 578,
						"Left": this.left,
						"Top": this.top,
						"Width": this.width,
						"Height": this.height,
						"IframeX": this.IframeX,
						"IframeY": this.IframeY,
						"BarW": 0,
						"BarH": 0,
						"ScrollTop": 0,
						"Web": JSON.parse(this.WebCfg2),
						"ShowType": this.ShowType
					}
				}
				this.socket[0].sendObj(Msg)
			},

			CloseAllPlayer()
			{
				/// 先关闭第二个实例，否则socket连接序号会错
				this.CloseSecondPlayer()
				this.CloseFirstPlayer()
			},

			StopSecondPlayer()
			{
				if (this.StartSecond)
				{
					this.rid++ // 增加序号
					let msg = {
						"req": "Player_Control",
						"rid": this.rid,
						"para": [{
							"ID": 1,
							"Type": 2
						},{
							"ID": 2,
							"Type": 2
						},{
							"ID": 3,
							"Type": 2
						},{
							"ID": 4,
							"Type": 2
						}]
					}
					console.log(msg)
					this.socket[3].sendObj(msg)
				}
			},

			StopFirstPlayer()
			{
				if (this.aid > 0)
				{
					this.rid++ // 增加序号
					/// 支持批量停止播放 暂停播放等操作 para中具体传多少个分屏窗口数据自己定，分屏窗口数量由ShowType决定
					/// ID是分屏窗口序号，从1开始 序号原则是从左向右开始编号，然后从上到下开始顺序编号，如遇右侧多层排列窗口时，直到右侧窗口序号排序完成
					/// Type可以是这些值：Pause,Play,Stop,StopAsync,Next,Prev,Clear,TogglePause 分别对应暂停播放 播放 停止播放 异步停止播放 下一个播放源 上一个播放源 清理播放列表 切换暂停状态
					let msg = {}
					if(this.ReStartPlay)
					{
						msg = {
							"req": "Player_Control",
							"rid": this.rid,
							"para": [{
							"ID": 1,
							"Type": 2
							},{
								"ID": 2,
								"Type": 2
							},{
								"ID": 3,
								"Type": 2
							},{
								"ID": 4,
								"Type": 2
							}]
						}
					}
					else{
						/// 不指定分屏信息时，停止所有播放
						msg = {
							"req": "Player_Control",
							"rid": this.rid,
							"para": []
						}
					}
					console.log(msg)
					this.socket[1].sendObj(msg)
				}
			},

			StopAllPlay()
			{
				this.StopSecondPlayer()
				this.StopFirstPlayer()
			},

			openWebsocket(port,type) {
				//打开websocket服务
				let ws = GetDefaultConn(port,type)
				const socketClient = new websocket(ws, {
					reconnectEnabled: false
				})
				socketClient.connect()
				if(1 == this.ReStartPlay && this.StartSecond)
				{
					this.ReStartPlay = 2
					this.socket.splice(1,0,socketClient) // 重新播放时已经启动第二个播放实例，采用插入方式，避免WS下标错误，确保第一个播放实例的WS连接下标是0和1，第二个播放实例是3和4
				}
				else
				{
					if(1 == this.ReStartPlay)
						this.ReStartPlay = 2
					this.socket.push(socketClient)
				}
				socketClient.onMessage = (msg) => 
				{
					console.log(msg.data)
					let res = JSON.parse(msg.data)
					if (res.event == 'Wrl_AppletOK') {
						//小程序创建成功后根据事件名称获取当前小程序id
						if (res.rid == this.run1) {
							this.aid = res.aid
							this.SendScrollInfo(0,this.aid)
							console.log(this.aid)
						}
						if (res.rid == this.run2) {
							this.aid2 = res.aid
							this.SendScrollInfo(2,this.aid2)
							console.log(this.aid2)
						}
					}
					if (res.event == 'Wrl_Listen') {
						//小程序建立侦听成功 这里得到当前小程序返回的端口 
						//这里创建另外一个websocket到播放程序端口 来实现重新指定播放源，暂停播放，抓图等
						let _this = this
						setTimeout(function()  {
							_this.isConnService = false
							_this.openWebsocket(res.data.port,0)
						}, 200);
					}
					//切换窗口的时候 设定当前窗口为此窗口
					if (res.event == 'Player_Selected') {
						this.win = res.ID // 选中的分屏窗口序号
						if(res.aid > 0)
						{
							this.curID = res.aid
							//this.result.push("当前操作播放器实例 " + res.aid)
						}
					}
					// 播放窗口收到鼠标按下通知
					if (res.event == 'Player_MouseDown') {
						if(res.aid > 0)
						{
							this.curID = res.aid
							//this.result.push("当前操作播放器实例 " + res.aid)
						}
					}
					// 定时录像结束通知
					if (res.event == 'Player_StopRecord') {
						this.isRecordFile = false;
						this.recordid = 0;
						this.$message.success("定时录像成功\n" + res.data.File);
					}
					// 设置文件保存位置结果通知
					if (res.event == 'Wrl_SelectFile') {
						if(res.data.length)
						{
							this.BeginRecordFile(res.data[0].File) /// 使用返回的文件位置
						}
						else
							this.BeginRecordFile(this.RecordFilePath) // 取消指定，使用默认位置
					}
					//请求录像返回
					if (res.rid == 90000) {
						//记录录像的pid
						this.recordid = res.data.PID
					}
					//请求停止录像返回
					if (res.rid == 90001) {
						this.recordid = 0;
						this.$message.success("录像成功\n" + res.data.File)
						//console.log(res.data.File)
					}
					//请求截图返回
					if (res.rid == 90002) {
						this.$message.success("截图成功\n" + res.data.Img[0].File)
						//console.log(res.data.Img[0].File)
					}
					if (res.err)
					{
						// 请求返回错误
						this.$message.success(res.err)
					}
					if (res.req == 'Wrl_Version') 
					{
						if (res.data.Update == 1) {
							/// 先强制隐藏播放小程序，避免弹框显示不出来
							this.hideApp(32);
							this.$confirm('有新版本发布, 是否马上升级?', '提示', {
								confirmButtonText: '确定',
								cancelButtonText: '取消',
								type: 'warning'
							}).then(() => {
								/// 先提前关闭播放小程序
								this.CloseAllPlayer()
								this.SendUpdateJson()
							}).catch(() => {
								this.showApp()
							})
						} else {
							this.$message.success('已经是最新版本！')
							/// 说明：即使版本号是最新的，也是可以发起升级请求SendUpdateJson()的，相当于覆盖安装，用于更新比如中间件或播放小程序的配置文件等
						}
					}
					//记录日志
					this.result.push(msg.data)
				}
				socketClient.onClose = (msg) => {
					console.log(msg)
				}
				socketClient.onError = (msg) => {
					// 避免IE中点击重复播放及firefox断开连接提示等问题
					if(!this.ReStartPlay && this.isConnService && !this.isDisConnect)
					{
						//连接不上，认为还没有RTSP多引擎低延迟网页播放器 没有安装时提示安装
						this.$confirm('PageHiPlayer—RTSP多引擎低延迟网页播放器 服务端口连接失败，可能尚未安装，是否马上下载安装？', '提示', {
							confirmButtonText: '确定',
							cancelButtonText: '取消',
							type: 'warning'
						}).then(() => {
							window.open('http://local.zorrosoft.com/Files/RtspWebPlayerIns.exe') // 建议打包为zip等格式下载，解压后安装，EXE文件下载浏览器会提示是否保留
						}).catch(() => {

						})
					}
				}
			},

			resize(position) {
				//请求改变播放实例小程序显示位置或大小，如不需要改变显示位置，不传X和Y
				if(this.aid2 > 0 && this.aid2 == this.curID){
					this.rid++ // 增加请求序号
					if(position){
						let msg = {
							"req": "Wrl_AppletResize",
							"rid": this.rid,
							"para": {
								"ID": this.aid2,
								"X": this.left,
								"Y": this.top,
								"Width": this.width,
								"Height": this.height
							}
						}
						this.socket[2].sendObj(msg)
					}
					else{
						let msg = {
							"req": "Wrl_AppletResize",
							"rid": this.rid,
							"para": {
								"ID": this.aid2,
								"Width": this.width,
								"Height": this.height
							}
						}
						this.socket[2].sendObj(msg)
					}
				}
				else
				{
					if(this.aid > 0){
						this.rid++ // 增加请求序号
						if(position){
							let msg = {
								"req": "Wrl_AppletResize",
								"rid": this.rid,
								"para": {
									"ID": this.aid,
									"X": this.left,
									"Y": this.top,
									"Width": this.width,
									"Height": this.height
								}
							}
							this.socket[0].sendObj(msg)
						}
						else{
							let msg = {
								"req": "Wrl_AppletResize",
								"rid": this.rid,
								"para": {
									"ID": this.aid,
									"Width": this.width,
									"Height": this.height
								}
							}
							this.socket[0].sendObj(msg)
						}
					}
				}
			},

			getCapture() {
				//请求截图，固定请求ID处理，也可以是建立一个map记录每个rid对应的含义，请求返回里再处理
				let msg = {
					"req": "Player_SnapshotImg",
					"rid": 90002,
					"para": [{
						"ID": this.win,
						"Type": 4,
						"Count": 1,
						"Base64":0,
						"Delay": 1000,
						"Interval": 200,
						"PathType": 1
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			setText() {
				//发送字幕到指定的窗口ID
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_MarqueePut",
					"rid": this.rid,
					"para": [{
						"ID": this.win,
						"Text": this.danmu.text,
						"Position": this.danmu.position,
						"Timeout": 0,
						"Color": this.danmu.color,
						"Opacity": this.danmu.opacity,
						"Refresh": 1,
						"Size": this.danmu.size,
						"X": this.danmu.x,
						"Y": this.danmu.y
					}]
				}
				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			setType() {
				//动态改变屏幕数量
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ShowType": this.ShowType
					}
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			NextFrame()
			{
				//请求显示下一帧 请求后播放进入暂停状态，可继续请求下一帧，或请求播放恢复正常播放状态
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_NextFrame",
					"rid": this.rid,
					"para": [{
						"ID":this.win,
						"FrameCount": 1
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			PlayPause()
			{
				//请求播放进入暂停状态
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_Control",
					"rid": this.rid,
					"para": [{
						"ID":this.win,
						"Type": 3
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			PlayContinue()
			{
				//请求播放继续，或请求播放恢复正常播放状态
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_Control",
					"rid": this.rid,
					"para": [{
						"ID":this.win,
						"Type": 4
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			PlayMute()
			{
				// 设置选中分屏窗口视频静音,1静音 0不静音
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_PutSoundStatus",
					"rid": this.rid,
					"para": [{
						"ID":this.win,
						"Mute": 1
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			PlaySpeed()
			{
				// 调整播放速度,仅限于文件或回放流，支持批量操作，当前演示针对选中分屏窗口设置2倍数播放
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_PutPlayRate",
					"rid": this.rid,
					"para": [{
						"ID":this.win,
						"Rate": 2
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			PlayFullScreen()
			{
				// 设置选中分屏窗口全屏显示 全屏后按ESC 字幕F\、双击或点击工具栏全屏图标可退出全屏状态
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_Fullscreen",
					"rid": this.rid,
					"para": [{
						"ID":this.win
					}]
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			SetSelWnd() {
				//设置选中分屏子窗口
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_SetSelect",
					"rid": this.rid,
					"para": {
						"ID": this.win
					}
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
				{
					this.result.push("执行播放器实例 " + this.curID)
					this.socket[3].sendObj(msg)
				}
				else
					this.socket[1].sendObj(msg)
			},

			AppletSnap() {
				// 整个播放窗口抓图，包含所有分屏子窗口，发送到中间件侦听端口执行
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Wrl_AppletSnap",
					"rid": this.rid,
					"para": {
						"ID": this.curID,
						"File": ".jpg",
						"Base64": 1
					}
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[2].sendObj(msg)
				else
					this.socket[0].sendObj(msg)
			},

			FullApplet() {
				// 整个播放窗口全屏，包含所有分屏子窗口，发送到中间件侦听端口执行
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Wrl_AppletControl",
					"rid": this.rid,
					"para": {
						"ID": this.curID,
						"Code": 2
					}
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[2].sendObj(msg)
				else
					this.socket[0].sendObj(msg)
			},

			RecordToFile() {
				//对指定的窗口ID进行录像 固定请求ID处理，也可以是建立一个map记录每个rid对应的含义，请求返回里再处理
				if (this.isRecordFile) {
					let msg = {
						"req": "Player_StopRecord",
						"rid": 90001,
						"para": [{
							"ID": this.win
						}]
					}
					this.isRecordFile = false
					if(this.aid2 > 0 && this.aid2 == this.curID)
						this.socket[3].sendObj(msg)
					else
						this.socket[1].sendObj(msg)
				} else {
					this.rid++ // 增加请求序号
					/// 先设置保存位置再录像
					let msg = {
						"req": "Wrl_SelectFile",
						"rid": this.rid,
						"para": {
							"Type": 1,
							"Title" : "请设置录像文件存放位置",
							"Ext" : "录像文件(*.mp4)\r*.mp4"
						}
					}
					if(this.aid2 > 0 && this.aid2 == this.curID)
						this.socket[2].sendObj(msg)
					else
						this.socket[0].sendObj(msg)
				}
			},

			BeginRecordFile(LocalFilePath)
			{
				let msg = {
						"req": "Player_RecordFile",
						"rid": 90000,
						"para": [{
							"ID": this.win,
							//不指定Url时取当前焦点分屏窗口源进行录像
							//"Url": encodeURIComponent(this.newrtsp),
							"File": encodeURIComponent(LocalFilePath),
							"Second": 30
						}]
					}
					this.isRecordFile = true
					if(this.aid2 > 0 && this.aid2 == this.curID)
						this.socket[3].sendObj(msg)
					else
						this.socket[1].sendObj(msg)
			},

			watermask() {
				//对指定的窗口ID发送水印，只支持VLC引擎播放，其它引擎播放，可采用叠加透明网页的方式，参考FloatWebInfo的实现
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_PutLogoShow",
					"rid": this.rid,
					"para": [{
						"ID": this.win,
						"File": "VLC.png",
						"Delay": 20,
						"Repeat": -1,
						"Opacity": 128,
						"X": 100,
						"Y": 100
					}]
				}
				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			changePlay() {
				//动态改变指定的窗口ID的播放源，Player_ChangePlay本身支持同时改变多个分屏窗口播放源，Play支持传数组 ForceDestroy指定是否先销毁原来播放引擎 默认不销毁
				this.rid++ // 增加请求序号
				let msg = {}
				//切换的播放引擎 0:海康私有协议 1:海康MP4文件 2:LibVLC 3:FFPlayer 4:大华私有协议
				if('FFPlayer' == this.SetPlayerEngine)
				{				
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": "PlayEngine=3 Link=0"
						}]}		
					}
				}
				else if('LibVLC' == this.SetPlayerEngine)
				{
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": `PlayEngine=2 Link=0 Transform=${this.Transform}`
						}]}	
					}
				}
				else if('HKSDKPLAY' == this.SetPlayerEngine)
				{
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": "PlayEngine=0 Link=0"
						}]}		
					}
				}
				else if('DHSDKPLAY' == this.SetPlayerEngine)
				{
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": "PlayEngine=4 Link=0"
						}]}		
					}
				}
				else if('HKMP4PLAY' == this.SetPlayerEngine)
				{
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": "PlayEngine=1 Link=0"
						}]}		
					}
				}
				else
				{
					msg = {
					"req": "Player_ChangePlay",
					"rid": this.rid,
					"para": {
						"ForceDestroy":0,
						"Play": [{
							"ID": this.win,
							"Uri": encodeURIComponent(this.newrtsp),
							"Option": "Link=0"
						}]}
					}
				}

				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			FloatWebInfo() {
				//对指定的窗口ID叠加网页内容显示
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Player_FloatWebInfo",
					"rid": this.rid,
					"para": [{
						"ID": this.win,
						"Url": encodeURIComponent(this.newfloatweb),
						"Rect":{"P":4,"W":300,"H":300}
					}]
				}
				if(this.aid2 > 0 && this.aid2 == this.curID)
					this.socket[3].sendObj(msg)
				else
					this.socket[1].sendObj(msg)
			},

			showApp() {
				//显示VLC小程序
				if (this.aid > 0) {
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletControl",
						"rid": this.rid,
						"para": {
							"ID": this.aid,
							"Code": 8
						}
					}
					this.socket[0].sendObj(msg)
				}
				//如果启动了第二个小程序 也显示
				if (this.aid2 > 0) {
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletControl",
						"rid": this.rid,
						"para": {
							"ID": this.aid2,
							"Code": 8
						}
					}
					this.socket[2].sendObj(msg)
				}
			},

			hideApp(code) {
				//隐藏VLC小程序 Code设置4是自动隐藏，如需强制隐藏，设置为32
				if (this.aid > 0) 
				{
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletControl",
						"rid": this.rid,
						"para": {
							"ID": this.aid,
							"Code": code
						}
					}
					this.socket[0].sendObj(msg)
				}
				//如果启动了第二个小程序 也同时隐藏
				if (this.aid2 > 0) 
				{
					this.rid++ // 增加请求序号
					let msg = {
						"req": "Wrl_AppletControl",
						"rid": this.rid,
						"para": {
							"ID": this.aid2,
							"Code": code
						}
					}
					this.socket[2].sendObj(msg)
				}
			},

			CheckUpdate() {
				//校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的小程序，Wrl_Version功能多
				this.rid++ // 增加请求序号
				let msg = {
					"req": "Wrl_Version",
					"rid":this.rid,
					"para": {
						"Version": this.version
					}
				}
				this.socket[0].sendObj(msg)
			},
			
			SendUpdateJson() {
				// 发送中间件的升级命令，实现自动升级，同时升级海康及VLC网页播放等小程序
				// 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成，此处举例的升级包是在线公测版的，正式版需要自己制作
				this.rid++ // 增加请求序号
				let msg = {
					"req":"Wrl_Update",
					"rid":this.rid,
					"para":{
						"Name":"PageHiPlayer—RTSP多引擎低延迟网页播放器升级包",
	  					"Date":"2024-10-13",
						"Desc":"优化中间件高级版启动内嵌网页小程序处理过程，解决个别情况启动失败问题，优化内嵌网页小程序的窗口激活及输入焦点处理，优化WS连接释放过程...",
						"DownAddr":"http://local.zorrosoft.com/Files/Update/RTSP_Update.pid",
						"Open":"http://local.zorrosoft.com/Player",
						"MD5":"2468C378C08EF0BD42E3DA645DC596FD",
						"Version":"2.2.16.7",
						"Size":43909120,
						"HideIns":0,
						"Cookie":"",
						"Auth":"",
						"TK":"6A8E4AFC690EE792087459569BEAD8ED7FCAB1FBAA0582416CBE7588D86DD83F993A8AFF5B8EB26AA24B517909D059F1D6CEA1FC2B3D7E6CB2C8CEFC1409A9951C96D5E1A572096063A6818D3EBAE0C969C5F626461B30164F865A0CD8796E4C176A707C6DFBC6B8BC01FCF1DBE3DD8288DCBF0180C08F2063D734DA6B5C9AE892C1B42B8A1976512F6FD2AAF05BD05B3688DF8A586FEDEC4AF10D0F213879F01894E806CB34B5B9B366BBC63E556BA6DAFB91D435724178CBA4A6C4B8A92A2644765D19F63E58C59477D0EBB9C7B321AFC446E5F01DE7EE98A6D724DE20A789FC5614C5300121C8E0B96E1F62A7C4BE4CE5307FB3CB7B9F826E07B96555C570"
					}
				}
				this.socket[0].sendObj(msg)
			}
		}
	};
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
