class officeApplet{

    static aidArr = [
        {aid: 0, rid: 2,ws1:null,ws2:null},
        {aid: 0, rid: 3,ws1:null,ws2:null},
        {aid: 0, rid: 4,ws1:null,ws2:null},
        {aid: 0, rid: 5,ws1:null,ws2:null},
    ]  //维护一个数组 用来保存Applet实例的aid
    static rid = 1000 //每次请求都要传递一个rid 参数 这里弄成静态变量 每次累加一次就行
    // 加载类型 可先通过请求Wrl_OfficeInfo获得当前需要设置的值
    // 1启动微软Word 2启动微软Excel 3启动微软PPT 5启动Adobe Pdf Reader或福昕OFD版式办公套件 8启动福昕OFD版式办公套件的OCX控件打开ofd
    // 11启动金山文字 12启动金山表格 13启动金山演示 15启动Adobe Pdf Reader或福昕OFD版式办公套件 16启动WPS PDF程序打开pdf(不支持OLE嵌入方式，必须用WPS专业版)   18启动福昕OFD版式办公套件的OCX控件打开ofd
    // 21启动永中文字 22启动永中表格 23启动永中简报 25启动Adobe Pdf Reader或福昕OFD版式办公套件  28启动福昕OFD版式办公套件的OCX控件打开ofd
    openType = 1  
    IframeX = -10 // 模拟iFrameX坐标 默认-10 根据自己情况修改
    IframeY = 0  // 模拟iFrameY坐标 默认0 根据自己情况修改
    officeApplet(ws = null, aid = 0){
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
        officeApplet.aidArr.forEach((item, index) => {
            if (item.rid == rid) {
                officeApplet.aidArr[index].aid = aid
                if(type==1){
                    officeApplet.aidArr[index].ws1 = ws
                }
                if(type==2){
                    officeApplet.aidArr[index].ws2 = ws
                }
                if(type==0){
                    officeApplet.aidArr[index].ws2 = null
                    officeApplet.aidArr[index].ws1 = null
                }
            }
        })
    }

