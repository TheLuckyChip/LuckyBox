$(document).ready(function () {

	// Функция записи в LocalStorage
	if ( !Storage.prototype.setObj ) {
		/**
		 *
		 * @param key - string
		 * @param obj - object
		 * @returns {*}
		 */
		Storage.prototype.setObj = function (key, obj) {
			try {
				return this.setItem(key, JSON.stringify(obj));
			} catch (e) {
				if (isQuotaExceeded(e)) {
					$.fn.openModal('', 'Превышен лимит localStorage', "modal-sm", false, true);
				}
				return null;
			}
		}
	}
	// Функция чтения из LocalStorage
	if ( !Storage.prototype.getObj ) {
		Storage.prototype.getObj = function (key) {
			return JSON.parse(this.getItem(key));
		}
	}
	// Функция проверли заполнения LocalStorage
	function isQuotaExceeded(e) {
		let quotaExceeded = false;
		if (e) {
			if (e.code) {
				switch (e.code) {
					case 22:
						quotaExceeded = true;
						break;
					case 1014:
						// Firefox
						if (e.name === 'NS_ERROR_DOM_QUOTA_REACHED') {
							quotaExceeded = true;
						}
						break;
				}
			} else if (e.number === -2147024882) {
				// Internet Explorer 8
				quotaExceeded = true;
			}
		}
		return quotaExceeded;
	}

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
	//Преобразование RGB888 <-> RGB565
	function hexToRgb(hex) {
		let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		return result ? {
			r: parseInt(result[1], 16),
			g: parseInt(result[2], 16),
			b: parseInt(result[3], 16)
		} : null;
	}
	function numToHex(c) {
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
	// decimal to hex
	function dec2hex (num) {
		let hexColor = convertBase(num).from(10).to(16).toUpperCase();
		return hexColor.length === 6 ? hexColor : rgb2to3(hexColor);
	}
	// hex to decimal
	function hex2dec (hex) {
		console.log(hex);
		let hexColor = hex.length === 4 ? hex : rgb3to2(hex);
		return convertBase(hexColor).from(16).to(10);
	}
	//console.log("C7EE",rgb3to2("CCFF75"));
	//console.log("19DF08",rgb2to3("1EE1"));
	//console.log("13434741",dec2hex("13434741"));
	//console.log("CCFF75",hex2dec("CCFF75"));

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
		//TODO нужно доделать свайп, чтобы двигал правильно «ярлыки вкладок», — проверить, должно уже работать
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
		let count = Number($input.val()) - step;
		count = count < min ? min : count;
		if(count > min)
			count =  count.toFixed(fixed);
		//console.log(count,step);
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
		let count = Number($input.val()) + step;
		count = count > max ? max : count;
		count =  count.toFixed(fixed);
		//console.log(count,step);
		$input.val(count);//
		$input.change();
		//return false;
	});
	//Рендер HTML шаблонов
	function renderTpl(props) {
		return function(tok, i) {
			return (i % 2) ? props[tok] : tok;
		};
	}
	function returnTplHtml(ar_props,tpl) {
		return (ar_props.map(function(item) {
			return tpl.split(/\$\{(.+?)\}/g).map(renderTpl(item)).join('');
		}));
	}
	(function ($) {
		/**
		 * Проверка объекта на пустоту
		 * @param obj - Объект
		 * @param key - Проверять ключи в объекте
		 * @returns {boolean}
		 */
		$.fn.objIsEmpty = function (obj,key) {
			if(key) {
				for (let k in obj) {
					if (obj.hasOwnProperty(k) && obj[k] !== "") {
						return false;
					}
				}
			}else{
				for (let i in obj) {
					if (obj.hasOwnProperty(i)) {
						return false;
					}
				}
			}
			return true;
		};
		/**
		 * Убирает повторяющиеся значения из массива
		 * @param arr
		 * @returns {Array}
		 */
		$.fn.arrayUnique = function(arr){
			let uniHash={}, outArr=[], i=arr.length;
			while(i--) uniHash[arr[i]]=i;
			for(i in uniHash) outArr.push(i);
			return outArr
		};
		/**
		 * Проверка массива на одинаковые значения (возвращает массив дублей)
		 * @param arr
		 * @returns {Array}
		 */
		$.fn.duplicateValues = function(arr){
			let arr_res = [];
			arr.sort();
			for (let i=1; i < arr.length; i++) {
				if (arr[i] === arr[i-1]) {
					let is_unique = true;
					for (let k=0; k < arr_res.length; k++) {
						if (arr_res[k] === arr[i]) {
							is_unique = false;
							break;
						}
					}
					if (is_unique) {
						arr_res.push(arr[i]);
					}
				}
			}
			return arr_res;
		}
	})(jQuery);

	//загружаем контент во вкладки
	$('li.swipe-tab a').on('show.bs.tab', function (e) {
		let url = $(this).attr("href");
		let target = $(this).data("target");
		let tab = $(this);
		if($.trim($(target).html())==='') {
			$(target).ajaxLoading();
			$(target).load(url, function (response, status, xhr) {
				if ( status === "error" ) {
					$.fn.openModal('', '<p class="text-center text-danger"><strong>Ошибка загрузки вкладки</strong></p><p>' + xhr.status + ' ' + xhr.statusText + '</p>', "modal-sm",false,true);
				}
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

	/**
	 * <b>Отправка данных на сервер</b>
	 * @param {string} url - адрес
	 * @param {object} data - данные
	 * @param {string} dataType - тип передаваемых данных "text","json","html"...
	 * @param {function|boolean} success_action - действия после успешного отправления данных
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
				if(success_action !== false)
					success_action(msg)
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
//////////////////////////////////////////////////////////////////////////
	//Определение датчиков
	var sensorsJson = {};
	$(document).on('click','#get_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		//refluxTest
		sendRequest("sensors.json",{},"json",getSensors,_this,$("#error_sensors"));
	});
	function getSensors(data) {
		console.log(data);
		let sensors = data["sensors"];
		for (let key in sensors) {
			if(sensors[key]["value"]<125){
				$("#sensor_name_"+key).val(sensors[key]["name"]);
				let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
				$("#sensor_color_"+key).val(jscolor).next("button").css("background-color","#"+jscolor);
				$("#sensor_val_"+key).text(sensors[key]["value"]).parent().find(".hidden").removeClass("hidden").addClass("show");
			}else{
				if(!$.fn.objIsEmpty(sensors[key]["name"],false))
					$("#sensor_name_"+key).val("");
				$("#sensor_color_"+key).val("FFFFFF");//.css("background-color","#FFFFFF");
				$("#sensor_val_"+key).text("").parent().find(".show").removeClass("show").addClass("hidden");
			}
			//console.log(key,sensors[key]);
		}
		sensorsJson = data;
		//localStorage.setObj('dtos', self.dtos);
	}
	$(document).on('click','#set_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		let nameError = false;
		for (let key in sensorsJson["sensors"]) {
			if(sensorsJson["sensors"][key]["value"]<125){
				let val_color = $("#sensor_color_"+key).val();
				if($("#sensor_name_"+key).val() === "")
					nameError = true;
				sensorsJson["sensors"][key]["name"] = $("#sensor_name_"+key).val();
				sensorsJson["sensors"][key]["color"] = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
				//sensorsJson["sensors"][key]["member"] = 1;
			}
		}
		if(nameError) {
			$.fn.openModal('', '<p class="text-center text-danger"><strong>Заполните названия подключенных датчиков</strong></p>', "modal-sm", true, false);
		}else {
			sendRequest("SetTempTest", sensorsJson, "json", setSensors, _this, $("#error_sensors"));
		}
	});
	function setSensors(data) {
		//console.log(data);
		//console.log(sensorsJson);
		$.fn.openModal('', '<p class="text-center text-success"><strong>Тестовое сообщение, УРА!</strong></p>', "modal-sm", true, false);
		localStorage.setObj('sensors', sensorsJson);
	}
	//Привязка датчиков к процессу ректификации, и запуск
	var refluxProcess = {"sensors":[],"power":0,"start":false};
	//localStorage.setObj('reflux',refluxProcess);
	const pressureTemplate = '<tr>' +
		'<td>Атмосферное давление</td>' +
		'<td><span id="reflux_pressure"></span> <span class="hidden">мм рт.ст.</span></td>' +
		'<td></td>' +
		'<td></td>' +
		'</tr>' +
		'<tr>' +
		'<td>t&#176 кипения спирта при данном давлении</td>' +
		'<td><span id="reflux_alco_boil"></span> <span class="hidden">&#176С</span></td>' +
		'<td></td>' +
		'<td></td>' +
		'</tr>';
	const deltaTempl =
		'<div class="input-group number-group">' +
		'<span class="input-group-btn minus">' +
		'<button type="button" class="btn btn-danger btn-number noSwipe" data-type="minus" data-field="count">' +
		'<span class="glyphicon glyphicon-minus"></span>' +
		'</button>' +
		'</span>' +
		'<input id="${id}" class="form-control input-number noSwipe" type="number" value="${value}" min="${min}" max="${max}" step="${step}" size="4">' +
		'<span class="input-group-btn plus">' +
		'<button type="button" class="btn btn-success btn-number noSwipe" data-type="plus" data-field="count">' +
		'<span class="glyphicon glyphicon-plus"></span>' +
		'</button>' +
		'</span>' +
		'</div>';
	const powerTempl =
		'<tr>' +
		'<td>Мощность тена</td>' +
		'<td><span id="power_value"></span> <span class="hidden">%</span></td>' +
		'<td>' +
		returnTplHtml([{id:"power_set", value: '0', min: '0', max: '100', step: '1'}], deltaTempl) +
		'</td>' +
		'<td></td>' +
		'</tr>';

	$(document).on('click','#reflux_add_sensor',function(e) {
		e.preventDefault();
		let sensors = localStorage.getObj('sensors');//sensorsJson
		//console.log(sensors);
		if(sensors !== null) {
			let section = '<section id="reflux_sensors" class="table-responsive"><table class="table table-noborder">';
			for (let key in sensors["sensors"]) {
				if (sensors["sensors"].hasOwnProperty(key) && key !== "p1") {
					let sensor_name = sensors["sensors"][key]["name"];
					let jscolor = sensors["sensors"][key]["color"] > 0 ? dec2hex(sensors["sensors"][key]["color"]) : "FFFFFF"; //"background-color","#"+jscolor
					//if(key === "p1")
						//sensor_name = "Атмосферное давление";
					let disabled_check = "";
					if(sensor_name === "")
						disabled_check = "disabled";

					section += '<tr><td>' +
						'<div class="input-group input-group-sm">'+
						'<span class="input-group-addon">' + key + '</span>'+
						'<input id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">'+
						'</div></td>'+
						'<td><input type="hidden" id="reflux_color_' + key + '" value="'+jscolor+'">'+
						'<button class="btn btn-sm jscolor {valueElement: \'reflux_color_' + key + '\'}" style="background-color: #'+jscolor+'">Цвет</button></td>'+
						'<td><input '+disabled_check+' data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
						'<td><label class="checkbox-inline"><input disabled id="delta_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Уставка</label></td>' +
						'<td><label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Отсечка</label></td>' +
						'</tr>';
				}
				/*if (sensors["sensors"].hasOwnProperty(key) && sensors["sensors"][key]["member"] > 0) {
					section += '<tr><td><label class="checkbox-inline">' +
						'<input data-sensor="' + key + '" type="checkbox" value="' + key + '">' + sensors["sensors"][key]["name"] + '</label></td>' +
						'<td><label class="checkbox-inline"><input disabled id="delta_' + key + '" name="reflux_delta" type="radio" value="Y">Уставка</label></td>' +
						'</tr>';
				}*/
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для ректификации', section, "modal-md", false, {
				text: "Выбрать",
				id: "sensors_select",
				class: "btn btn-success",
				click: function () {
					let sensors_select = $('#reflux_sensors input[type=checkbox]');
					let reflux_sensors = $.map(sensors_select, function (e) {
						if ($(e).is(":checked")) {
							//console.log($(e).data("sensor"));
							let key = $(e).data("sensor");
							let name = $("#reflux_name_" + key).val();
							let val_color = $("#reflux_color_" + key).val();
							let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
							let delta = $("#delta_" + key).prop("checked");
							let cutoff = $("#cutoff_" + key).prop("checked");
							return {"key": key, "name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue":0};
						}
					});
					refluxProcess["sensors"] = reflux_sensors;
					localStorage.setObj('reflux', refluxProcess);
					//console.log(ar_sensors);
					$(this).closest(".modal").modal("hide");
					pasteRefluxSensors();
				}
			},
			{id: "modal_sensors_select"}
			);
			//jscolor.installByClassName("jscolor");
		}
	});
	$(document).on('shown.bs.modal',"#modal_sensors_select", function(event){
		//console.log(event);
		jscolor.installByClassName("jscolor");
	});
	$(document).on('click','#reflux_sensors input[type=checkbox]',function() {
		let checked = !$(this).prop("checked");
		let radio_delta = $("#delta_"+$(this).data("sensor"));
		let radio_cutoff = $("#cutoff_"+$(this).data("sensor"));
		radio_delta.prop("disabled",checked);
		radio_cutoff.prop("disabled",checked);
		if(checked) {
			radio_delta.prop("checked", false);
			radio_cutoff.prop("checked",false);
		}
	});
	pasteRefluxSensors = function(){
		let sensors = localStorage.getObj('sensors');//sensorsJson
		let refluxTemplate = '';
		let localReflux = localStorage.getObj('reflux');
		if(localReflux !== null)
			refluxProcess = localReflux;
		if(refluxProcess["sensors"].length > 0) {
			$.each(refluxProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = e["key"];
				let name_sensor = e["name"];
				sensors["sensors"][sensor_key]["name"] = name_sensor;
				sensors["sensors"][sensor_key]["color"] = e["color"];
				sensors["sensors"][sensor_key]["member"] = 1;
				let tpl_delta = '';
				let tpl_delta_result = '';
				if (e["delta"]) {
					tpl_delta = returnTplHtml([{id:"reflux_delta_"+sensor_key, value: e["allertValue"], min: '0', max: '1', step: '0.05'}], deltaTempl);
					tpl_delta_result = '<span id="reflux_delta_result_'+sensor_key+'"></span> <span class="hidden">&#176С</span>'
				}
				if (e["cutoff"]) {
					tpl_delta = returnTplHtml([{id:"reflux_cutoff_"+sensor_key, value: e["allertValue"], min: '0', max: '105', step: '1'}], deltaTempl);
					tpl_delta_result = '<span id="reflux_cutoff_result_'+sensor_key+'"></span> <span class="hidden">&#176С</span>'
				}
				refluxTemplate += '<tr><td>t&#176' + name_sensor + '</td>' +
					'<td><span id="reflux_' + sensor_key + '"></span> <span class="hidden">&#176С</span></td>' +
					'<td>' + tpl_delta + '</td>' +
					'<td>' + tpl_delta_result + '</td>' +
					'</tr>'
			});
			sensors["process"]["allow"] = 2;
		}
		if(refluxTemplate !== '') {
			sendRequest("SetTempTest", sensors, "json", false, false, $("#error_reflux"));
			//localStorage.setObj('sensors', sensors);
			refluxTemplate += pressureTemplate + powerTempl;
			$("#start_group_button").removeClass("hidden");
		}else{
			$("#start_group_button").addClass("hidden");
		}
		$("#reflux_process tbody").html(refluxTemplate);
		$("#power_set").val(refluxProcess["power"]);
		if(refluxProcess["start"] === true) {
			getReflux();
			//refluxStartProcess ()
			$('#reflux_start').prop("disabled",true);
		}else{
			$('#reflux_stop').prop("disabled",true);
		}
	};

	var tmpRefluxFlag = false;
	$(document).on('click','#reflux_start',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#reflux_stop').prop("disabled",false);
		//let sendRefluxProcess = refluxProcess;
		refluxProcess["start"] = tmpRefluxFlag = true;
		//sendRequest("refluxModeSensorsIn", sendRefluxProcess, "json", refluxStartProcess, _this, $("#error_reflux"));
		//refluxStartProcess ();
		//localStorage.setObj('reflux',refluxProcess);
		setReflux();
		setTimeout(getReflux, 1000);
	});
	$(document).on('click','#reflux_stop',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#reflux_start').prop("disabled",false);
		//let sendRefluxProcess = refluxProcess;
		//sendRefluxProcess["start"] = false;
		//sendRequest("SetTempTest", sendRefluxProcess, "json", refluxStopProcess, _this, $("#error_reflux"));
		//refluxStopProcess ();
		tmpRefluxFlag = true;
		refluxProcess["start"] = false;
		//localStorage.setObj('reflux',refluxProcess);
		//getReflux();
		setReflux();
	});
	/*function refluxStartProcess () {

		//setInterval(getDistillation,2000);
		//refluxProcess = localStorage.getObj('reflux');
		refluxProcess["start"] = true;
		localStorage.setObj('reflux',refluxProcess);
		getReflux();
	}
	function refluxStopProcess () {
		//refluxProcess = localStorage.getObj('reflux');
		refluxProcess["start"] = false;
		localStorage.setObj('reflux',refluxProcess);
		getReflux();

	}*/
	function setReflux() {
		let refluxSendData = {
			"process":{"allow":0,"number":0},
			"t1":{"allertValue":0},
			"t2":{"allertValue":0},
			"t3":{"allertValue":0},
			"t4":{"allertValue":0},
			"t5":{"allertValue":0},
			"t6":{"allertValue":0},
			"t7":{"allertValue":0},
			"t8":{"allertValue":0},
			"power":0
		};
		let flag_send = false;
		let power_set = $("#power_set");
		refluxSendData["process"]["allow"] = (refluxProcess["start"] ? 1 : 0);
		refluxSendData["power"] = refluxProcess["power"] = power_set.val();
		if(refluxProcess["power"] !== power_set.val())
			flag_send = true;

		$.each(refluxProcess["sensors"], function (i, e) {
			let sensor_key = e["key"];
			let reflux_delta = $("#reflux_delta_"+sensor_key);
			let reflux_cutoff = $("#reflux_cutoff_"+sensor_key);
			if(reflux_delta.length) {
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_delta.val();
				if(e["allertValue"] !== reflux_delta.val())
					flag_send = true;
			}
			if(reflux_cutoff.length) {
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_cutoff.val();
				if(e["allertValue"] !== reflux_cutoff.val())
					flag_send = true;
			}
		});
		if(tmpRefluxFlag)
			flag_send = true;
		if(flag_send) {
			tmpRefluxFlag = false;
			localStorage.setObj('reflux', refluxProcess);
			sendRequest("refluxModeSensorsIn", refluxSendData, "json", false, false, $("#error_reflux"));
		}
	}
	function getReflux() {
		setReflux();
		$.ajax({
			url: 'refluxModeSensorsOut',//'refluxModeSensorsIn',//'reflux.json',//refluxModeSensorsOut
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				console.log('Reflux',msg);
				$.each(msg["sensors"],function (i,e) {
					let sensor_key = Object.keys(e).shift();
					$("#reflux_"+sensor_key).text(msg["sensors"][i][sensor_key]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					let allertValue = msg["sensors"][i][sensor_key]["allertValue"];
					allertValue = allertValue > 0 ? allertValue.toFixed(2) : "";
					if(allertValue > 0) {
						$("#reflux_delta_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#reflux_cutoff_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
					}
					if(sensor_key === "p1")
						$("#reflux_pressure").text(msg["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				});
				//$("#reflux_delta").val(msg["delta"].toFixed(2));
				//$("#reflux_delta_result").text(msg["delta_result"].toFixed(2));

				$("#reflux_alco_boil").text(msg["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				$("#power_value").text(msg["power"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				//$("#power_cutoff_result").text(msg["power"].toFixed(2));
				/*
				if(msg["settingAlarm"] === false){
					$("#reflux_status").removeClass("success").addClass("danger");
				}else{
					$("#reflux_status").removeClass("danger").addClass("success");
				}*/
				if(refluxProcess["start"] === true)
					setTimeout(getReflux,2000);
			}
		});

	}






	//Свойства TODO - переделать все свойства в один запрос
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
	/*function getReflux() {
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
	});*/


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