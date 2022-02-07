function GetDefaultConn()
{
	if(document.location.href.toLowerCase().indexOf("https") == -1)
		return 'ws://127.0.0.1:83?sid=' + getrandom(5).toLocaleString() + '&flag=1';
	else
		return 'wss://wrl.zorrosoft.com:443?sid=' + getrandom(5).toLocaleString() + '&flag=1';
}

// 判断是否IE浏览器，用于区别使用Web Socket连接组件
function isIE()
{
	if (!!window.ActiveXObject || "ActiveXObject" in window)
		return true;
	else
		return false;
}

// 判断是否为Firefox，用于区别处理页面滚动和页面切换可见性
function isFirefox()
{
	if (navigator.userAgent.toLowerCase().indexOf("firefox") != -1)
		return true;
	else
		return false;
}

function hasVerticalScrollbar() 
{
	if(document.documentElement.clientHeight)
		return document.body.scrollHeight > document.documentElement.clientHeight;
   return document.body.scrollHeight > window.innerHeight;
}

function hasHorizontalScrollbar() 
{
	if(document.documentElement.clientWidth)
		return document.body.scrollWidth > document.documentElement.clientWidth;
    return document.body.scrollWidth > window.innerWidth;
}

function getScrollbarWidth() 
{
    var scrollDiv = document.createElement("div");
    scrollDiv.style.cssText = 'width: 99px; height: 99px; overflow: scroll; position: absolute; top: -9999px;';
    document.body.appendChild(scrollDiv);
    var scrollbarWidth = scrollDiv.offsetWidth - scrollDiv.clientWidth;
    document.body.removeChild(scrollDiv);
    return scrollbarWidth;
}

// 获取随机数
function getrandom(nums) 
{
    return ('000000'+ Math.floor(Math.random() * 999999)).slice(-6);
}