    static getAidArr(rid) {
        for (let i in officeApplet.aidArr) {
            if (officeApplet.aidArr[i].rid == rid) {
                return officeApplet.aidArr[i].aid
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
        for (let i in officeApplet.aidArr) {
            if (officeApplet.aidArr[i].aid == aid) {
                if (type == 1) {
                    this.ws = officeApplet.aidArr[i].ws1
                } else {
                    this.ws = officeApplet.aidArr[i].ws2
                }
            }
        }
    }
    /**
     * 启动一个办公网页组件
     * @param rid
     * @param left
     * @param top
     * @param width
     * @param height
     * @param ServerOpenFile
     */
    startFirst(rid, left, top, width, height, ServerOpenFile,edit) {
        //  启动第一个办公网页组件
        //  Type为浏览器类型，传0自动判断(前提是当前浏览器已启动并显示在最前端，Flag指定当前页加载时必须是0) 可强制指定浏览器类型Type(1代表IE 2代表Chrome 4代表Firefox 5代表Brave(英文版) 8代表Opera 9代表Vivaldi(英文版) 10代表Yandex 16代表Edge(Chromium内核) 20代表Electron 32代表360极速浏览器 33代表360安全浏览器 34代表360极速浏览器 35代表360企业安全浏览器 36代表360游戏浏览器 37代表360AI浏览器 40代表联想浏览器 45代表双核浏览器 50代表QQ浏览器 51代表微信网页 55代表齐安信浏览器 57代表红莲花浏览器 60代表搜狗浏览器 61代表夸克 62代表遨游浏览器 63代表猎豹浏览器 66代表豆包 70代表华为浏览器)
        //  Type需要指定浏览器类型时，可先通过TestWrl.txt文档中的接口Wrl_BrowserInfo获取到当前浏览器的类型Type、浏览器主窗口句柄BrowserWnd、浏览器绘制窗口句柄DrawWnd及当前网页标题Title，前端可通过判断Title是否为当前网页需要内嵌决定是否继续启动，如果继续启动，把获取到的参数Type、BrowserWnd、DrawWnd设置到当前请求参数里，可以加快启动速度
        //  Title：网页标题中的关键词
        //  Flag掩码：1指定新标签加载(1和16都不指定时为当前页加载) 2显示标题栏 4不自动裁剪越界窗口 8自动适配网页高度和宽度显示 64启用Web参数 128防截屏 256强制显示到副屏 512允许同一网页加载多实例
        //  Version：0:OLE嵌入方式加载 1:完整加载(支持文档对比操作) 2:OLE嵌入方式加载(与0差别在于WPS专业版及微软Office高版本提前启动Office进程以支持多文档及加快启动性能) Option为5时Version自动默认为1
        //  Option：对应变量openType的说明
        //  Open：为需要打开的文档路径，磁盘目录斜杠用/ 支持服务器HTTP协议的文件下载路径(请确保无需登录即可下载)，下载地址中如果没有包含文件名，请在Content-Disposition中指定filename
        //  此参数如非全路径，包括服务器路径，除非启动时指定了数据文件目录DataPath，否则默认使用中间件程序Data子目录作为根目录使用 也可在Web中指定
        //  注意：Open、Url中如果有特殊字符= & 双引号或中文等，需要用URL编码处理后传递
        //  BarW和BarH分别是网页右侧和底部预留区域，ScrollTop为顶部滚动预留高度
        //  Web节点中参数可自行配置，目前支持这些参数：
        // 		Edit代表编辑权限(除只读1、512、1024都支持外，其它只有WORD才支持，4、8和1024只有在OLE嵌入方式下生效 其中1、2、16、32、64、128互斥) 1只读方式打开 2打开后自动处于修订模式 4禁止另存 8禁止打印 16禁止复制内容 32只能修订 64只能批注 128预览模式 256禁止粘贴内容出编辑窗口 512在PageHi菜单支持上传下载文档 1024安全文档(载入后和上传后自动删除本地文档) 2048禁用全屏编辑
        // 		User代表操作文档的用户名
        // 		PW代表打开文档需要的密码
        // 		DataPath代表文档查找和保存默认路径
        //      WaterMark 打开或新建文件后自动加水印节点信息，内容和接口Office_InsertWaterMark一致
 
        let msg = {
            "req": "Wrl_OfficeApplet",
            "rid": rid,
            "para": {
                "Type": "0",
                "Title": "办公网页组件(Word)",
                "Version": 2,//0使用OLE嵌入不先启动OLE Server 1是完整嵌入(有文件菜单和文档对比等更多功能可用) 2使用OLE嵌入，预先启动OLE Server，加快启动速度 尽可能支持多文档操作
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
                "Web": {"Edit": edit, "User": "test", "Cookie": "", "DataPath": "c:/OfficeDoc"},
                "Option": this.openType,
                "Open": encodeURIComponent(ServerOpenFile)
            }
        }
        this.ws.sendMessage(msg)
    }

    startSecond(rid, left, top, width, height, ServerOpenFile,edit) {
        //启动第二个办公网页组件，参数说明请参考startFirst
        let msg = {
            "req": "Wrl_OfficeApplet",
            "rid": rid,
            "para": {
                "Type": "0",
                "Title": "办公网页组件(Excel)",
                "Version": 2,
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
                "Web": {"Edit": edit,  "User": "zorro", "Cookie": ""},
                "Open": ServerOpenFile,
                "Option": this.openType
            }
        }
        this.ws.sendMessage(msg)
    }

    addMark() {
        // 请求插入书签
        officeApplet.rid++
        let msg = {
            "req": "Office_InsertMark",
            "rid": officeApplet.rid,
            "para": {"Name": "TestMark", "Content": "[InsertMark]", "Hidden": 0, "Sort": 0}
        }
        this.ws.sendMessage(msg)
    }

    markRePlace() {
        // 请求替换书签内容
        officeApplet.rid++// 增加请求序号
        let msg = {
            "req": "Office_PutMarkText",
            "rid": officeApplet.rid,
            "para": {
                "Marks": [{"Name": "Caption", "Text": "公文标题"}, {
                    "Name": "TestMark",
                    "Text": "TestMarkContent"
                }]
            }
        }
        this.ws.sendMessage(msg)
    }

    convertFirstPage() {
        // 转换文档中序号为Index页码内容成图片，先设置转换图片存放位置
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": officeApplet.rid++,
            "para": {
                "Type": 1,
                "Title": "请设置图片保存位置",
                "Ext": "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
            }
        }
        this.ws.sendMessage(msg)
    }

    insertContent() {
        // 请求插入指定的文字或链接
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_Insert",
            "rid": officeApplet.rid,
            "para": [{"Paragraph": 1},
                {"FontSize": "16", "FontName": "宋体", "Color": "0", "Bold": "8", "Text": "详情请访问成都佐罗软件官网："},
                {"Paragraph": 1},
                {"Address": "http://www.zorrosoft.com", "Display": "成都佐罗软件官网"}]
        }
        this.ws.sendMessage(msg)
    }

