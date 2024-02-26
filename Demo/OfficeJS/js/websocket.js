const location_ws = 'ws://127.0.0.1'
const remote_ws = 'wss://wrl.zorrosoft.com'
const flag = 1 // flag为 1 代表启用日志输出，系统正式上线后设置 0 可提高运行速度
const cid = 'zorrosoft' // 授权公司唯一标识，购买后获得
websocket.wsArr = []
function websocket(port='',type=0){
    this.ws = null
    this.url = null
    this.type = type
    this.port = port

    this.connect = () => {
        this.url = this.getDefaultConn(this.port,this.type)
        if (typeof WebSocket != 'undefined') 
        {
            this.ws = new WebSocket(this.url);
            this.ws.onopen = this.onopen
            this.ws.onerror = this.onerror
            this.ws.onclose = this.onclose
        }
        else
        {
            console.log("您的浏览器不支持Websocket通信协议，请使用Chrome或者Firefox浏览器！")
			this.ws = createObject(getrandom(5))
			this.ws.EnableLog = true
			if (this.instance) 
			{
				if (this.ws.ReadyState > 1) {
					// 还未连接
					this.ws.Connect(url);
				}
			} 		
        }
    }
    this.getInstance = ()=>{
        return this.ws
    }
    this.onopen = (event) => {
        console.log("websocket is open");
        //只发送一次，获取本机安装的office软件信息，启动时需要判断哪个Office是当前使用的类型Option
        if(websocket.wsArr.length==0){
            let msg = {
                "req": "Wrl_OfficeInfo",
                "rid": 1,
                "para": {}
            }
            this.sendMessage(msg)
        }
        websocket.wsArr.push(this)
        this.success()
    }
    this.success = ()=>{

    }

    this.onerror = (event) => {
        if (this.ws.readyState == 3 && websocket.wsArr.length==0) {
            let isBool = confirm('提示：PageHi办公网页组件 服务端口连接失败，可能是尚未安装，是否马上下载安装？')
            if (isBool) {
                window.open('http://local.zorrosoft.com/Files/PageHiOfficeIns.exe') // 建议打包为zip等格式下载，解压后安装，EXE文件下载浏览器会提示是否保留
            }
        }
    }
    this.onclose = (event) => {
        console.log("websocket is close");
    }
    this.sendMessage = (msg) => {
        this.ws.send(JSON.stringify(msg));
    }
  
    /*IE中创建WebSocket控件对象*/
    this.createObject = (id) => {
        var obj = $('<object></object>');
        obj.attr('ID', id);
        obj.attr('CLASSID', 'CLSID:C0971B90-4513-4E2D-A0B6-15B915FE748A');
        obj.attr('width', 0);
        obj.attr('height', 0);
        obj.appendTo('body') 
        return document.getElementById(id)
    } 

    //获取ws连接的地址
    this.getDefaultConn = (port='',type=0)=>{
        /// flag为1代表启用日志输出，系统正式上线后设置0可提高运行速度
        /// sid代表本次连接的会话ID，必须保证唯一
        let Protocol = location.protocol;
        if (Protocol.toUpperCase().indexOf('HTTPS') > -1)
        {
            port = port=='' ?453:port
            /// HTTPS网站，需要申请自己主站下其它地方都不会用到的一个子域名来做SSL证书，如域名在阿里云上，请下载Apache类型的证书并提交给客服制作授权
            if(1 == type)
                return remote_ws+':'+port+'?sid=' + this.getRandom(5).toLocaleString() + '&flag=1&cid='+cid+'&tk=8C4560272C8A38C32EF6102CAB6B4D886504F06C63202A316B3FD88381FC5491704DA444156B9F6FDA313843E412F1E1DC414A7899399F14D76688090FC7DCE11DA121CB2B0E819B2B7080DB9CF09D4D66192C5893ABE182DA38DF8A02EFAACB304BF9A242ADEBFAA09FC0304918895DE3B56E30A17AA8D92E3D61C1AC2453E6C1C637C3E260FE9A445EC858BADEB9312A43DD99323EF5D63414B9BC7D3F4004C7E109ADD5A6289ADAB004A2A544D312BB84E467DAC4C9449418F3FCCC9529049DCFD562B77EF2CE429B242C23975E6EA922E0564B6507177187E92F254EC2678A795B5D2EC92F818A7364FB7CA3E553D4F94119F868261E5A0A8E7EBE841CF7'; // 这里注意test.yuanmaster.com替换为自己的子域名
            else
                return remote_ws+':'+port+'?sid=' + this.getRandom(5).toLocaleString() + '&flag='+flag; // 这里注意wrl.zorrosoft.com替换为自己的子域名
        }
        else
        {
            port = port=='' ?83:port
            /// type为1时代表中间件启用了安全校验模式，需要按文档生成tk，具体方法参考SDK包中的文档：中间件安全解决方案.pdf
            if(1 == type)
                return location_ws+':'+port+'?sid=' + this.getRandom(5).toLocaleString() + '?flag=1&cid='+cid+'&tk=8C4560272C8A38C32EF6102CAB6B4D886504F06C63202A316B3FD88381FC5491704DA444156B9F6FDA313843E412F1E1DC414A7899399F14D76688090FC7DCE11DA121CB2B0E819B2B7080DB9CF09D4D66192C5893ABE182DA38DF8A02EFAACB304BF9A242ADEBFAA09FC0304918895DE3B56E30A17AA8D92E3D61C1AC2453E6C1C637C3E260FE9A445EC858BADEB9312A43DD99323EF5D63414B9BC7D3F4004C7E109ADD5A6289ADAB004A2A544D312BB84E467DAC4C9449418F3FCCC9529049DCFD562B77EF2CE429B242C23975E6EA922E0564B6507177187E92F254EC2678A795B5D2EC92F818A7364FB7CA3E553D4F94119F868261E5A0A8E7EBE841CF7';
            else
                return location_ws+':'+port+'?sid=' + this.getRandom(5).toLocaleString() + '&flag='+flag;
        }
    }
// 获取随机数
    this.getRandom = (nums)=>{
        return ('000000' + Math.floor(Math.random() * 999999)).slice(-6);
    }
    this.connect()
}


