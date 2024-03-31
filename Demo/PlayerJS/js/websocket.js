var websocket = function(port) 
{
	this.instance = null
	this.connect = function() 
	{
		var that = this
		var url = getDefaultConn(port)
		if (typeof WebSocket != 'undefined') 
		{
			this.instance = new WebSocket(url)
			this.instance.onopen = function() 
			{
				console.log('WS连接成功')
			}
			this.instance.onerror = function(event) {
				console.log('WS连接失败')
				that.onError(event)		
			}
			this.instance.onmessage = function(msg) {
				that.onMessage(msg)
			}

		} 
		else 
		{
			console.log("您的浏览器不支持Websocket通信协议，请使用Chrome或者Firefox浏览器！")
			this.instance = createObject(getrandom(5))
			this.instance.EnableLog = true
			if (this.instance) 
			{
				if (this.instance.ReadyState > 1) {
					// 还未连接
					this.instance.Connect(url);
				}
			} 		
		}
	}

	this.onMessage = function(msg) {
		//这里处理所有接收到的数据包 
	}

	this.onError = function(event) 
	{
	}

	this.close = function() {
		this.instance.close()
	}

	this.sendObj = function(data) 
	{
		console.log(this.instance.ReadyState)
		var that = this
		if (this.instance.ReadyState <= 1 || this.instance.readyState <= 1) 
		{
			//若是ws开启状态
			this.instance.send(JSON.stringify(data))
		} 
		else 
		{
			// 若未开启 ，则等待1s后重新调用
			setTimeout(function() {
				that.sendObj(data);
			}, 1000);
		}

	}
	return this
}

/*IE中创建WebSocket控件对象*/
function createObject(id) {
	var obj = $('<object></object>');
	obj.attr('ID', id);
	obj.attr('CLASSID', 'CLSID:21ADE2E6-B4DD-4F3E-8BD5-9DDAD1785F3A');/*单机版请替换为C0971B90-4513-4E2D-A0B6-15B915FE748A*/
	obj.attr('width', 0);
	obj.attr('height', 0);
	obj.appendTo('body') 
	return document.getElementById(id)
}

/*获取socket链接*/
function getDefaultConn(port) {
	if (location.protocol.indexOf('https') > -1)
		return 'wss://wrl.zorrosoft.com:' + port + '?sid=' + getrandom(5).toLocaleString() + '&flag=1';
	else
		return 'ws://127.0.0.1:' + port + '?sid=' + getrandom(5).toLocaleString() + '&flag=1';
}

//获取随机数
function getrandom(nums) {
	return ('000000' + Math.floor(Math.random() * 999999)).slice(-6);
}
