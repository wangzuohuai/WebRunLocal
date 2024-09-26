function CADApplet(ws = null, aid = 0) {
    this.ws = ws
    this.aid = aid
    this.openType = 0  // 加载类型 0默认打开软件在线编辑图纸 1 OCX看图
    this.IframeX = -10 //根据情况自己修改把
    this.IframeY = 0  //根据情况自己修改把
}

CADApplet.rid = 1000; //每次请求都要传递一个rid 参数 这里弄成静态变量 每次累加一次就行
//维护一个数组 用来保存Applet实例的aid
CADApplet.aidArr = [
    {aid: 0, rid: 2, ws1: null, ws2: null},
    {aid: 0, rid: 3, ws1: null, ws2: null},
    {aid: 0, rid: 4, ws1: null, ws2: null},
    {aid: 0, rid: 5, ws1: null, ws2: null},
    {aid: 0, rid: 6, ws1: null, ws2: null},
    {aid: 0, rid: 7, ws1: null, ws2: null},
    {aid: 0, rid: 8, ws1: null, ws2: null},
    {aid: 0, rid: 9, ws1: null, ws2: null},
];
/**
 * 让rid和aid和ws都映射起来
 * @param rid
 * @param ws
 * @param type=1或者type=2 根据文档来选择例如操作的是宽度 边距等 需要使用第一个ws
 */
CADApplet.setAidArr = function (rid = 0, aid = 0, type = 1, ws = null) {
    CADApplet.aidArr.forEach((item, index) => {
        if (item.rid == rid) {
            CADApplet.aidArr[index].aid = aid
            if (type == 1) {
                CADApplet.aidArr[index].ws1 = ws
            }
            if (type == 2) {
                CADApplet.aidArr[index].ws2 = ws
            }
            if (type == 0) {
                CADApplet.aidArr[index].ws2 = null
                CADApplet.aidArr[index].ws1 = null
            }
        }
    })
}

CADApplet.getAidArr = function (rid) {
    for (let i in CADApplet.aidArr) {
        if (CADApplet.aidArr[i].rid == rid) {
            return CADApplet.aidArr[i].aid
        }
    }
    return 0
}
/**
 * 根据操作的类型 来选择ws
 *
 * @param aid
 * @param type 根据文档来选择 例如操作的是宽度 边距等 需要使用第一个ws
 */
CADApplet.prototype.setWebsocket = function (aid, type = 1) {
    this.aid = aid
    for (let i in CADApplet.aidArr) {
        if (CADApplet.aidArr[i].aid == aid) {
            if (type == 1) {
                this.ws = CADApplet.aidArr[i].ws1
            } else {
                this.ws = CADApplet.aidArr[i].ws2
            }
        }
    }
}
/**
 * 启动AutoCADCAD网页组件
 * @param rid
 * @param left
 * @param top
 * @param width
 * @param height
 * @param OpenFile
 */
