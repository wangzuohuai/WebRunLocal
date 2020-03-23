(function (window, undefined) {
    $(function () {
        var $win = $('body');
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

            $WrlWS = document.getElementById("WrlWS");
			 if($WrlWS)
			 {
				 $WrlWS.EnableLog = true;
				if($WrlWS.ReadyState > 1)
				{
					// 还未连接
					$WrlWS.Connect(url);
				}
			 }
        });
        $win.find('#btn_close').click(function () {
            $WrlWS = document.getElementById("WrlWS");
			 if($WrlWS)
			 {
                $WrlWS.close();
            }
        });
        $win.find('#btn_send').click(function () {
            var msg = $win.find('#inp_send').val();
			$WrlWS = document.getElementById("WrlWS");
            if ($WrlWS && msg) {
                $WrlWS.send(msg);
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