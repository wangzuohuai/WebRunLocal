class CADApplet{

    static aidArr = [
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
    ]  //维护一个数组 用来保存Applet实例的aid

    static rid = 1000 //每次请求都要传递一个rid 参数 这里弄成静态变量 每次累加一次就行
    openType = 0  // 加载类型 0默认打开软件在线编辑图纸 1 OCX看图
    IframeX = -10 // 模拟iFrameX坐标 默认-10 根据自己情况修改
    IframeY = 0  // 模拟iFrameY坐标 默认0 根据自己情况修改
    CADApplet(ws = null, aid = 0){
        this.ws = ws
        this.aid = aid
    }
    /**
     * 让rid和aid和ws都映射起来
     * @param rid
     * @param ws
     * @param type=1或者type=2 根据文档来选择例如操作的是宽度 边距等 需要使用第一个ws
     */
    static setAidArr(rid=0, aid=0,type=1,ws=null) {
        CADApplet.aidArr.forEach((item, index) => {
            if (item.rid == rid) {
                CADApplet.aidArr[index].aid = aid
                if(type==1){
                    CADApplet.aidArr[index].ws1 = ws
                }
                if(type==2){
                    CADApplet.aidArr[index].ws2 = ws
                }
                if(type==0){
                    CADApplet.aidArr[index].ws2 = null
                    CADApplet.aidArr[index].ws1 = null
                }
            }
        })
    }

    static getAidArr(rid) {
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
    setWebsocket(aid, type = 1) {
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
     * 启动AutoCAD网页组件
     * @param rid
     * @param left
     * @param top
     * @param width
     * @param height
     * @param OpenFile
     */
    startACAD(rid, left, top, width, height, OpenFile,edit) {
        //  启动第一个CAD网页组件
        //      Type为浏览器类型，传0自动判断(前提是当前浏览器已启动并显示在最前端，Flag指定当前页加载时必须是0) 可强制指定浏览器类型Type(2代表Chrome 4代表Firefox 8代表Opera 16代表Edge(Chromium内核) 20代表Electron 32代表360极速浏览器 33代表360安全浏览器 34代表360企业安全浏览器 50代表QQ浏览器 60代表搜狗浏览器)
        //      Title：网页标题中的关键词
        //      Flag掩码：1指定新标签加载(1和16都不指定时为当前页加载) 2显示标题栏 4不自动裁剪越界窗口 8自动适配网页高度和宽度显示 64启用Web参数 128防截屏 256强制显示到副屏 512允许同一网页加载多实例
        //      Option：对应变量openType的说明
        //      Open：为需要打开的文档路径，磁盘目录斜杠用/ 支持服务器HTTP协议的文件下载路径(请确保无需登录即可下载)，下载地址中如果没有包含文件名，请在Content-Disposition中指定filename
        //      此参数如非全路径，包括服务器路径，除非启动时指定了数据文件目录DataPath，否则默认使用中间件程序Data子目录作为根目录使用 也可在Web中指定
        //      注意：Open、Url中如果有特殊字符= & 双引号或中文等，需要用URL编码处理后传递
        //      BarW和BarH分别是网页右侧和底部预留区域，ScrollTop为顶部滚动预留高度
        //      Web节点中参数可自行配置，目前支持这些参数：
        // 		    DataPath代表文档查找和保存默认路径
 
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
                "Web": {"Edit": edit, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 0,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }

    StartDwgView(rid, left, top, width, height, OpenDwg) {
        //启动第二个CAD网页组件，参数参考startACAD
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
                "Web": {"User": "zorro", "Cookie": ""},
                "Open": OpenDwg,
                "Option": 1
            }
        }
        this.ws.sendMessage(msg)
    }
    
    StartSolidworksApplet(rid, left, top, width, height, OpenFile,edit) {
        // 启动一个Solidwork网页组件，参数参考startACAD，不同点是req名称及Open打开图纸的扩展名 Option设置1时代表启动eDrawings看图
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
                "Web": {"Edit": edit, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 0,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }
    
    StartCatiaApplet(rid, left, top, width, height, OpenFile,edit) {
        // 启动一个CATIA网页组件，参数参考startACAD，不同点是req名称及Open打开图纸的扩展名
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
                "Web": {"Edit": edit, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 0,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }
   
    StartProEApplet(rid, left, top, width, height, OpenFile,edit) {
        // 启动一个ProE网页组件，参数参考CADApplet-class.js中的startACAD Option设置1时代表启动Creo View看图 暂时不支持设置0
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
                "Web": {"Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 1,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }

    StartUGApplet(rid, left, top, width, height, OpenFile,edit) {
        // 启动一个ProE网页组件，参数参考CADApplet-class.js中的startACAD Option设置1时代表启动Creo View看图 暂时不支持设置0
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
                "Web": { "Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 1,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }

    StartZWCadApplet(rid, left, top, width, height, OpenFile,edit) {
        // 启动一个中望CAD网页组件，参数参考startACAD，不同点是req名称及Open打开图纸的扩展名
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
                "Web": {"Edit": edit, "User": "test", "Cookie": "", "DataPath": "c:/CadDoc"},
                "Option": 0,
                "Open": encodeURIComponent(OpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }

    saveFile() {
        // 请求保存图纸
        CADApplet.rid++ // 增加请求序号
        let msg = {
            "req": "CAD_Save",
            "rid": CADApplet.rid,
            "para": {}
        }
        this.ws.sendMessage(msg)
    }

    saveAsFile() {
        CADApplet.rid++ // 增加请求序号
        /// 先设置保存位置再另存
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": CADApplet.rid,
            "para": {
                "Type": 1,
                "Title": "请设置另存图纸位置",
                "Ext": "另存图纸(*.dwg;*.dxf)\r*.dwg;*.dxf"
            }
        }
        this.ws.sendMessage(msg)
    }

    ExportFile() {
        // 请求导出图纸
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

    printFile(url) {
        // 请求打印当前图纸
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

    appletFullEdit() {
        CADApplet.rid++ // 增加请求序号
        let msg = {
            "req": "CAD_SwitchFullScreen",
            "rid": CADApplet.rid,
            "para": {}
        }
        this.ws.sendMessage(msg)
    }

    closeApplet() {
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

    checkUpdate(version) {
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

    changeOpen(myOpenDoc) {
        //重新打开图纸
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

    resizeMargin(left, top, width, height) {
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

    resize(width, height) {
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

    showApp() {
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

    hideApp(code) {
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

    appScroll(scrollLeft = 0, scrollTop = 0) {
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

    sendScrollInfo(BarCode = 2, scrollLeft = 0, scrollTop = 0) {
        /// 设置页码滚动信息，BarW BarH分别为预留右侧宽度和底部高度
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
                    "Left": scrollLeft,
                    "Top": scrollTop
                }
            }
            this.ws.sendMessage(msg)    
        }
     }

    beginSaveAsFile(LocalFilePath) {
        // 请求开始另存图纸
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

    beginExpportFile(LocalFilePath) {
        // 请求开始另存图纸
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

    sendUpdateJson() {
        // 发送中间件的升级命令，实现自动升级，同时升级AutoCAD、CATIA、Solidworks、ProE等CAD网页组件
        // 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成
        CADApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_Update",
            "rid": CADApplet.rid,
            "para": {
                "Name": "PageHiCAD-图纸在线编辑组件升级包",
                "Date": "2025-01-03",
                "Desc": "1、中间件高级版兼容支持豆包桌面版，兼容法文系统，解决安装后系统服务可能无法正常启动问题，解决多线程下载可能卡住问题；2、增加西门子CAD图纸JS等格式在线查看网页组件支持...",
                "DownAddr": "http://local.zorrosoft.com/Files/Update/CAD_Update.pid",
                "Open": "http://local.zorrosoft.com/CADJS",
                "MD5": "4818243AECBCCECD2DFB55111B19AA7B",
                "Version": "2.2.16.11",
                "Size": 25985024,
                "HideIns": 0,
                "Cookie": "",
                "Auth": "",
                "TK": "4896A8DE2C967BB9C80C6E1541A64AD0B92F7C0F6A1F97FF0C37613ACEB6539A4221AB15A250C1C392AD1CE2694B43AE68AE2CA529A8DF4AC6AA21186841957E36D4E3D716E15A67EBAD1FF2ADBEB4225B29AE4CEBB31580567C87568C25E3ACBF18606E08FFF2186400743CA48893AEEF80FC59BA807C381CDCDBD557F0B054CFE75FF0762AF5536A935C44C523EFCD084CA8A3B0BAAB2BFDAF18C4B37241FBBEC2DECC9225738B1B716AEBCA3A5C26C28BB70BD622E5388ACEE2CF7652BF6167BFB94D6EA5DBC52D87A7F1B61BC80F12E5396EABDBC9257D75A4362957D51FD637D0353C4B67B5105CE3E6BA4FF3C9332C523817CF814E2B4C7908497B56F2"
            }
        }
        this.ws.sendMessage(msg)
    }

    snapDoc() {
        CADApplet.rid++ // 增加请求序号
        let msg = {"req": "Wrl_AppletSnap", "rid": CADApplet.rid, "para": {"ID": this.aid, "Base64":0, "File": new Date().getTime()+".jpg"}}
        this.ws.sendMessage(msg)
    }
}