    saveFile() {
        // 请求保存文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_Save",
            "rid": officeApplet.rid,
            "para": {}
        }
        this.ws.sendMessage(msg)
    }

    saveAsFile() {
        officeApplet.rid++ // 增加请求序号
        /// 先设置保存位置再另存
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": officeApplet.rid,
            "para": {
                "Type": 1,
                "Title": "请设置另存文件位置",
                "Ext": "另存文件(*.doc;*.docx;*.pdf;*.html)\r*.doc;*.docx;*.pdf;*.html"
            }
        }
        this.ws.sendMessage(msg)
    }

    exportFile() {
        // 请求导出文档
        officeApplet.rid++ // 增加请求序号
        /// 先设置导出位置
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": officeApplet.rid,
            "para": {
                "Type": 1,
                "Title": "请设置导出文件位置",
                "Ext": "导出文件(*.pdf)\r*.pdf"
            }
        }
        this.ws.sendMessage(msg)
    }

    insertImg() {
        // 请求当前光标位置插入图片 先让用户选择图片文件
        officeApplet.rid++ // 增加请求序号
        /// 先选择本地图片文件
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": officeApplet.rid,
            "para": {
                "Type": 0,
                "Title": "请选择需要插入的图片",
                "Ext": "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
            }
        }
        this.ws.sendMessage(msg)
    }

    getFirstImg() {
        // 提取文档中页码序号Index的图片，先设置图片保存位置
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_SelectFile",
            "rid": officeApplet.rid,
            "para": {
                "Type": 1,
                "Title": "请设置图片保存位置",
                "Ext": "图片文件(*.jpg;*.png;*.gif;*.bmp;*.tiff)\r*.jpg;*.png;*.gif;*.bmp;*.tiff"
            }
        }
        this.ws.sendMessage(msg)
    }

    printFile(url) {
        // 请求打印当前文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_PrintOut",
            "rid": officeApplet.rid,
            "para": {
                "ImgFile": url,
                "Copies": 1
            }
        }
        this.ws.sendMessage(msg)
    }

    disableRevision() {
        // 关闭留痕，就是关闭修订模式
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_PutTrackRevisions",
            "rid": officeApplet.rid,
            "para": {
                "TrackRevisions": 0
            }
        }
        this.ws.sendMessage(msg)
    }

    enableRevision() {
        // 请求留痕，就是修订模式
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_PutTrackRevisions",
            "rid": officeApplet.rid,
            "para": {
                "TrackRevisions": 1
            }
        }
        this.ws.sendMessage(msg)
    }

    showRevision() {
        // 显示留痕信息，就是显示修订内容
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_PutShowRevisions",
            "rid": officeApplet.rid,
            "para": {
                "ShowRevisions": 1
            }
        }
        this.ws.sendMessage(msg)
    }

    acceptRevision() {
        // 接受留痕，就是接受修订内容
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_TrackRevisions",
            "rid": officeApplet.rid,
            "para": {
                "Type": 1
            }
        }
        this.ws.sendMessage(msg)
    }

    appletFullEdit() {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_SwitchFullScreen",
            "rid": officeApplet.rid,
            "para": {}
        }
        this.ws.sendMessage(msg)
    }

    closeApplet() {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletControl",
            "rid": officeApplet.rid,
            "para": {
                "ID": this.aid,
                "Code": 1
            }
        }
        this.ws.sendMessage(msg)
    }

    checkUpdate(version) {
        //校验中间件版本是不是需要升级,如果额外指定PID参数，代表校验PID代表的网页组件，Wrl_Version功能多
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_Version",
            "rid": officeApplet.rid,
            "para": {
                "Version": version
            }
        }
        this.ws.sendMessage(msg)
    }

    changeOpen(myOpenDoc) {
        //重新打开文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_Open",
            "rid": officeApplet.rid,
            "para": {
                "Open": encodeURIComponent(myOpenDoc)
            }
        }
        this.ws.sendMessage(msg)
    }

    resizeMargin(left, top, width, height) {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletResize",
            "rid": officeApplet.rid,
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
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletResize",
            "rid": officeApplet.rid,
            "para": {
                "ID": this.aid,
                "Width": width,
                "Height": height
            }
        }
        this.ws.sendMessage(msg)
    }

    showApp() {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletControl",
            "rid": officeApplet.rid,
            "para": {
                "ID": this.aid,
                "Code": 8
            }
        }
        this.ws.sendMessage(msg)
    }

    hideApp(code) {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletControl",
            "rid": officeApplet.rid,
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
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_AppletScroll",
            "rid": officeApplet.rid,
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
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_ScrollBar",
            "rid": officeApplet.rid,
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

    beginSaveAsFile(LocalFilePath) {
        // 请求开始另存文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_SaveAs",
            "rid": officeApplet.rid,
            "para": {
                "NewFile": encodeURIComponent(LocalFilePath)
            }
        }
        this.ws.sendMessage(msg)
    }

    beginConvertFirstPage(LocalFilePath) {
        // 转换文档中页码序号Index的Base64编码数据，如指定本地保存文件名File，则保存到本地文件中
        officeApplet.rid++ // 增加请求序号
        let msg = {}
        if (LocalFilePath.length) {
            msg = {
                "req": "Office_ConvertImage",
                "rid": officeApplet.rid,
                "para": {
                    "File": encodeURIComponent(LocalFilePath),
                    "Index": 1
                }
            }
        } else {
            msg = {
                "req": "Office_ConvertImage",
                "rid": officeApplet.rid,
                "para": {
                    "Index": 1
                }
            }
        }
        this.ws.sendMessage(msg)
    }

    beginExpportFile(LocalFilePath) {
        // 请求开始另存文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_Export",
            "rid": officeApplet.rid,
            "para": {
                "NewFile": encodeURIComponent(LocalFilePath)
            }
        }
        this.ws.sendMessage(msg)
    }

    beginInsertImg(LocalFilePath) {
        // Save为1时自动保存文档
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_InsertImg",
            "rid": officeApplet.rid,
            "para": {
                "ImgFile": encodeURIComponent(LocalFilePath),
                "Order": 5,	// 插入后图片在文字之上
                "Save": 1	// 插入后自动保存文档
            }
        }
        this.ws.sendMessage(msg)
    }

    beginGetFirstImg(LocalFilePath) {
        // 提取文档中序号Index的Base64编码数据，如指定本地保存文件名File，则保存到本地文件中
        officeApplet.rid++ // 增加请求序号
        let msg = {}
        if (LocalFilePath.length) {
            msg = {
                "req": "Office_GetImage",
                "rid": officeApplet.rid,
                "para": {
                    "File": encodeURIComponent(LocalFilePath),
                    "Index": 1
                }
            }
        } else {
            msg = {
                "req": "Office_GetImage",
                "rid": officeApplet.rid,
                "para": {
                    "Index": 1
                }
            }
        }
        this.ws.sendMessage(msg)
    }

    sendUpdateJson() {
        // 发送中间件的升级命令，实现自动升级，同时升级微软及金山办公等网页组件
        // 注意：Wrl_Update中的请求参数如MD5 TK Size等，请根据文档“中间件制作升级包说明.pdf”中的打包工具生成
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Wrl_Update",
            "rid": officeApplet.rid,
            "para": {
                "Name": "PageHiOffice—文档在线编辑组件升级包",
                "Date": "2025-02-20",
                "Desc": "1、中间件高级版增加支持在夸克、遨游、猎豹及双核浏览器中的使用； 2、增强识别当前网页窗口信息接口，增强内嵌小程序启动参数可避免其加载到其它网页；3、优化高级版小程序在服务器版系统中的运行体验；4、Office网页组件增加其控件或VBA接口的全面调用方式，不再局限于单独封装的接口；5、PageHiOffice网页组件增加OFD文档打开支持，解决在Win7等低版本系统直接关闭浏览器时可能弹崩溃问题，解决启动多个实例时设置的编辑权限不一样可能相互影响问题...",
                "DownAddr": "http://local.zorrosoft.com/Files/Update/Office_Update.pid",
                "Open": "http://local.zorrosoft.com/officeJS",
                "MD5": "BF2E94FFEE99430864C54C61C152F60D",
                "Version": "2.2.17.1",
                "Size": 36667392,
                "HideIns": 0,
                "Cookie": "",
                "Auth": "",
                "TK": "7E91C6041EBEC3C9C8A45B9D0BC1267211592329F64EF35C1FEF300E083377A3D1427817032667CBA9F6366CDF04B71C2BDEA146DCAB3382E02EEF2C5270DA1F45646D826759ACD056511A71DD2B389C57C8E3B2A9DBE89F84D3840190E5B6B14428B5840A140AD72FAECB2EACA8F93105C07919DF6565E84A885047DFC82B04CE4DE19D614CDC115CD9D3749DDE35AE3FF0F64216DDC950D0F09330A81D2F9B8B95524B201C039DE61DC41CB8A1095EC61CDD71CC0E8245EC4F1EC2EBA7E4A53C8FF87E88967F51902AACE3224CA9F7EB357FD884D0E2B0F1FD8A2FE0EA14AACAC4576D63F0C1937BF66BB49FF0A69F4AA6F62A0A14AFE4943874AF6865F814"
            }
        }
        this.ws.sendMessage(msg)
    }

    compareDoc(LocalFilePath) {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_DocCompare",
            "rid": officeApplet.rid,
            "para": {"Name": encodeURIComponent(LocalFilePath), "Target": "2", "Out": "C:/OfficeDoc/Result.doc"}
        }
        this.ws.sendMessage(msg)
    }

    mergeDoc(LocalFilePath) {
        officeApplet.rid++ // 增加请求序号
        let msg = {
            "req": "Office_FileMerge",
            "rid": officeApplet.rid,
            "para": {"Files": [encodeURIComponent(LocalFilePath)]}
        }
        this.ws.sendMessage(msg)
    }

    snapDoc() {
        officeApplet.rid++ // 增加请求序号
        let msg = {"req": "Wrl_AppletSnap", "rid": officeApplet.rid, "para": {"ID": this.aid, "Base64":0, "File": new Date().getTime()+".jpg"}}
        this.ws.sendMessage(msg)
    }
    insertTable(){
        officeApplet.rid++ // 增加请求序号
        let msg = {"req":"Office_InsertTable","rid":officeApplet.rid,"para":{"RowNum":2,"ColumnNum":5}}
        this.ws.sendMessage(msg)
    }
    insertMark(url){
        //Type 水印类型 Type默认0系统文字水印 1自定义文字水印 2图片水印
       // Content 类型为0时系统文字水印名称 类型为1时文字水印的内容 类型为2时图片文件路径或服务器水印图片下载地址 如有非英文、特殊字符或符号等，需要做UrlEncode编码
        officeApplet.rid++ // 增加请求序号
        //let msg = {"req":"Office_InsertWaterMark","rid":officeApplet.rid,"para":{"Type":0,"Content":"%E6%9C%BA%E5%AF%86%201"}}
         let msg = {"req":"Office_InsertWaterMark","rid":officeApplet.rid,"para":{"Type":2,"Content":url,"Width":15,"Height":18}}
        this.ws.sendMessage(msg)
    }
    insertNote(){
        officeApplet.rid++ // 增加请求序号
        let msg = {"req":"Office_InsertComment","rid":officeApplet.rid,"para":{"Text":"批注内容"}}
        this.ws.sendMessage(msg)
    }
    DocRed(){
        officeApplet.rid++ // 增加请求序号
        let msg = {"req":"Office_RedTemplate","rid":officeApplet.rid,"para":{"Position":"Content","File":"http://local.zorrosoft.com/Files/template.doc"}}
        this.ws.sendMessage(msg)
    }
}