CADApplet.prototype.startACAD = function (rid, left, top, width, height, OpenFile, edit) {
    //启动一个AutoCAD网页组件，参数参考CADApplet-class.js中的startACAD
    let msg = {
        "req": "Wrl_ACADApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "CAD网页组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Edit": edit, "Hide": 0, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 0,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartDwgView = function (rid, left, top, width, height, OpenDwg) {
    //启动第二个CAD网页组件，参数参考CADApplet-class.js中的startACAD
    let msg = {
        "req": "Wrl_ACADApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "DWG看图组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Hide": 0, "User": "zorro", "Cookie": ""},
            "Open": OpenDwg,
            "Option": 1
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartSolidworksApplet = function (rid, left, top, width, height, OpenFile, edit) {
    //启动一个Solidwork网页组件，参数参考CADApplet-class.js中的startACAD Option设置1时代表启动eDrawings看图
    let msg = {
        "req": "Wrl_SWCadApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "CAD网页组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Edit": edit, "Hide": 0, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 0,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartCatiaApplet = function (rid, left, top, width, height, OpenFile, edit) {
    //启动一个StartCatiaApplet网页组件，参数参考CADApplet-class.js中的startACAD
    let msg = {
        "req": "Wrl_CatiaApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "CAD网页组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Edit": edit, "Hide": 0, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 0,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartProEApplet = function (rid, left, top, width, height, OpenFile, edit) {
    //启动一个ProE网页组件，参数参考CADApplet-class.js中的startACAD Option设置1时代表启动Creo View看图 暂时不支持设置0
    let msg = {
        "req": "Wrl_ProEApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "CAD网页组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Edit": edit, "Hide": 0, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 1,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartZWCadApplet = function (rid, left, top, width, height, OpenFile, edit) {
    //启动一个中望CAD网页组件，参数参考CADApplet-class.js中的startACAD
    let msg = {
        "req": "Wrl_ZWApplet",
        "rid": rid,
        "para": {
            "Type": "0",
            "Title": "CAD网页组件",
            "Flag": 578,
            "Left": left,
            "Top": top,
            "Width": width,
            "Height": height,
            "IframeX": this.IframeX,
            "IframeY": this.IframeY,
            "BarW": 0,
            "BarH": 0,
            "ScrollTop": 0,
            "Web": {"Edit": edit, "Hide": 0, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 0,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.saveFile = function () {
    // 请求保存文档
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_Save",
        "rid": CADApplet.rid,
        "para": {}
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.saveAsFile = function () {
    CADApplet.rid++ // 增加请求序号
    /// 先设置保存位置再另存
    let msg = {
        "req": "Wrl_SelectFile",
        "rid": CADApplet.rid,
        "para": {
            "Type": 1,
            "Title": "请设置另存文件位置",
            "Ext": "另存图纸(*.dwg;*.dxf)\r*.dwg;*.dxf"
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.ExportFile = function () {
    // 请求导出文档
    CADApplet.rid++ // 增加请求序号
    /// 先设置导出位置
    let msg = {
        "req": "Wrl_SelectFile",
        "rid": CADApplet.rid,
        "para": {
            "Type": 1,
            "Title": "请设置导出文件位置",
            "Ext": "导出文件(*.pdf;*.png)\r*.pdf;*.png"
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.printFile = function (url) {
    // 请求打印当前文档
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_Print",
        "rid": CADApplet.rid,
        "para": {
            "Count": 1
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.appletFullEdit = function () {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_SwitchFullScreen",
        "rid": CADApplet.rid,
        "para": {}
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.closeApplet = function () {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletControl",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "Code": 1
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.checkUpdate = function (version) {
    //校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的网页组件，Wrl_Version功能多
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_Version",
        "rid": CADApplet.rid,
        "para": {
            "Version": version
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.changeOpen = function (myOpenDoc) {
    //重新打开文档
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_Open",
        "rid": CADApplet.rid,
        "para": {
            "Open": encodeURIComponent(myOpenDoc)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.resizeMargin = function (left, top, width, height) {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletResize",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "X": left,
            "Y": top,
            "Width": width,
            "Height": height
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.resize = function (width, height) {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletResize",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "Width": width,
            "Height": height
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.showApp = function () {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletControl",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "Code": 8
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.hideApp = function (code) {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletControl",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "Code": code
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.appScroll = function (scrollLeft = 0, scrollTop = 0) {
    // 默认纵向滚动网页组件实例，如需要横向滚动，Code设置为1，修改Left的值
    // NoLog指示服务日志不输出相关日志，因为时间比较多，输出日志导致日志文件信息过多
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletScroll",
        "rid": CADApplet.rid,
        "para": {
            "ID": this.aid,
            "NoLog": 1,
            "Code": 2,
            "Left": 0,
            "Top": Math.round(scrollTop)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.sendScrollInfo = function (BarCode = 2, scrollLeft = 0, scrollTop = 0) {
    /// 设置页码滚动信息，BarW BarH分别为预留右侧宽度和底部高度
    console.log(scrollLeft,scrollTop)
    if(this.ws)
    {
        CADApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_ScrollBar",
            "rid": CADApplet.rid,
            "para": {
                "ID": this.aid,
                "BarW": 0,
                "BarH": 0,
                "Code": BarCode,
                "Left": Math.round(scrollLeft),
                "Top": Math.round(scrollTop)
            }
        }
        this.ws.sendMessage(msg)    
    }
}

CADApplet.prototype.beginSaveAsFile = function (LocalFilePath) {
    // 请求开始另存文档
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_SaveAs",
        "rid": CADApplet.rid,
        "para": {
            "NewFile": encodeURIComponent(LocalFilePath)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.beginExpportFile = function (LocalFilePath) {
    // 请求开始另存文档
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "CAD_ConvertTo",
        "rid": CADApplet.rid,
        "para": {
            "DestFile": encodeURIComponent(LocalFilePath)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.sendUpdateJson = function () {
    // 发送中间件的升级命令，实现自动升级，同时升级AutoCAD、CATIA、Solidworks、ProE等CAD在线编辑组件
    // 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_Update",
        "rid": CADApplet.rid,
        "para": {
            "Name": "PageHiCAD-图纸在线编辑组件升级包",
            "Date": "2024-09-26",
            "Desc": "1、中间件水印版服务WS连接TK校验时有效期修改为不能超过30天，优化高级版内嵌小程序启动稳定性，改进非系统服务方式运行启动时要求管理员权限的时机；2、中间件解决下载0字节大小文件问题，优化下载时临时文件名生成算法，避免过长文件名导致错误，上传文件时请求地址增加PHPostSize和PHPostMD5两个参数，接收方可提前获取数据包大小和MD5哈希值；3、解决中间件服务进程异常崩溃后服务启动后加载内嵌小程序可能失败问题，异常重启后小程序编号不重新计数，解决内嵌小程序启动可能偶尔卡死问题，解决上一版引入的系统hosts文件内容丢失数据问题；4、PageHiCAD网页组件改进在副屏启动的显示效果，解决窗口显示大小可能错误问题..",
            "DownAddr": "http://local.zorrosoft.com/Files/Update/CAD_Update.pid",
            "Open": "http://local.zorrosoft.com/CADJS",
            "MD5": "F98B8C09F8B6F30CD5DD2F7F9B2CA595",
            "Version": "2.2.16.6",
            "Size": 23887872,
            "HideIns": 0,
            "Cookie": "",
            "Auth": "",
            "TK": "095D406AF1A3DCBBEA5F5E300FB8C3C4082041BF7A66FE08ADF176EAA93B5FC0764CB62EC0480EFFE8A5A72165CAD78DFF9C0DA94D1187C737E46865E4F2C445257E0B4C63111FED60F98DB58880DF9C8234FD5A25D8112C5093C4FD1F1ABD82D2CB5EAD9B7083E312085C2727AF1FB97121AB6208173BEB5CC5F150FFF1421FB4BFD31EE60360462EE2E3155AA87E7DE66F6914592FF544845C3B682771EEBE2E4C42D12C466B56C8CBB0D7EABAD5337F4DC6DCF7562126BCF73431D74F97BC4C74EAE3B579E99BDC8C9C1F0BA78CE0C8D08269615C226DFB7EFCEC743709484241AD642A30D97C2A4DD2A43E167039674429D7A3F0BA5CE72B64FE3C49AA35"
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.snapDoc = function () {
    CADApplet.rid++ // 增加请求序号
    let msg = {
        "req": "Wrl_AppletSnap",
        "rid": CADApplet.rid,
        "para": {"ID": this.aid, "Base64": 0, "File": new Date().getTime() + ".jpg"}
    }
    this.ws.sendMessage(msg)
}