(function (window, undefined)
{
    $(function ()
	{
        //////////////////////	目前脚本只支持一个网页内启动一个小程序实例	/////////////////////////
        var $win = $('body');
		var PluginOKSocket;					// WebSocket主连接对象
		var appletsocket;					// WebSocket连接小程序对象
		var nRequstAppletID 	= 0;  		// 请求启动小程序的序号
		var nAppletRunID 		= 0;  		// 小程序启动后的实例序号
		var bIsSendScrollInfo 	= false;	// 是否已发送滚动条信息
		var bRunInCurrentPage	= true;		// 是否为当前页面加载的小程序
		var nScrollLeftOld 		= 0;		// 原横向滚动位置
		var nScrollTopOld 		= 0;		// 原纵向滚动位置

        showmessage = function (msg, type)
		{
            var datetime = new Date();
            var tiemstr = datetime.getHours() + ':' + datetime.getMinutes() + ':' + datetime.getSeconds() + '.' + datetime.getMilliseconds();
            if (type)
			{
                var $p = $('<div>').appendTo($('body').find('#div_msg'));
                var $type = $('<span>').text('[' + tiemstr + ']' + type + '：').appendTo($p);
                var $msg = $('<span>').addClass('thumbnail').css({ 'margin-bottom': '5px' }).text(msg).appendTo($p);
            }
			else
			{
                var $center = $('<center>').text(msg + '(' + tiemstr + ')').css({ 'font-size': '12px' }).appendTo($('body').find('#div_msg'));
            }
        };

		WrlVisibilityListener = function (AddEvent)
		{
			if(bRunInCurrentPage)
			{
				if(AddEvent)
                {
                    if(document.addEventListener)
					    document.addEventListener('visibilitychange',PageVisibilityState,false);
                    else
					    document.attachEvent('onvisibilitychange',PageVisibilityState,false);
				}
                else
                {
                    if(document.removeEventListener)
					    document.removeEventListener('visibilitychange',PageVisibilityState,false);
                    else
					    document.detachEvent('onvisibilitychange',PageVisibilityState,false);
                }
			}
		}
		
		WrlScrollListener = function (AddEvent)
		{
			if(!isFirefox())
			{
				if(AddEvent)
				{
					if(window.pageXOffset != undefined)
						document.onscroll = scrollFunc;
					else
						window.onscroll = scrollFunc;
				}
			}
			else
			{
				if(AddEvent)
					document.addEventListener("scroll",scrollFunc,false);
				else
					document.removeEventListener("scroll",scrollFunc);
			}
		}
	
		// 滚动内嵌小程序
		scrollFunc = function (e)
		{
			if(!nAppletRunID || !bRunInCurrentPage)
				return;
				if(!nAppletRunID || !bRunInCurrentPage)
				return;
			var nOffsetX = 0,nScrollLeft = 0;
			var nOffsetY = 0,nScrollTop = 0;
			if(window.pageXOffset != undefined)
				nScrollLeft = Math.round(window.pageXOffset);
			else
			{
				//获取页面的scrollLeft(兼容写法)
				nScrollLeft = Math.round(document.documentElement.scrollLeft || document.body.scrollLeft);
			}
			if(window.pageYOffset != undefined)
				nScrollTop = Math.round(window.pageYOffset);
			else
			{
				//获取页面的scrollTop(兼容写法)
				nScrollTop = Math.round(document.documentElement.scrollTop || document.body.scrollTop);
			}
			nOffsetX = nScrollLeftOld - nScrollLeft;
			nOffsetY = nScrollTopOld - nScrollTop;		
			nScrollLeftOld = nScrollLeft;
			nScrollTopOld = nScrollTop;
			if(nOffsetX || nOffsetY)
				WrlScrollApplet(nScrollLeft,nScrollTop,nOffsetX,nOffsetY);
		}
		
		function SendScrollInfo()
		{
			if(nAppletRunID)
			{
				var nScrollTop = 0,nScrollLeft = 0,BarCode = 0;
				var bHorizontalBar = hasHorizontalScrollbar();
				var bVerticalBar = hasVerticalScrollbar();
				if(bHorizontalBar)
				{
					if(window.pageXOffset != undefined)
						nScrollLeft = Math.round(window.pageXOffset);
					else
					{
						//获取页面的scrollLeft(兼容写法)
						nScrollLeft = Math.round(document.documentElement.scrollLeft || document.body.
						scrollLeft);
					}
					nScrollLeftOld = nScrollLeft;
				}
				if(bVerticalBar)
				{
					if(window.pageYOffset != undefined)
						nScrollTop = Math.round(window.pageYOffset);
					else
					{
						//获取页面的scrollTop(兼容写法)
						nScrollTop = Math.round(document.documentElement.scrollTop || document.body.scrollTop);
					}
					nScrollTopOld = nScrollTop;
				}
				var msg = '{"req":"Wrl_ScrollBar","rid":';
				msg += getrandom(5).toLocaleString();
				msg += ',"para":{"ID":';
				msg += nAppletRunID;
				msg += ',"BarH":';
				if(bHorizontalBar)
					BarCode = 1;
				msg += 0;			// 有水平滚动条，可设置预留底部高度
				msg += ',"BarW":';
				if(bVerticalBar)
					BarCode += 2;
				msg += 0; 			// 有垂直滚动条，可设置预留右侧宽度
				msg += ',"Code":';
				msg += BarCode; 	// 滚动条是否存在信息
				msg += ',"Top":';
				msg += nScrollTop;	// 垂直滚动位置
				msg += ',"Left":';	// 水平滚动位置
				msg += nScrollLeft;
				msg += '}}';
				if(!isIE())
				{
					PluginOKSocket.send(msg);
				}
				else
				{
					PluginOKSocket = document.getElementById("WrlWS");
					PluginOKSocket.send(msg);
				}
				bIsSendScrollInfo = true;
				console.log(msg);
			}
		}
		
		// 监听窗口大小改变事件，使滚动状况匹配
		$(window).resize(function () 
		{
            SendScrollInfo();
        });
		
		// 处理主连接接收到的JSON数据包
		DealRecMessage = function (Data)
		{
			var jsondata = $.parseJSON(Data);
			if(jsondata.rid == nRequstAppletID)
			{
				if(0 == jsondata.ret)
				{
					nAppletRunID = jsondata.data.ID;
					$win.find('#btn_max').attr('disabled', false);
					$win.find('#btn_fitpage').attr('disabled', false);					
				}
				else
				{
					if(undefined == jsondata.err)
						showmessage(Data, 'receive');
					else
						showmessage('小程序请求错误：' + jsondata.err);
				}
			}
			else
			{
				if(bRunInCurrentPage && jsondata.aid == nAppletRunID && jsondata.event == 'Wrl_Listen')
				{
					// 小程序启动成功，发送页面滚动信息
					SendScrollInfo();
				}
				if(jsondata.req == 'Wrl_AppletScroll'
					|| jsondata.req == 'Wrl_ScrollBar'
					|| jsondata.req == 'Wrl_AppletResize')
				{
					console.log(Data);
				}
				else
				{
					showmessage(Data, 'receive');
				}
			}

		}
		
		// 处理主连接发送的JSON数据包
		DealSendMessage = function (Data)
		{
			var jsondata = $.parseJSON(Data);
			if(jsondata.req == "Wrl_IEApplet"
				|| jsondata.req == "Wrl_IETab"
				|| jsondata.req == "Wrl_VLCApplet"
				|| jsondata.req == "Wrl_VLCWebPlayer"
				|| jsondata.req == "Wrl_HKWebPlayer"
				|| jsondata.req == "Wrl_DHCWebPlayer"
				|| jsondata.req == "Wrl_HWCWebPlayer"
				|| jsondata.req == "Wrl_FlashApplet"
				|| jsondata.req == "Wrl_OfficeApplet"
				|| jsondata.req == "Wrl_ACADApplet"
				|| jsondata.req == "Wrl_TXWebApplet"
				|| jsondata.req == "Wrl_PCLWebPlayer"
				|| jsondata.req == "Wrl_IEPage"
				|| jsondata.req == "Wrl_FlashWebPlayer"
				|| jsondata.req == "Wrl_SWCadApplet"
				|| jsondata.req == "Wrl_AppletStart")
			{
				nRequstAppletID = jsondata.rid;
				if(1 == (1 & jsondata.para.Flag) || 16 == (16 & jsondata.para.Flag))
					bRunInCurrentPage = false; // 新标签或新窗口加载小程序
			}
		}
		
		// 处理小程序连接接收的JSON数据包
		DealAppletRecMsg = function (Data)
		{
		}
		
		// 处理小程序连接发送的JSON数据包
		DealAppletSendMsg = function (Data)
		{
		}
		
		WrlScrollApplet = function (nScrollLeft,nScrollTop,nOffsetX,nOffsetY)
		{
			if(nAppletRunID)
			{
				var nCode = 0;
				if(nOffsetX && nOffsetY)
					nCode = 3; // 同时滚动
				else
				{
					if(nOffsetX)
						nCode = 1; // 横向滚动
					if(nOffsetY)
						nCode = 2; // 纵向滚动
				}
				var msg = '{"req":"Wrl_AppletScroll","rid":';
				msg += getrandom(5).toLocaleString();
				msg += ',"para":{"ID":';
				msg += nAppletRunID;
				msg += ',"Top":';
				msg += nScrollTop;
				msg += ',"Left":';
				msg += nScrollLeft;
				msg += ',"Code":';
				msg += nCode;		 // 滚动方向
				msg += ',"NoLog":1'; // 不输出日志
				msg += '}}';
				if(!isIE())
				{
					PluginOKSocket.send(msg);
				}
				else
				{
					PluginOKSocket = document.getElementById("WrlWS");
					PluginOKSocket.send(msg);
				}
				console.log(msg);
			}
		}

		PageVisibilityState = function ()
		{
			if(nAppletRunID < 1)
				return;// 未启动小程序
			// 控制小程序显示、仅对当前网页内加载的小程序
			var msg = '{"req":"Wrl_AppletControl","rid":';
			msg += getrandom(5).toLocaleString();
			msg += ',"para":{"ID":';
			msg += nAppletRunID;
			if (document.visibilityState == 'visible')
			{
				/// 恢复显示
				msg += ',"Code":';
				msg += 8;
			}
			else if (document.visibilityState == 'hidden')
			{
				/// 需要隐藏
				msg += ',"Code":';
				msg += 4;
			}
			msg += '}}';
			PluginOKSocket.send(msg);
			showmessage(msg, 'send');
		}

        $win.find('#btn_conn').attr('disabled', false);
        $win.find('#btn_send').attr('disabled', true);	
        $win.find('#btn_close').attr('disabled', true);
        $win.find('#btn_max').attr('disabled', true);
        $win.find('#btn_fitpage').attr('disabled', true);
		$win.find('#btn_restore').attr('disabled', true);

        $win.find('#btn_conn').click(function () 
		{
            $win.find('#btn_conn').attr('disabled', true);
            $win.find('#btn_close').attr('disabled', false);
            var url = $win.find('#inp_url').val();
			
			if(!isIE())
			{
				// 创建一个Socket实例
				PluginOKSocket = new WebSocket(url);
				showmessage('开始连接');
				
				// 打开Socket 
				PluginOKSocket.onopen = function (event) 
				{
					// 发送一个初始化消息
					showmessage(url + ' 连接成功');
					$win.find('#btn_send').attr('disabled', false);	
					WrlVisibilityListener(true);
					WrlScrollListener(true);
				};
				
				// 监听消息
				PluginOKSocket.onmessage = function (eve) 
				{
					DealRecMessage(eve.data);
				};
				
				// 监听Socket的关闭
				PluginOKSocket.onclose = function (event) 
				{
					WrlVisibilityListener(false);
					WrlScrollListener(false);
					nAppletRunID = 0;
					nRequstAppletID = 0;
					
					showmessage('连接已断开');
					$win.find('#btn_conn').attr('disabled', false);
					$win.find('#btn_send').attr('disabled', true);	
					$win.find('#btn_close').attr('disabled', true);
					$win.find('#btn_max').attr('disabled', true);
					$win.find('#btn_fitpage').attr('disabled', true);
					$win.find('#btn_restore').attr('disabled', true);
				};
			}
			else
			{
				 PluginOKSocket = document.getElementById("WrlWS");
				 if(PluginOKSocket)
				 {
					// 设置是否输出日志 
					PluginOKSocket.EnableLog = false;
					//PluginOKSocket.EnableLog = true;
					if(PluginOKSocket.ReadyState > 1)
					{
						// 还未连接
						PluginOKSocket.Connect(url);
					}
				 }
			}
        });
		
        $win.find('#btn_close').click(function ()
		{
			if(!isIE())
			{
				if (PluginOKSocket) 
				{
					PluginOKSocket.close();
				}
			}
			else
			{
				 PluginOKSocket = document.getElementById("WrlWS");
				 if(PluginOKSocket)
				 {
					PluginOKSocket.close();
				}
			}
        });
		
        $win.find('#btn_send').click(function ()
		{
            var msg = $win.find('#inp_send').val();
			if(!isIE())
			{
				if (PluginOKSocket && msg) 
				{
					$win.find('#inp_send').val('');
					PluginOKSocket.send(msg);
					DealSendMessage(msg);
					showmessage(msg, 'send');
				}
			}
			else
			{
				PluginOKSocket = document.getElementById("WrlWS");
				if (PluginOKSocket && msg) 
				{
					$win.find('#inp_send').val('');
					PluginOKSocket.send(msg);
					DealSendMessage(msg);
					showmessage(msg, 'send');
				}
			}
        });
		
        $win.find('#inp_send').keyup(function () 
		{
            if (event.ctrlKey && event.keyCode == 13) 
			{
                $win.find('#btn_send').trigger('click');
            }
        });

        $win.find('#btn_clear').click(function () 
		{
            $win.find('#div_msg').empty();
        });
		
		$win.find('#btn_max').click(function () 
		{
            if(nAppletRunID < 1)
				return;// 未启动小程序
			if(!bIsSendScrollInfo)
				SendScrollInfo();
			var msg;
			var W = $(window).width();
			var H = $(window).height();
			// 小程序显示到整个客户区
			msg = '{"req":"Wrl_AppletResize","rid":';
			msg += getrandom(5).toLocaleString();
			msg += ',"para":{"ID":';
			msg += nAppletRunID;
			msg += ',"X":8,"Y":310,"Width":';// 这里X和Y可分别实现视图区左侧和顶部预留局域
			msg += W;
			msg += ',"Height":';
			msg += H;
			msg += '}}';
			PluginOKSocket.send(msg);
			$win.find('#btn_restore').attr('disabled', false);	
			showmessage(msg, 'send');
        });

		$win.find('#btn_fitpage').click(function () 
		{
            if(nAppletRunID < 1)
				return;// 未启动小程序
			if(!bIsSendScrollInfo)
				SendScrollInfo();
			var msg;
			// 小程序自动适配网页大小显示切换
			msg = '{"req":"Wrl_AppletControl","rid":';
			msg += getrandom(5).toLocaleString();
			msg += ',"para":{"ID":';
			msg += nAppletRunID;
			msg += ',"Code":';
			msg += 16;
			msg += '}}';
			PluginOKSocket.send(msg);
			showmessage(msg, 'send');
        });

		$win.find('#btn_restore').click(function () 
		{
            if(nAppletRunID < 1)
				return;// 未启动小程序
			var msg;
			msg = '{"req":"Wrl_AppletResize","rid":';
			msg += getrandom(5).toLocaleString();
			msg += ',"para":{"ID":';
			msg += nAppletRunID;
			msg += ',"X":0,"Y":28,"Width":';
			msg += 480;
			msg += ',"Height":';
			msg += 320;
			msg += '}}';
			PluginOKSocket.send(msg);
			showmessage(msg, 'send');
        }); 		
    });
})(window);
