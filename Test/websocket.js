(function (window, undefined) {
    $(function () {
        var socket, $win = $('body');
        showmessage = function (msg, type) {
            var datetime = new Date();
            var tiemstr = datetime.getHours() + ':' + datetime.getMinutes() + ':' + datetime.getSeconds() + '.' + datetime.getMilliseconds();
            if (type) {
                var $p = $('<div>').appendTo($win.find('#div_msg'));
                var $type = $('<span>').text('[' + tiemstr + ']' + type + '：').appendTo($p);
                var $msg = $('<span>').addClass('thumbnail').css({ 'margin-bottom': '5px' }).text(msg).appendTo($p);
            } else {
                var $center = $('<center>').text(msg + '(' + tiemstr + ')').css({ 'font-size': '12px' }).appendTo($win.find('#div_msg'));
            }
        };

        $win.find('#refresh_clearcache').click(function () {
            $.yszrefresh();
        });

        $win.find('#btn_conn').attr('disabled', false);
        $win.find('#btn_close').attr('disabled', true);

        $win.find('#btn_conn').click(function () {
            $win.find('#btn_conn').attr('disabled', true);
            $win.find('#btn_close').attr('disabled', false);
            var url = $win.find('#inp_url').val();
            // 创建一个Socket实例
            socket = new WebSocket(url);
            showmessage('开始连接');
            // 打开Socket 
            socket.onopen = function (event) {
                // 发送一个初始化消息
                showmessage('连接成功');
            };
            // 监听消息
            socket.onmessage = function (eve) {
                showmessage(eve.data, 'receive');
            };
            // 监听Socket的关闭
            socket.onclose = function (event) {
                showmessage('断开连接');
                $win.find('#btn_conn').attr('disabled', false);
                $win.find('#btn_close').attr('disabled', true);
            };
        });
        $win.find('#btn_close').click(function () {
            if (socket) {
                socket.close();
            }
        });
        $win.find('#btn_send').click(function () {
            var msg = $win.find('#inp_send').val();
            if (socket && msg) {
                socket.send(msg);
                showmessage(msg, 'send');
                $win.find('#inp_send').val('');
            }
        });
        $win.find('#inp_send').keyup(function () {
            if (event.ctrlKey && event.keyCode == 13) {
                $win.find('#btn_send').trigger('click');
            }
        });

        $win.find('#btn_clear').click(function () {
            $win.find('#div_msg').empty();
        }); 
    });
})(window);