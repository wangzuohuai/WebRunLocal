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
    {aid: 0, rid: 10, ws1: null, ws2: null},
    {aid: 0, rid: 11, ws1: null, ws2: null},
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
    // 启动一个AutoCAD网页组件，参数说明参考CADApplet-class.js中的startACAD
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
    // 启动第二个CAD网页组件，参数说明参考CADApplet-class.js中的startACAD
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
    // 启动一个Solidwork网页组件，参数说明参考CADApplet-class.js中的startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置0时代表启动Solidworks软件2014及以上版本 设置1时代表启动eDrawings看图 设置2时代表启动Composer Player
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
    // 启动一个CATIA网页组件，参数说明参考startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置0时代表启动Catia P3 V5R21软件
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
    // 启动一个ProE网页组件，参数说明参考CADApplet-class.js中的startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置1时代表启动Creo View看图 暂时不支持设置0启动ProE实现在线编辑
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
            "Web": { "Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 1,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartUGApplet = function (rid, left, top, width, height, OpenFile, edit) {
    // 启动一个UG网页组件，参数说明参考CADApplet-class.js中的startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置1时代表启动西门子的JT2GO 桌面程序或OCX控件看图 暂时不支持设置0启动UG软件
    let msg = {
        "req": "Wrl_UGApplet",
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
            "Web": {"Cookie": "", "DataPath": "c:/CadDoc"},
            "Option": 1,
            "Open": encodeURIComponent(OpenFile)
        }
    }
    this.ws.sendMessage(msg)
}

CADApplet.prototype.StartZWCadApplet = function (rid, left, top, width, height, OpenFile, edit) {
    // 启动一个中望CAD网页组件，参数说明参考CADApplet-class.js中的startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置0时代表启动中望CAD 2017及以上版本，暂时不支持3D软件
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
    // 先设置保存位置再另存
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
    // 先设置导出位置
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
    // 校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的网页组件，Wrl_Version功能多
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
            "Date": "2025-02-20",
            "Desc": "1、中间件高级版增加支持在夸克、遨游、猎豹及双核浏览器中的使用； 2、增强识别当前网页窗口信息接口，增强内嵌小程序启动参数可避免其加载到其它网页；3、优化高级版小程序在服务器版系统中的运行体验；4、CAD网页组件增加其控件或VBA接口的全面调用方式，不再局限于单独封装的接口；5、CAD网页组件ACAD增加支持2025版，中望CAD增加支持2017年开始的所有版本，Solidworks网页组件优化对EDrawing的支持，并新增Composer Player控件播放支持...",
            "DownAddr": "http://local.zorrosoft.com/Files/Update/CAD_Update.pid",
            "Open": "http://local.zorrosoft.com/CADJS",
            "MD5": "12624902683B9FC8B059678C6903C07B",
            "Version": "2.2.17.1",
            "Size": 27492352,
            "HideIns": 0,
            "Cookie": "",
            "Auth": "",
            "TK": "8D7E8921E6E5AFB60373F5A6A951607B6BFEDC942C180807625277669863347CBE6AE950BBE54AEAB1FF7FD2AB5A2A9ABB0C4225DFD08318E4F21D11337791BC579F90AAAE3F384D245C57EF5C347AB6A05B74DBEB3DB8CE382A3DEB1DD27EF8B32EEE3E6A7FDB3D19F7917F9D3FF5664B9066BBBF49B0702AA8DB5581FBDF63D20DE5718ABC3D5FA5B5319EE34CAF1DE305C688991A3F225BFD93E6EE6F028076E6DD17F815244E36E1A39F5FAD47B6419CDC3CF77B1FBE9048E4A78678B389504AD137199EFF9E74656492460AA8B160C6053E06585E939A8397452392244EDDDFC655B74C78374010F83DB01DC60F79E000670010EB8CCCD2710890276CE4"
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

