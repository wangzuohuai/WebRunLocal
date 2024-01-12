const location_ws = 'ws://127.0.0.1'
const remote_ws = 'wss://wrl.zorrosoft.com'
const flag = 1 //flag为1代表启用日志输出，系统正式上线后设置0可提高运行速度
const cid = 'zorrosoft'

class websocket {
    static wsArr = []
    ws = null
    url = null
    port=''
    type = 0
    constructor(port='',type=0) {
        this.type = type
        this.port = port
        this.connect()
    }
    connect = () => {
        this.url = this.getDefaultConn(this.port,this.type)
        this.ws = new WebSocket(this.url);
        this.ws.onopen = this.onopen
        this.ws.onerror = this.onerror
        this.ws.onclose = this.onclose
    }
    onopen = (event) => {
        console.log("websocket is open");
        //只发送一次，获取office 的连接信息
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
    success(){

    }
    getInstance = () => {
        return this.ws
    }
    onerror = (event) => {
        if (this.ws.readyState == 3) {
            this.connect(this.url)
        }
    }
    onclose = (event) => {
        console.log("websocket is close");
    }
    sendMessage = (msg) => {
        this.ws.send(JSON.stringify(msg));
    }
    //获取ws连接的地址
    getDefaultConn(port='',type=0) {
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
    getRandom(nums) {
        return ('000000' + Math.floor(Math.random() * 999999)).slice(-6);
    }
}
