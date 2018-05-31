$(document).ready(function () {


	//Преобразование RGB888 <-> RGB565
	//TODO пока не нужно, но оставим
	/*function hexToRgb(hex) {
		let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		return result ? {
			r: parseInt(result[1], 16),
			g: parseInt(result[2], 16),
			b: parseInt(result[3], 16)
		} : null;
	}
	function numToHex(c) {
		console.log(c);
		let hex = c.toString(16);
		return hex.length === 1 ? "0" + hex : hex;
	}
	function rgb3to2(rgb3) {
		let rgb = hexToRgb(rgb3);

		let r = (0x1f * rgb.r / 0xff) & 0x1f;
		let g = (0x3f * rgb.g / 0xff) & 0x3f;
		let b = (0x1f * rgb.b / 0xff) & 0x1f;

		let result = b & 0x1f;
		result += ((g & 0x3f) << 5);
		result += (r & 0x1f) << 11;

		return (numToHex((result >> 8) & 0xff) + numToHex(result & 0xff)).toUpperCase();
	}
	function rgb2to3(rgb2) {
		let color = parseInt(rgb2, 16);
		let r = ((color >> 11) & 0x1F)*0xff/0x1F;
		let g = ((color >> 5) & 0x3F)*0xff/0x3F;
		let b = ((color) & 0x1F)*0xff/0x1F;

		r = Math.round(r);
		g = Math.round(g);
		b = Math.round(b);

		return (numToHex(r) + numToHex(g) + numToHex(b)).toUpperCase();
	}
	console.log("C7EE",rgb3to2("CCFF75"));
	console.log("CCFF75",rgb2to3("C7EE"));
	*/


	//Преобразование цвета hex <-> десятичные
	function convertBase (num) {
		return {
			from : function (baseFrom) {
				return {
					to : function (baseTo) {
						return parseInt(num, baseFrom).toString(baseTo);
					}
				};
			}
		};
	}
	// decimal to hex
	function dec2hex (num) {
		return convertBase(num).from(10).to(16).toUpperCase();
	}
	// hex to decimal
	function hex2dec (num) {
		return convertBase(num).from(16).to(10);
	}
	console.log("13434741",dec2hex("13434741"));
	console.log("CCFF75",hex2dec("CCFF75"));

	//Свайп вкладок
	$(function() {
		function widthOfList () {
			let itemsWidth = 0;
            let itemWidth = 0;
			$('.list-tab li').each(function () {
				itemWidth = $(this).outerWidth();
				itemsWidth += itemWidth;
			});
			return itemsWidth;
		}

		function widthOfHidden () {
			//console.log($('.wrapper-nav').outerWidth(), widthOfList(), getLeftPos());
			//console.log($('.wrapper-nav').outerWidth() - widthOfList() - getLeftPos());
			//TODO ошибка в 2 пикселя !!! хрень какая-то, найти
			return ($('.wrapper-nav').outerWidth() - widthOfList() - getLeftPos() + 2);
		}

		function getLeftPos () {
			return $('.list-tab').position().left;
		}
		//TODO нужно доделать свайп, чтобы двигал правильно «ярлыки вкладок»
		$(".list-tab-content").swipe( {
			swipeStatus:function(event, phase, direction, distance, fingerCount) {
				if ( phase === "move" || phase === "start" ) {
                    let $target = event.target.nodeName;
					if( $target.toLowerCase() === 'input' || $target.toLowerCase() === 'select' || $target.toLowerCase() === 'button' ) {
						return false;
					}else{
						$('input,select,button').blur();
					}
				}
			},
			swipeLeft:function(event, distance, duration, fingerCount, fingerData, currentDirection) {
                let listWidth = widthOfList();
                let hiddenWidth = widthOfHidden();
                let containerWidth = $('.wrapper-nav').outerWidth();
                let $tab = $('.list-tab .active').next();


				if ($tab.length > 0) {
					let posTab = $tab.offset();
					let posList = $('.list-tab').offset();
					//console.log($tab,posTab);
					let leftTab = posTab.left-10;


					$tab.find('a').tab('show');
					//console.log(hiddenWidth, listWidth, leftTab, containerWidth, (listWidth-Math.abs(posList.left)));
					if((listWidth-Math.abs(posList.left))>containerWidth)
					//if(hiddenWidth < listWidth)
						$('.list-tab').animate({left:"-="+leftTab+"px"},'slow');
					//$('.list-tab').animate({left:"+="+widthOfHidden()+"px"},'slow');
				}
			},
			swipeRight:function(event, distance, duration, fingerCount, fingerData, currentDirection) {
                let listWidth = widthOfList();
                let hiddenWidth = widthOfHidden();
				let containerWidth = $('.wrapper-nav').outerWidth();
                let $tab = $('.list-tab .active').prev();
				if ($tab.length > 0) {
					let posTab = $tab.offset();
					let posList = $('.list-tab').offset();
					let leftTab = posTab.left-10;
					$tab.find('a').tab('show');
					//console.log(hiddenWidth, listWidth, leftTab, containerWidth, (listWidth-Math.abs(posList.left)));
					if(leftTab < 0)
					//if(hiddenWidth < listWidth)
						$('.list-tab').animate({left:"-="+leftTab+"px"},'slow');
					//$('.list-tab').animate({left:"-="+getLeftPos()+"px"},'slow');
				}
			},
			allowPageScroll:"auto"
		});
	});
	//Скрытие навбара после клика
	$(document).on('click','.navbar-collapse.in',function(e) {
		if( $(e.target).is('a') && ( $(e.target).attr('class') !== 'dropdown-toggle' ) ) {
			$(this).collapse('hide');
		}
	});

	//Кнопки + и -
	$(document).on('click','.minus',function(e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		let step = $input.attr("step");
		let min = $input.attr("min");
		let fixed = 0;
		const f = x => ( (x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0) );
		if (typeof min === typeof undefined && min === false)
			min = 0;
		if (typeof step !== typeof undefined && step !== false){
			step = parseFloat(step);
			fixed = f(step);
		}else{
			step = 1;
		}
		let count = parseFloat($input.val()) - step;
		count = count < min ? min : count;
		count =  parseFloat(count).toFixed(fixed);
		console.log(count,step);
		$input.val(count);
		$input.change();
		//return false;
	});
	$(document).on('click','.plus',function(e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		let step = $input.attr("step");
		let max = $input.attr("max");
		let fixed = 0;
		const f = x => ( (x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0) );
		if (typeof max === typeof undefined && max === false)
			max = 100;
		if (typeof step !== typeof undefined && step !== false){
			step = parseFloat(step);
			fixed = f(step);
		}else{
			step = 1;
		}
		let count = parseFloat($input.val()) + step;
		count = count > max ? max : count;
		count =  parseFloat(count).toFixed(fixed);
		console.log(count,step);
		$input.val(count);//
		$input.change();
		//return false;
	});

	//загружаем контент во вкладки
	$('li.swipe-tab a').on('show.bs.tab', function (e) {
		let url = $(this).attr("href");
		let target = $(this).data("target");
		let tab = $(this);
		if($.trim($(target).html())==='') {
			$(target).ajaxLoading();
			$(target).load(url, function (result) {
				$(target).ajaxLoading('stop');
				tab.tab('show');
			});
		}
	});
	$('li.swipe-tab a:first').tab('show');

	function alertAjaxError(err,exception,container){
		//console.log(err,exception);
		let msg = '';

		if (err.status === 0) {
			msg = 'Not connect.\n Verify Network.';
		} else if (err.status === 404) {
			msg = 'Requested page not found. [404]';
		} else if (err.status === 500) {
			msg = 'Internal Server Error [500].';
		} else if (exception === 'parsererror') {
			msg = 'Requested JSON parse failed.';
		} else if (exception === 'timeout') {
			msg = 'Time out error.';
		} else if (exception === 'abort') {
			msg = 'Ajax request aborted.';
		} else {
			msg = 'Uncaught Error.\n' + err.responseText;
		}
		let error_alert = '<div class="alert alert-danger alert-dismissible fade in mt-10" role="alert">' +
			'<button type="button" class="close" data-dismiss="alert" aria-label="Close">' +
			'<span aria-hidden="true">&times;</span>' +
			'</button>' +
			'<strong>'+msg+'</strong>' +
			'</div>';
		container.html(error_alert);
		//alert(msg);
	}

	//Свойства
	function getSettings() {
	    $.ajax({
	        url: 'configs.json',
	        data: {},
	        type: 'GET',
	        dataType: 'json',
	        success: function(msg) {
	            console.log('Settings', msg);
	            $("#settings_ssdp").val(msg["SSDP"]);
	            $("#settings_ssid").val(msg["ssid"]);
	            $("#settings_password").val(msg["password"]);
	            $("#settings_ssidap").val(msg["ssidAP"]);
	            $("#settings_passwordap").val(msg["passwordAP"]);
	            $("#settings_timezone").val(msg["timezone"]);


                //sensorsConditions.push({
                //        temperatures: msg["temperatures"]
                //    }
                //);

                // setTimeout(getDistillation, 2000);
			    // setTimeout(getReflux, 2000);
	        }
        })/*.done()*/;

	}

	/**
	 * <b>Отправка данных на сервер</b>
	 * @param {string} url - адрес
	 * @param {object} data - данные
	 * @param {string} dataType - тип передаваемых данных "text","json","html"...
	 * @param {object|boolean} success_action - действия после успешного отправления данных
	 * @param {object|boolean} load_target - елемент «отправитель» (для лоадера)
	 * @param {object|boolean} error_target - контейнер для вывода ошибок
	 */
	function sendRequest(url,data,dataType,success_action,load_target,error_target) {
		//console.log(url,data,target);
		$.ajax({
			url: url,
			data: data,
			type: 'GET',
			dataType: dataType,
			beforeSend: function(){
				if(load_target !== false)
					load_target.ajaxLoading({disabled:true});
			},
			success: function (msg) {
				//TODO реализовать success_action
			},
			error:function (err,exception) {
				if(error_target !== false)
					alertAjaxError(err,exception,error_target);
			},
			complete:function () {
				if(load_target !== false)
					load_target.ajaxLoading('stop');
			}
		});
	}

	//Определение датчиков
	function getSensors() {

	}





    //Обновление прошивки
	$("#file_update").on("change",function (e) {
		let vidFileLength = $(this)[0].files.length;
		if(vidFileLength !== 0) {
			$("#settings_update").prop("disabled",false);
		}
	});
	$("form#firmware_update").submit(function(e){
		e.preventDefault();
		let formData = new FormData();
		formData.append('update', $('#file_update')[0].files[0]);
		//console.log(formData,$('#file_update')[0].files[0]);
		$.ajax({
			url: 'update',
			type: 'POST',
			data: formData,
			//async: false,
			cache: false,
			contentType: false,
			enctype: 'multipart/form-data',
			processData: false,
			beforeSend: function () {
				$("#settings_update").ajaxLoading({disabled: true});
			},
			success: function (msg) {
				console.log(msg);
			},
			error: function (err, exception) {
				alertAjaxError(err, exception, $("#error_settings"));
			},
			complete: function () {
				$("#settings_update").ajaxLoading('stop').prop("disabled",false);
			}
		});
		//return false;
	});

	$(document).on("focus","#settings_password, #settings_passwordap",function () {
		$(this).prop("type","text");
	});
	$(document).on("blur","#settings_password, #settings_passwordap",function () {
		$(this).prop("type","password");
	});
	$("#settings_set_ssdp").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssdp = $("#settings_ssdp").val();
		sendRequest("ssdp",{"ssdp":ssdp},"text",false,_this,$("#error_settings"));
	});
	$("#settings_set_ssid").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssid = $("#settings_ssid").val();
		let pass = $("#settings_password").val();
		sendRequest("ssid",{"ssid":ssid,"password":pass},"text",false,_this,$("#error_settings"));
		//TODO сделать действия SUCCESS в sendRequest
		$.fn.openModal('', 'Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.', "modal-sm", false, true);
	});
	$("#settings_set_ssidap").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssidap = $("#settings_ssidap").val();
		let pass = $("#settings_passwordap").val();
		sendRequest("ssidap",{"ssidAP":ssidap,"passwordAP":pass},"text",false,_this,$("#error_settings"));
		//TODO сделать действия SUCCESS в sendRequest
		$.fn.openModal('', 'Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.', "modal-sm", false, true);
	});
	$("#settings_auto_timezone").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let date = new Date();
		let timezone = Math.abs(date.getTimezoneOffset()/60);
		$("#settings_timezone").val(timezone);
		sendRequest("TimeZone",{"timezone":timezone},"text",false,_this,$("#error_settings"));
	});
	$("#settings_set_timezone").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let timezone = $("#settings_timezone").val();
		sendRequest("TimeZone",{"timezone":timezone},"text",false,_this,$("#error_settings"));
	});
	$("#settings_restart").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		$.fn.openModal('', '<h4 class="text-danger">Вы действительно хотите перезагрузить устройство?</h4>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				sendRequest("restart",{"device":"ok"},"text",false,_this,false);
			}
		},
			{
				text: "Нет",
				id: "return_tab",
				class: "btn btn-danger btn-sm",
				click: function () {
					$(this).closest(".modal").modal("hide");
				}
			}], {buttons: "replace"});
	});


	//Дистиляция
	function getDistillation() {
		$.ajax({
			url: 'distillation.json',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				console.log('Distillation',msg);
				$("#distillation_t_1").text(msg["temperature"]);
				$("#distillation_t_2").text(msg["temperature2"]);
				$("#distillation_t_3").text(msg["temperature3"]);
				$("#distillation_t_4").text(msg["temperature4"]);
				$("#distillation_get_setting").text(msg["settingTank"].toFixed(2));
				if(msg["settingAlarmDistillation"] === false){
					$("#distillation_status").removeClass("success").addClass("danger");
				}else{
					$("#distillation_status").removeClass("danger").addClass("success");
				}
			}
		});
	}
	$("#distillation_set_setting").on("click",function () {
		let setting = Number($("#distillation_setting").val());

		//TODO сделать действия SUCCESS в sendRequest
		$("#distillation_get_setting").text(setting);
		sendRequest("SetTempTank",{"SettingTank":setting},"text",false,$(this),$("#error_distillation"));
	});

	//Ректификация
	function getReflux() {
		$.ajax({
			url: 'reflux.json',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				console.log('Reflux',msg);
				$("#reflux_t_1").text(msg["temperature"].toFixed(2));
				$("#reflux_t_2").text(msg["temperature2"].toFixed(2));
				$("#reflux_t_3").text(msg["temperature3"].toFixed(2));
				$("#reflux_t_4").text(msg["temperature4"].toFixed(2));
				$("#reflux_setting").text(msg["setting"].toFixed(2));
				$("#reflux_pressure").text(msg["pressure"].toFixed(2));
				$("#reflux_alco_boil").text(msg["temperatureAlcoholBoil"].toFixed(2));
				if(msg["settingAlarm"] === false){
					$("#reflux_status").removeClass("success").addClass("danger");
				}else{
					$("#reflux_status").removeClass("danger").addClass("success");
				}
				setTimeout(getBrewing, 2000);
			}
		});
	}
	$("#reflux_set_delta").on("click",function () {
		let delta = Number($("#reflux_delta").val());

		let temp_2 = Number($("#reflux_t_2").text());
		let temp_alco_boil = Number($("#reflux_alco_boil").text());

		let setting = temp_2 + delta;
		let data_send = {
			delta:delta,
			setting:setting,
			temperatureAlcoholBoil:temp_alco_boil
		};

		//TODO сделать действия SUCCESS в sendRequest
		$("#reflux_setting").text(setting);
		sendRequest("SetTemp",data_send,"text",false,$(this),$("#error_reflux"));
	});


	//Затирание
	//TODO нужно думать с этими переменными (согласовать с ардуино), что бы можно было запускать затирание с любого шага
	var start_brewing = 0;
	var step_brewing = 0;
	function getBrewing() {
		$.ajax({
			url: 'brewing.json',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				console.log('Brewing',msg);
				$("#brewing_temp_1").val(msg["pauseTemp1"].toFixed(0));
				$("#brewing_temp_2").val(msg["pauseTemp2"].toFixed(0));
				$("#brewing_temp_3").val(msg["pauseTemp3"].toFixed(0));
				$("#brewing_temp_4").val(msg["pauseTemp4"].toFixed(0));
				$("#brewing_time_1").val(msg["pauseTime1"].toFixed(0));
				$("#brewing_time_2").val(msg["pauseTime2"].toFixed(0));
				$("#brewing_time_3").val(msg["pauseTime3"].toFixed(0));
				$("#brewing_time_4").val(msg["pauseTime4"].toFixed(0));
				$("#brewing_time").text(msg["time"]);

				let step_brew = msg["stepBrewing"].toFixed(0);
				let step = 0;
				$(".brewing-status").removeClass('success');
				switch(step_brew) {
					case '1':
					case '2':
						step = 1;
						break;
					case '3':
					case '4':
						step = 2;
						break;
					case '5':
					case '6':
						step = 3;
						break;
					case '7':
					case '8':
						step = 4;
						break;

				}
				if(step > 0)
					$("#brewing_status_"+step).addClass('success');

				setTimeout(getPower, 2000);
			}
		});
	}

	function settingsBrewing(){
		let pauseTemp1 = Number($("#brewing_temp_1").val());
		let pauseTemp2 = Number($("#brewing_temp_2").val());
		let pauseTemp3 = Number($("#brewing_temp_3").val());
		let pauseTemp4 = Number($("#brewing_temp_4").val());

		let pauseTime1 = Number($("#brewing_time_1").val());
		let pauseTime2 = Number($("#brewing_time_2").val());
		let pauseTime3 = Number($("#brewing_time_3").val());
		let pauseTime4 = Number($("#brewing_time_4").val());

		let data_send = {
			startBrewing:start_brewing,
			stepBrewing:step_brewing,
			pauseTemp1:pauseTemp1,
			pauseTemp2:pauseTemp2,
			pauseTemp3:pauseTemp3,
			pauseTemp4:pauseTemp4,
			pauseTime1:pauseTime1,
			pauseTime2:pauseTime2,
			pauseTime3:pauseTime3,
			pauseTime4:pauseTime4
		};

		//TODO сделать действия SUCCESS в sendRequest
		sendRequest("SettingBrewing",data_send,"text",false,$(this),$("#error_brewing"));
	}

	function startBrewing(start,step){
		start_brewing = start;
		step_brewing = step;
		settingsBrewing();
	}

	function stopBrewing() {
		settingsBrewing();
	}

	$("#brewing_set_pause_temp").on("click",function () {
		settingsBrewing()
	});

	//TODO тут нужно будет передать из кнопки шаг затирания (сделать кнопку списком)
	$("#start_brewing").on("click",function () {
		startBrewing(1,1);
	});

	$("#stop_brewing").on("click",function () {
		start_brewing = 0;
		step_brewing = 0;
		stopBrewing(1,1);
		sendRequest("SetHeaterPower",{"heaterPower":0},"text",false,$(this),$("#error_brewing"));
	});

	//мощность тэна
	function getPower() {
		$.ajax({
			url: 'heater.json',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				console.log('Heater',msg);
				$("#heater_power").val(msg["heaterPower"].toFixed(2));

				//setTimeout(getDistillation, 2000);
			}
		});
	}

	$("#heater_set_power").on("click",function () {
		let pover = Number($("#heater_power").val());
		pover = (pover > 0 ? pover : 0);

		//TODO сделать действия SUCCESS в sendRequest
		sendRequest("SetHeaterPower",{"heaterPower":pover},"text",false,$(this),$("#error_heater"));
	});

	//TODO сейчас запускается каждая функция по очереди, потом переделать каждую на «старт/стоп»
	setTimeout(getSettings, 2000);
	//setInterval(getDistillation,2000);
});