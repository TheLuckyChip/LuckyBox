(function ($) {
	/**
	 * Проверка объекта на пустоту
	 * @param {object} obj - Объект
	 * @param {boolean} key - Проверять ключи в объекте
	 * @returns {boolean}
	 */
	$.fn.objIsEmpty = function (obj, key) {
		if (key) {
			for (let k in obj) {
				if (obj.hasOwnProperty(k) && obj[k] !== "") {
					return false;
				}
			}
		} else {
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
	$.fn.arrayUnique = function (arr) {
		let uniHash = {}, outArr = [], i = arr.length;
		while (i--){
			uniHash[arr[i]] = i
		}
		for (let j in uniHash){
			outArr.push(j)
		}
		return outArr
	};
	/**
	 * Проверка массива на одинаковые значения (возвращает массив дублей)
	 * @param arr
	 * @returns {Array}
	 */
	$.fn.duplicateValues = function (arr) {
		let arr_res = [];
		arr.sort();
		for (let i = 1; i < arr.length; i++) {
			if (arr[i] === arr[i - 1]) {
				let is_unique = true;
				for (let k = 0; k < arr_res.length; k++) {
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
	};
	/**
	 * Удаление элемента из массива
	 * @param arr
	 * @param value
	 * @returns {Array}
	 */
	$.fn.arrayUnset = function (arr, value) {
		for (let i = 0; i < arr.length; i++) {
			if (arr[i] === value) {
				arr.splice(i, 1);
				break;
			}
		}
		return arr;
	};

	/**
	 * Заполнение селекта
	 * @param select
	 * @param dataArray
	 * @param optgroup
	 */
	$.fn.fillSelect = function (select, dataArray, optgroup) {
		if (optgroup === undefined){ optgroup = false}

		if (select.prop('tagName') === 'SELECT') {
			if (!optgroup) {
				$.each(dataArray, function (index, value) {
					let option_text = "";
					let option_val = "";
					$.each(value, function (key, val) {
						//console.log(key, val);
						if (key === "text") {
							option_text = val;
						} else {
							option_val += " " + key + "=\"" + val + "\"";
						}
					});
					let option = "<option" + option_val + ">" + option_text + "</option>";
					select.append($(option)
						//.attr("value", value.id)
						//.text(value.name)
						//.attr("data-"+value.data[0],value.data[1])
					);
				});
			} else {
				let opt_group = $('<optgroup label="' + optgroup + '">');
				$.each(dataArray, function (index, value) {
					let option_text = "";
					let option_val = "";
					$.each(value, function (key, val) {
						//console.log(key, val);
						if (key === "text") {
							option_text = val;
						} else {
							option_val += " " + key + "=\"" + val + "\"";
						}
					});
					let option = "<option" + option_val + ">" + option_text + "</option>";
					opt_group.append($(option)
						//.attr("value", value.id)
						//.text(value.name)
					);
				});
				opt_group.appendTo(select);
			}
		}
	};
	/**
	 * Очистка селекта
	 * @param select
	 */
	$.fn.clearSelect = function (select) {
		if (select.prop('tagName') === 'SELECT') {
			select.prop('options').length = 0;
		}
	}
})(jQuery);
//$(document).ready(function () {
$(function () {
	// Функция записи в LocalStorage
	if (!Storage.prototype.setObj) {
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
					$.fn.openModal('', '<p class="text-center text-danger text-strong">Превышен лимит localStorage, данные графиков переполнили память браузера</p>', "modal-sm", false, [
						{
							text: "Закрыть",
							id: "return_tab",
							class: "btn btn-primary btn-sm",
							click: function () {
								$(this).closest(".modal").modal("hide");
							}
						},
						{
							text: "Удалить графики",
							id: "del_graf",
							class: "btn btn-danger btn-sm",
							click: function () {
								$(this).closest(".modal").modal("hide");
								clearChart();
							}
						}], {buttons: "replace"});
				}
				return null;
			}
		}
	}
	// Функция чтения из LocalStorage
	if (!Storage.prototype.getObj) {
		Storage.prototype.getObj = function (key) {
			return JSON.parse(this.getItem(key));
		}
	}

	// Функция проверки заполнения LocalStorage
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
	function convertBase(num) {
		return {
			from: function (baseFrom) {
				return {
					to: function (baseTo) {
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
		let r = ((color >> 11) & 0x1F) * 0xff / 0x1F;
		let g = ((color >> 5) & 0x3F) * 0xff / 0x3F;
		let b = ((color) & 0x1F) * 0xff / 0x1F;

		r = Math.round(r);
		g = Math.round(g);
		b = Math.round(b);

		return (numToHex(r) + numToHex(g) + numToHex(b)).toUpperCase();
	}

	// decimal to hex
	function dec2hex(num) {
		let hexColor = convertBase(num).from(10).to(16).toUpperCase();
		return hexColor.length === 6 ? hexColor : rgb2to3(hexColor);
	}

	// hex to decimal
	function hex2dec(hex) {
		//console.log(hex);
		let hexColor = hex.length === 4 ? hex : rgb3to2(hex);
		return convertBase(hexColor).from(16).to(10);
	}

	//console.log("C7EE",rgb3to2("CCFF75"));
	//console.log("19DF08",rgb2to3("1EE1"));
	//console.log("13434741",dec2hex("13434741"));
	//console.log("CCFF75",hex2dec("CCFF75"));

	/**
	 * заливка svg цветом, в зависимости от параметров датчиков
	 * @param {string} color  - hex цвет
	 * @param {number} current - текущее значение датчика
	 * @param {number} max - максимально возможное значение датчика
	 * @returns {string}
	 */
	function colorPersent(color, current, max) {
		let rgb = hexToRgb(color);
		let alpha = 1;
		if (Number(max) > 0) {
			alpha = ((current * 100) / max) / 100
		}
		return 'rgba(' + rgb.r + ',' + rgb.g + ',' + rgb.b + ',' + alpha + ')';
		//return "#FF0000";
	}

	//Преобразование секунд во время
	function secToTime(sec){
		let dt = new Date();
		dt.setTime(sec*1000);
		let hours = dt.getUTCHours();
		let minutes = dt.getUTCMinutes();
		// let seconds = dt.getUTCSeconds();
		return (hours<10 ? "0" + hours : hours) +":"+ (minutes<10 ? "0" + minutes : minutes);
	}

	/**
	 * Проверка на мобильную версию
	 * @returns {boolean}
	 */
	function isMobile() {
		try {
			if (/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini|Opera Mobile|Kindle|Windows Phone|PSP|AvantGo|Atomic Web Browser|Blazer|Chrome Mobile|Dolphin|Dolfin|Doris|GO Browser|Jasmine|MicroB|Mobile Firefox|Mobile Safari|Mobile Silk|Motorola Internet Browser|NetFront|NineSky|Nokia Web Browser|Obigo|Openwave Mobile Browser|Palm Pre web browser|Polaris|PS Vita browser|Puffin|QQbrowser|SEMC Browser|Skyfire|Tear|TeaShark|UC Browser|uZard Web|wOSBrowser|Yandex.Browser mobile/i.test(navigator.userAgent)) {
				console.log("Is isMobile");
				return true;
			}
			return false
		}
		catch (e) {
			console.log("Error in isMobile");
			return false;
		}
	}

	//Запуск аудио
	let audio =  document.getElementById("alert_audio");
	// console.log(audio.duration);
	// audio.addEventListener('timeupdate',function(){
	// 	let currentTimeMs = audio.currentTime*1000;
	// 	console.log(currentTimeMs);
	// },false);
	let playAudio = false;
	function stopSound() {
		audio.loop = false;
		audio.pause();
		if (isMobile && window.navigator && window.navigator.vibrate) {
			window.navigator.vibrate(0);
		}
		// audio.currentTime = 0;
		playAudio = false;
	}
	function playSound() {
		if(!playAudio && audio !== undefined) {
			//audio.pause();
			//audio.currentTime = 0;
			audio.load();

			playAudio = true;
			audio.loop = true;
			audio.play();
			// Вибрация поддерживается
			if (isMobile && window.navigator && window.navigator.vibrate) {
				navigator.vibrate(1000);
			}
			setTimeout(function () {
				audio.currentTime = 0;
				stopSound();
			}, 5000);
		}
	}
	//Свайп вкладок
	$(function () {
		function widthOfList() {
			let itemsWidth = 0;
			let itemWidth = 0;
			$('.list-tab li').each(function () {
				itemWidth = $(this).outerWidth();
				itemsWidth += itemWidth;
			});
			return itemsWidth;
		}

		function widthOfHidden() {
			//console.log($('.wrapper-nav').outerWidth(), widthOfList(), getLeftPos());
			//console.log($('.wrapper-nav').outerWidth() - widthOfList() - getLeftPos());
			return ($('.wrapper-nav').outerWidth() - widthOfList() - getLeftPos() + 2);
		}

		function getLeftPos() {
			return $('.list-tab').position().left;
		}

		$(".list-tab-content").swipe({
			swipeStatus: function (event, phase, direction, distance, fingerCount) {
				if (phase === "move" || phase === "start") {
					let $target = event.target.nodeName;
					if ($target.toLowerCase() === 'input' || $target.toLowerCase() === 'select' || $target.toLowerCase() === 'button') {
						return false;
					} else {
						$('input,select,button').blur();
					}
				}
			},
			swipeLeft: function (event, distance, duration, fingerCount, fingerData, currentDirection) {
				let listWidth = widthOfList();
				let hiddenWidth = widthOfHidden();
				let containerWidth = $('.wrapper-nav').outerWidth();
				let $tab = $('.list-tab .active').next();


				if ($tab.length > 0 && $("a.disabled", $tab).length === 0) {
					let posTab = $tab.offset();
					let posList = $('.list-tab').offset();
					//console.log($tab,posTab);
					let leftTab = posTab.left - 10;


					$tab.find('a').tab('show');
					//console.log(hiddenWidth, listWidth, leftTab, containerWidth, (listWidth-Math.abs(posList.left)));
					if ((listWidth - Math.abs(posList.left)) > containerWidth) {
						//if(hiddenWidth < listWidth)
						$('.list-tab').animate({left: "-=" + leftTab + "px"}, 'slow');
						//$('.list-tab').animate({left:"+="+widthOfHidden()+"px"},'slow');
					}
				}
			},
			swipeRight: function (event, distance, duration, fingerCount, fingerData, currentDirection) {
				//let listWidth = widthOfList();
				//let hiddenWidth = widthOfHidden();
				//let containerWidth = $('.wrapper-nav').outerWidth();
				let $tab = $('.list-tab .active').prev();
				if ($tab.length > 0 && $("a.disabled", $tab).length === 0) {
					let posTab = $tab.offset();
					//let posList = $('.list-tab').offset();
					let leftTab = posTab.left - 10;
					$tab.find('a').tab('show');
					//console.log(hiddenWidth, listWidth, leftTab, containerWidth, (listWidth-Math.abs(posList.left)));
					if (leftTab < 0) {
						//if(hiddenWidth < listWidth)
						$('.list-tab').animate({left: "-=" + leftTab + "px"}, 'slow');
						//$('.list-tab').animate({left:"-="+getLeftPos()+"px"},'slow');
					}
				}
			},
			allowPageScroll: "auto"
		});
	});
	//Скрытие навбара после клика
	$(document).on('click', '.navbar-collapse.in', function (e) {
		if ($(e.target).is('a') && ($(e.target).attr('class') !== 'dropdown-toggle')) {
			$(this).collapse('hide');
		}
	});

	//Рендер HTML шаблонов
	function renderTpl(props) {
		return function (tok, i) {
			return (i % 2) ? props[tok] : tok;
		};
	}

	function returnTplHtml(ar_props, tpl) {
		return (ar_props.map(function (item) {
			return tpl.split(/\$\{(.+?)\}/g).map(renderTpl(item)).join('');
		}));
	}

	$('#navbar-menu li a').on("click",function(e){
		if($(this).hasClass("disabled")){
			e.preventDefault();
			e.stopPropagation();
			e.stopImmediatePropagation();
			return false;
		}
	});

	$('#nav-tabs li a').on("click",function(e){
		if($(this).hasClass("disabled")){
			e.preventDefault();
			e.stopPropagation();
			e.stopImmediatePropagation();
			return false;
		}
	});

	//загружаем контент во вкладки
	$('li.swipe-tab a').on('show.bs.tab', function (e) {
		let url = $(this).attr("href");
		let target = $(this).data("target");
		let tab = $(this);
		if ($.trim($(target).html()) === '') {
			$(target).ajaxLoading();
			$(target).load(url, function (response, status, xhr) {
				console.log("tabs");
				if (status === "error") {
					$.fn.openModal('', '<p class="text-center text-danger text-strong">Ошибка загрузки вкладки</p><p>' + xhr.status + ' ' + xhr.statusText + '</p>', "modal-sm", false, true);
				}
				$(target).ajaxLoading('stop');
				tab.tab('show');
			});
		}else{
			switch (target) {
				case "#distillation":
					distillationProcess["sensors"] = {};
					$.fn.pasteDistillationSensors(false);
					break;
				case "#reflux":
					refluxProcess["sensors"] = {};
					$.fn.pasteRefluxSensors(false);
					break;
				case "#mashing":
					mashingProcess["sensors"] = {};
					$.fn.pasteMashingSensors(false);
					break;
			}
		}
	});
	$('li.swipe-tab a:first').tab('show');

	function alertAjaxError(err, exception, container) {
		//console.log(err,exception);
		let msg = '';

		if (err.status === 0) {
			msg = 'Нет подключения. Проверьте сеть.';
		} else if (err.status === 404) {
			msg = 'Такой страницы не существует. [404]';
		} else if (err.status === 500) {
			msg = 'Внутренняя ошибка сервера [500].';
		} else if (exception === 'parsererror') {
			msg = 'Ошибка парсинга JSON параметров.';
		} else if (exception === 'timeout') {
			msg = 'Ошибка тайм-аута (превышено время ожидания ответа).';
		} else if (exception === 'abort') {
			msg = 'Запрос Ajax прерван.';
		} else {
			msg = 'Неизвестная ошибка.' + err.responseText;
		}
		let error_alert = '<div class="alert alert-danger alert-dismissible fade in mt-10" role="alert">' +
			'<button type="button" class="close" data-dismiss="alert" aria-label="Close">' +
			'<span aria-hidden="true">&times;</span>' +
			'</button>' +
			'<span class="text-strong">' + msg + '</span>' +
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
	 * @param {string|boolean} success_text - текстовое сообщение в диалоговое окно при успешной отправке данных
	 */
	let ajax_url_debug = '';//'http://192.168.0.190/';
	function sendRequest(url, data, dataType, success_action, load_target, error_target, success_text) {
		//console.log(url,data,target);
		$.ajax({
			url: ajax_url_debug + url,
			data: data,
			type: 'GET',
			dataType: dataType,
			beforeSend: function () {
				if (load_target !== false) {
					$(load_target).ajaxLoading({disabled: true, color: "#FF0000"});
				}
			},
			success: function (msg) {
				if (success_action !== false) {
					success_action(msg);
				}
				if (success_text !== false) {
					$.fn.openModal('', success_text, "modal-sm", true, false);
				}
			},
			error: function (err, exception) {
				if (error_target !== false) {
					alertAjaxError(err, exception, error_target);
				}
			},
			complete: function () {
				if (load_target !== false) {
					$(load_target).ajaxLoading('stop');
				}
			}
		});
	}
	//Кнопки + и -
	let flagSendProcess = false;
	let timeout = false;
	function f(n) {
		n = (typeof n === 'string') ? n : n.toString();
		if (n.indexOf('e') !== -1) return parseInt(n.split('e')[1]) * -1;
		let separator = (1.1).toString().split('1')[1];
		let parts = n.split(separator);
		return parts.length > 1 ? parts[parts.length - 1].length : 0;
	}
	// TODO не работает в ИЕ
	/*function f(x) {
		return x.toString().includes('.') ? x.toString().split('.').pop().length : 0;
	}*/
	$(document).on('mousedown', '.minus', function (e) {
		e.preventDefault();
		flagSendProcess = true;
		let _this = $(this);
		let count_interval = 0;
		let time = 500;
		let $input = _this.parent().find('input');
		let step = Number($input.attr("step"));
		let min = Number($input.attr("min"));
		timeout = setInterval(function () {
			count_interval++;
			let fixed = 0;
			// const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
			// const f = x => ~(x + '').indexOf('.') ? (x + '').split('.')[1].length : 0;
			if (typeof min === typeof undefined && min === false) {
				min = 0;
			}
			if (typeof step !== typeof undefined && step !== false) {
				step = parseFloat(step);
				fixed = f(step);
			} else {
				step = 1;
			}
			if (count_interval > 3) {
				time = time / 2;
				step = step * 2;
				count_interval = 0;
			}
			let count = Number($input.val()) - step;
			count = count < min ? min : count;
			if (count > min) {
				count = count.toFixed(fixed);
			}
			//console.log(count,step);
			$input.val(count);
		}, time);
	});
	$(document).on('mouseup', '.minus', function (e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		$input.change();
		clearInterval(timeout);
		setTimeout(function() {flagSendProcess = false;}, 500);
	});
	$(document).on('click', '.minus', function (e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		let step = Number($input.attr("step"));
		let min = Number($input.attr("min"));
		let fixed = 0;
		// const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
		if (typeof min === typeof undefined && min === false) {
			min = 0;
		}
		if (typeof step !== typeof undefined && step !== false) {
			step = parseFloat(step);
			fixed = f(step);
		} else {
			step = 1;
		}
		let count = Number($input.val()) - step;
		count = count < min ? min : count;
		if (count > min) {
			count = count.toFixed(fixed);
		}
		$input.val(count);
		//$input.change();
	});
	$(document).on('mousedown', '.plus', function (e) {
		//$(".plus").on('mouseup',function(e) {
		e.preventDefault();
		flagSendProcess = true;
		let _this = $(this);
		let count_interval = 0;
		let time = 500;
		let $input = _this.parent().find('input');
		let step = Number($input.attr("step"));
		let max = Number($input.attr("max"));
		timeout = setInterval(function () {
			count_interval++;
			let fixed = 0;
			// const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
			if (typeof max === typeof undefined && max === false) {
				max = 100;
			}
			if (typeof step !== typeof undefined && step !== false) {
				step = parseFloat(step);
				fixed = f(step);
			} else {
				step = 1;
			}
			if (count_interval > 3) {
				time = time / 2;
				step = step * 2;
				count_interval = 0;
			}
			//console.log(time,step);
			let count = Number($input.val()) + step;
			count = count > max ? max : count;
			count = count.toFixed(fixed);

			$input.val(count);//
			//$input.change();
			//return false;
		}, time);
	});
	$(document).on('mouseup', '.plus', function (e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		$input.change();
		clearInterval(timeout);
		setTimeout(function() {flagSendProcess = false;}, 500);
	});
	$(document).on('click', '.plus', function (e) {
		e.preventDefault();
		let $input = $(this).parent().find('input');
		let step = Number($input.attr("step"));
		let max = Number($input.attr("max"));
		let fixed = 0;
		// const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
		if (typeof max === typeof undefined && max === false) {
			max = 100;
		}
		if (typeof step !== typeof undefined && step !== false) {
			step = parseFloat(step);
			fixed = f(step);
		} else {
			step = 1;
		}
		let count = Number($input.val()) + step;
		count = count > max ? max : count;
		//console.log(count,step,fixed);
		count = count.toFixed(fixed);
		$input.val(count);
		//$input.change();
	});
	$(document).on("change",".number-group input.input-number",function () {
		let $input = $(this);
		let val = Number($input.val());
		let min = Number($input.attr("min"));
		let max = Number($input.attr("max"));
		if(val < min){
			$input.val(parseFloat(min));
		}else if(val > max){
			$input.val(parseFloat(max));
		}else{
			$input.val(parseFloat(val));
		}
	});

	//поиск нужного значения в датчиках
	function getSensorValue(key) {
		let keyData = $.map(globalSensorsJson["sensors"], function (e) {
			return e[key]
		});
		return keyData[0];
	}

//////////////////////////////////////////////////////////////////////////
	//Определение датчиков
	let sensorsJson = {};
	let globalSensorsJson = {};
	let sensorsIntervalId = 0;
	//Флаг отправки данных процесса в МК
	//let flagSendProcess = false;
	//Интервал запуска процесса
	let sensorsProcessId = false;
	//регекспы для датчиков
	const re_p = new RegExp(/p1/);
	const re_t = new RegExp(/^t/);
	const re_out = new RegExp(/^out/);
	const re_in = new RegExp(/^in/);
	const re_pause = new RegExp(/^pause/);
	const re_Kp = new RegExp(/Kp/);
	const re_Ki = new RegExp(/Ki/);
	const re_Kd = new RegExp(/Kd/);
	//Список алгоритмов ректификации
	let algorithmReflux = [
		{"value":0,"text":"Ручной режим, только сигнализация"},
		{"value":1,"text":"Прима отбор по пару (головная фракция по жидкости)"},
		{"value":2,"text":"Отбор по пару"},
		{"value":3,"text":"РК по жидкости"},
		{"value":4,"text":"Бражная колонна, регулировка отбора охлаждением"},
		{"value":5,"text":"Бражная колонна, регулировка отбора мощностью"}
		// {"value":3,"text":"РК по жидкости 1 клапан (головы - импульсы, тело - дельта)"},
		// {"value":4,"text":"РК по жидкости 2 клапана (головы - импульсы, тело - дельта)"},
		// {"value":5,"text":"РК по жидкости 2 клапана (головы - открыт, тело - дельта)"},
		// {"value":6,"text":"Бражная колонна, регулировка отбора охлаждением"},
		// {"value":7,"text":"Бражная колонна, регулировка отбора мощностью"}
	];
	//Глобальный объект dtoReceiver служит для опроса МК.
	let dtoReceiver = {
		dtos: [],                               // Контейнер состояний в ОЗУ
		frequencyRecordingToLocalStorage: 5,    // Частота архивации (Через сколько опросов осуществляется запись в localStorage)
		reqestDelayDefalt: 1000,

		dtoGet: function (json, target) {
			const self = dtoReceiver;  // Для доступа к this в jquery
			let requestCounter = 0;    // Счётчик запросов, служит для записи в localStorage каждые frequencyRecordingToLocalStorage раз

			self.dtoCurrent = json;
			self.dtoContainer = target;
			self.dtoCurrent.d = (new Date()).getTime();  // Пользуемся временем клиента, для несчастных без доступа к NTP

			// Считывание предыдущих сохранённых значений
			this.dtos = localStorage.getObj('dtos');

			// Проверка на существование сохранённых значений
			if (this.dtos == null) {
				this.dtos = [];
			}

			self.dtos.push(self.dtoCurrent);

			// Запись в хранилище
			if ((requestCounter++ % self.frequencyRecordingToLocalStorage) === 0) {
				localStorage.setObj('dtos', self.dtos);
			}

			// Вызов события что данные получены
			$(document).trigger('newDTOreceived', [self.dtoCurrent, self.dtoContainer]);
		},

		// Очистка LocalStorage
		clearDeviceConditions: function () {
			this.dtos = [];
			localStorage.removeItem('dtos');
		},
		// Запуск опроса ESP
		start: function (dtoJson, target) {
			//console.log("startChart",target);
			this.dtoGet(dtoJson, target);
		}
	};

	Highcharts.setOptions({
		global: {
			useUTC: false
		},
		lang: {
			loading: 'Загрузка...',
			months: ['Январь', 'Февраль', 'Март', 'Апрель', 'Май', 'Июнь', 'Июль', 'Август', 'Сентябрь', 'Октябрь', 'Ноябрь', 'Декабрь'],
			weekdays: ['Воскресенье', 'Понедельник', 'Вторник', 'Среда', 'Четверг', 'Пятница', 'Суббота'],
			shortMonths: ['Янв', 'Фев', 'Март', 'Апр', 'Май', 'Июнь', 'Июль', 'Авг', 'Сент', 'Окт', 'Нояб', 'Дек'],
			exportButtonTitle: "Экспорт",
			printButtonTitle: "Печать",
			rangeSelectorFrom: "С",
			rangeSelectorTo: "По",
			rangeSelectorZoom: "Период",
			downloadPNG: 'Скачать PNG',
			downloadJPEG: 'Скачать JPEG',
			downloadPDF: 'Скачать PDF',
			downloadSVG: 'Скачать SVG',
			printChart: 'Напечатать график'
		}
	});

	let plot = {};
	let drowChart = false;
	function startChart() {
		drowChart = true;
		$(document).one("newDTOreceived", function (e) {
			//console.log(e,dtoReceiver.dtoContainer);
			let process = Number(globalSensorsJson["process"]["allow"]);
			if (process !== 0) {
				plot = getPlot();
			}
		});
	}

	function clearChart() {
		drowChart = false;
		$(document).off("newDTOreceived");
		plot = {};
		//console.log("clearChart");
		dtoReceiver.clearDeviceConditions();
		if (plot.hasOwnProperty("series")) {
			plot.series.forEach(function (s) {
				s.setData([])
			});
			plot.redraw();
		}
	}

	function getPlot() {
		console.log("Запуск графиков!");
		let jsonPlot = {
			chart: {},
			xAxis: {
				type: 'datetime',
				dateTimeLabelFormats: {
					day: "%e. %b",
					month: "%b '%y",
					year: "%Y"
				}
			},
			yAxis: [{ // Primary yAxis
				labels: {
					format: '{value}°C',
					style: {
						color: 'black'
					}
				},
				title: {
					text: 'Температура',
					style: {
						color: 'black'
					}
				}
			}
			],
			series: [],
			rangeSelector: {
				buttons: [{
					count: 1,
					type: 'minute',
					text: '1M'
				}, {
					count: 5,
					type: 'minute',
					text: '5M'
				}, {
					type: 'all',
					text: 'All'
				}],
				inputEnabled: false,
				selected: 2
			},
			title: {
				text: 'Данные датчиков'
			},
			legend: {
				enabled: true
			},
			exporting: {
				enabled: true
			},
			plotOptions: {
				series: {
					showInNavigator: true
				}
			},
		};
		if (globalSensorsJson["process"]["allow"] !== 4){
			jsonPlot.yAxis[1] = { // Secondary yAxis
				gridLineWidth: 0,
				title: {
					text: 'Мощность',
					style: {
						color: 'black'
					}
				},
				labels: {
					format: '{value} %',
					style: {
						color: 'black'
					},
					align: 'left',
					x: 0,
				},
				max: 100,
				opposite: false
			};
			jsonPlot.series[0] = {
				name: "Мощность", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 1, showInLegend: true,
				data: dtoReceiver.dtos.map(function (dc) {
					return [dc.d, dc.h]
				})
			};
		}

		if (globalSensorsJson["process"]["allow"] === 2) {
			jsonPlot.yAxis[2] = { // Secondary yAxis
				gridLineWidth: 0,
				title: {
					text: 'Атм. давление',
					style: {
						color: 'black'
					}
				},
				labels: false,
				max: 800,
				opposite: false
			};
			jsonPlot.series[1] = {
				name: "Атмосферное давление", yAxis: 2, type: "area", step: 'left', fillOpacity: 0.05, color: "#00e4f0", lineWidth: 1, showInLegend: true,
				data: dtoReceiver.dtos.map(function (dc) {
					return [dc.d, dc.p]
				})
			};
		}

		let view_chart = dtoReceiver.dtoContainer;
		let plotNew = Highcharts.stockChart(view_chart, jsonPlot);

		//console.log("plot",dtoReceiver.dtos[0].temperatures);
		$.each(dtoReceiver.dtos[0].t, function (key, t) {
			// console.log("plot", key, t);
			if (re_t.test(key)) {
				let sensorData = getSensorValue(key);
				plotNew.addSeries({
					name: sensorData["name"],
					color: "#"+dec2hex(sensorData["color"]),
					data: dtoReceiver.dtos.map(function (dc) {
						return [dc.d, dc.t[key]]
					})
				});
			}
		});

		$(document).on("newDTOreceived", function (e, dto) {
			if (plot.hasOwnProperty("series")) {
				let count = 0;
				let process = Number(globalSensorsJson["process"]["allow"]);
				if (process !== 4) {
					plot.series[0].addPoint([dto.d, dto.h], false);
				}
				if (process === 4) {
					count = -1;
				}
				if (process === 2) {
					plot.series[1].addPoint([dto.d, dto.p], false);
					count = 1;
				}
				//console.log("newDTOreceived", dto.temperatures);
				$.each(dto.t, function (key, t) {
					//console.log("plot", key, t);
					if (re_t.test(key)) {
						plotNew.series[count + 1].addPoint([dto.d, t], false);
						count++;
					}
				});
			}
			plotNew.redraw();
		});

		return plotNew;
	}

	//опрос датчиков
	$(document).on('click', '#get_sensors', function (e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("sensorsOutSet", {}, "json", getSensors, _this, $("#error_sensors"), false);
	});
	$(document).on('click', '#reset_sensors', function (e) {
		e.preventDefault();
		let _this = $(this);
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите сбросить все настройки?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				sendRequest("resetData", {"reset":1}, "json", getSensors, _this, $("#error_sensors"), false);
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

	//сортировка датчиков
	$(document).on('change', "#sensors_settings select", function () {
		let arrSortSensors = [];
		$.each($('#sensors_settings select'), function (i, e) {
			if ($(e).val() !== "") {
				arrSortSensors.push($(e).val());
			}
		});
		$('#sensors_settings select option').show();
		$.each(arrSortSensors, function (i, e) {
			$.each($('#sensors_settings select'), function (j, s) {
				if ($(s).val() !== e) {
					$('option[value="' + e + '"]', $(s)).hide();
				}
			});
		});
	});

	//Заполнение вкладки датчики данными после запроса
	function getSensors(data) {
		//console.log(data);
		$("#sensors_settings select").prop('selectedIndex',0);
		sensorsJson = data;
		let sensors = data;
		for (let key in sensors) {
			if (sensors.hasOwnProperty(key)) {
				let sensor_value = sensors[key]["value"];
				if (re_p.test(key)) {
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
					$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
				}
				if (re_t.test(key)) {
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
					if (sensor_value < 150) {
						$("#sensor_name_" + key).val(sensors[key]["name"]);
						$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
						$("#sensor_val_" + key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#svg_sensor_" + key).text(sensor_value.toFixed(1) + '°С');
						$("#svg_sensor_color_" + key).css('fill', jscolor);
					} else {
						if (!$.fn.objIsEmpty(sensors[key]["name"], false)) {
							$("#sensor_name_" + key).val("");
						}
						$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
						$("#sensor_val_" + key).text("").parent().find(".show").removeClass("show").addClass("hidden");
					}
				}
				if (re_out.test(key)) {
					$("#device_name_" + key).val(sensors[key]["name"]);
				}
				if (re_in.test(key)) {
					$("#safety_name_" + key).val(sensors[key]["name"]);
				}
			}
		}
		//localStorage.setObj('sensors', sensorsJson);
	}

	//Сохранение датчиков и их сортировки
	$(document).on('click', '#set_sensors', function (e) {
		e.preventDefault();
		let _this = $(this);
		let nameError = false;
		let arrSortSensors = [1, 2, 3, 4, 5, 6, 7, 8];
		let sensorsSend = {};
		for (let key in sensorsJson) {
			if (sensorsJson.hasOwnProperty(key)) {
				sensorsSend[key] = {};
				if (re_p.test(key)) {
					let color_val = $("#sensor_color_" + key).val();
					sensorsSend[key]["color"] = sensorsJson[key]["color"] = (color_val !== "FFFFFF" && color_val !== "") ? hex2dec(color_val) : 0;
					//console.log(key,color_val);
				}
				if (re_t.test(key)) {
					let color_val = $("#sensor_color_" + key).val();
					let sensor_val = $("#sensor_name_" + key).val();
					if (sensor_val === "" && sensorsJson[key]["value"] < 150) {
						nameError = true;
					}
					sensorsSend[key]["name"] = sensorsJson[key]["name"] = sensor_val;
					//console.log(key,color_val);
					sensorsSend[key]["color"] = sensorsJson[key]["color"] = (color_val !== "FFFFFF" && color_val !== "") ? hex2dec(color_val) : 0;

					//if (key !== "p1") {
					let sort_number = Number($("#sensor_number_" + key).val());
					if (sort_number !== 0) {
						$.fn.arrayUnset(arrSortSensors, sort_number);
					}
					sensorsSend[key]["number"] = sensorsJson[key]["number"] = sort_number;
					//}
				}
				if (re_out.test(key)) {
					sensorsSend[key]["name"] = sensorsJson[key]["name"] = $("#device_name_" + key).val();
				}
				if (re_in.test(key)) {
					sensorsSend[key]["name"] = sensorsJson[key]["name"] = $("#safety_name_" + key).val();
				}
			}
		}
		let count = 0;
		//console.log(arrSortSensors);
		if (arrSortSensors.length > 0) {
			for (let key in sensorsJson) {
				if (sensorsJson.hasOwnProperty(key) /*&& key !== "p1"*/) {
					if (sensorsSend[key]["number"] === 0) {
						sensorsSend[key]["number"] = sensorsJson[key]["number"] = arrSortSensors[count];
						count++;
					}
				}
			}
		}
		if (nameError) {
			$.fn.openModal('', '<p class="text-center text-danger text-strong">Заполните названия подключенных датчиков</p>', "modal-sm", true, false);
		} else {
			//console.log(sensorsJson,sensorsSend);
			sendRequest("sensorsInSet", sensorsSend, "json", setSensors, _this, $("#error_sensors"), false);
		}
	});

	function setSensors(data) {
		//console.log(data);
		sendRequest("sensorsOutSet", {}, "json", getSensors, $('#get_sensors'), $("#error_sensors"), '<p class="text-center text-success  text-strong">Настройки датчиков сохранены</p>');
		//$.fn.openModal('', '<p class="text-center text-success text-strong">Тестовое сообщение, УРА!</p>', "modal-sm", true, false);
		//localStorage.setObj('sensors', sensorsJson);
	}

	//Шаблоны
	const pressureTemplate =
		'<div class="row row-striped">' +
		'<div class="pt-10 clearfix">' +
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">Атмосферное давление</div>' +
		'<div class="col-xs-12 col-sm-3 text-center text-middle text-strong"><span id="reflux_pressure"></span> <span class="hidden">мм рт.ст.</span></div>' +
		'</div>' +
		//'<div class="row row-striped">' +
		'<div class="pb-10 clearfix">' +
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">t&#176 кипения спирта при данном давлении</div>' +
		'<div class="col-xs-12 col-sm-3 text-center text-middle text-strong"><span id="reflux_alco_boil"></span><span class="hidden">&#176С</span></div>' +
		'</div>' +
		'</div>';
	const deltaTempl =
		'<div class="input-group input-group-sm number-group">' +
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
		'<div class="row row-striped">' +
		'<div class="pt-10 pb-10 clearfix">' +
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">Мощность тена</div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="${id_value}"></span><span class="hidden">%</span></div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 col-centered">' +
		returnTplHtml([{id: "${id_set}", value: '0', min: '0', max: '100', step: '1'}], deltaTempl) +
		'</div>' +
		'</div>' +
		'</div>';
	const powerLowerTempl =
		'<div class="row row-striped">' +
		'<div class="pt-10 pb-10 clearfix">' +
		'<div class="col-xs-12 col-sm-7 text-center-xs text-middle text-strong">Рабочая мощность после прогрева куба</div>' +
		'<div class="col-xs-3 col-xs-offset-1 hidden-sm"></div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 col-centered">' +
		returnTplHtml([{id: "${id_lower_set}", value: '0', min: '0', max: '100', step: '1'}], deltaTempl) +
		'</div>' +
		'</div>' +
		'</div>';
	const pauseTempl =
		'<div class="row row-striped">' +
		'<div class="row-xs clearfix">${pause_thead}</div>' +
		'<div id="${id_step_bg}" class="pt-10 pb-10 clearfix">' +
		'<div id="${id_step_text}" class="col-xs-12 col-sm-3 text-center-xs text-middle text-strong">${pause_name}</div>' +
		'<div class="col-xs-5 col-xs-offset-0 col-sm-3 col-sm-offset-0 col-centered">' +
		returnTplHtml([{id: "${id_time}", value: "${value_time}", min: '0', max: '360', step: '1'}], deltaTempl) +
		'</div>' +
		'<div class="col-xs-5 col-xs-offset-0 col-sm-3 col-sm-offset-0 col-centered">' +
		returnTplHtml([{id: "${id_temperature}", value: "${value_temperature}", min: '0', max: '100', step: '1'}], deltaTempl) +
		'</div>' +
		'<div class="col-xs-2 col-xs-offset-0 col-sm-3 col-sm-offset-0 text-center text-middle">' +
		'<label class="checkbox-inline"><input class="noSwipe" ${checked_stop} id="${id_stop}" type="checkbox" value="Y"></label>'+
		'</div>' +
		'</div></div>'
	;
	//Привязка датчиков к процессу дистилляции, и запуск
	let distillationProcess = {"sensors": {}, "powerHigh": 0, "powerLower": 0, "start": false};
	$(document).on('click', '#distillation_add_sensor', function (e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("distillationSensorsSetLoad", {}, "json", selectSensorsDistillation, _this, $("#error_distillation"), false);
	});

	//Запрос датчиков для дистилляции и вывод их в диалоговое окно
	function selectSensorsDistillation(data) {
		let sensors = data;//sensorsJson
		//console.log(sensors);
		if (sensors !== null) {
			let section = '<section id="distillation_sensors" class="table-responsive"><table class="table table-noborder">';
			let tpl_temperature = '';
			let tpl_devices = '';
			let tpl_safety = '';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)/* && key !== "p1"*/) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					if (sensor_name !== "") {
						if (re_t.test(key)) {
							let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="distillation_radio_' + key + '" type="radio" value="Y">Отсечка</label>';
							let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";

							tpl_temperature += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon" style="background-color: #' + jscolor + '">' + key + '</span>' +
								'<input readonly id="distillation_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'<input type="hidden" id="distillation_color_' + key + '" value="' + jscolor + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td>' + sensor_cutoff + '</td>' +
								'</tr>';
						}
						if (re_out.test(key)) {
							tpl_devices += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon">' + key + '</span>' +
								'<input readonly id="distillation_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td></td>' +
								'<td></td>' +
								'</tr>';
						}
						if (re_in.test(key)) {
							tpl_safety += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon">' + key + '</span>' +
								'<input readonly id="distillation_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td></td>' +
								'<td></td>' +
								'</tr>';
						}
					}
				}
			}
			if (tpl_temperature !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики температуры</td></tr>' + tpl_temperature;
			}
			if (tpl_devices !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Исполнительные устройства</td></tr>' + tpl_devices;
			}
			if (tpl_safety !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики безопасности</td></tr>' + tpl_safety;
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для дистилляции', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						distillationProcess["sensors"] = {};
						let sensors_select = $('#distillation_sensors input[type=checkbox]');
						$.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								let key = $(e).data("sensor");
								let tmp = false;
								if (re_t.test(key)) {
									tmp = true;
								}
								let name = $("#distillation_name_" + key).val();
								let val_color = (tmp ? $("#distillation_color_" + key).val() : "");
								let color = (val_color !== "FFFFFF" && val_color !== "") ? Number(hex2dec(val_color)) : 0;
								let cutoff = (tmp ? Number($("#cutoff_" + key).prop("checked")) : 0);

								distillationProcess["sensors"][key] = {};
								if (tmp) {
									distillationProcess["sensors"][key] = {"name": name, "cutoff": cutoff, "color": color, "allertValue": 0, "value": 0,"member":1};
								} else {
									distillationProcess["sensors"][key] = {"name": name, "value": 0,"member":1};
								}
							}
						});
						$(this).closest(".modal").modal("hide");
						$.fn.pasteDistillationSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
		}
	}

	$(document).on('click', '#distillation_sensors input[type=checkbox]', function () {
		let checked = !$(this).prop("checked");
		let radio_cutoff = $("#cutoff_" + $(this).data("sensor"));
		radio_cutoff.prop("disabled", checked);
		if (checked) {
			radio_cutoff.prop("checked", false);
		}
	});

	$.fn.pasteDistillationSensors = function (sensors_select) {
		if ($.fn.objIsEmpty(globalSensorsJson, false)) {
			setTimeout(function () {
				//console.log("distillationTemplate",globalSensorsJson);
				$.fn.pasteDistillationSensors(false);
			}, 1000);
		}
		let sensorsDistillationSend = {
			"t1": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t2": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t3": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t4": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t5": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t6": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t7": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t8": {"name": "", "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"out1": {"name": "", "member": 0},
			"out2": {"name": "", "member": 0},
			"out3": {"name": "", "member": 0},
			"out4": {"name": "", "member": 0},
			"out5": {"name": "", "member": 0},
			"out6": {"name": "", "member": 0},
			"out7": {"name": "", "member": 0},
			"out8": {"name": "", "member": 0},
			"in1": {"name": "", "member": 0},
			"in2": {"name": "", "member": 0},
			"in3": {"name": "", "member": 0},
			"in4": {"name": "", "member": 0}
		};
		let distillationTemplate = '';
		let tpl_devices_body = '';
		let tpl_safety_body = '';
		if (!sensors_select && $.fn.objIsEmpty(distillationProcess["sensors"], false)) {
			$.ajax({
				url: ajax_url_debug + 'distillationSensorsGetTpl',
				data: {},
				type: 'GET',
				dataType: 'json',
				success: function (msg) {
					distillationProcess["sensors"] = msg;
					distillationProcess["powerHigh"] = Number(msg["powerHigh"]);
					distillationProcess["powerLower"] = Number(msg["powerLower"]);
					//console.log('distillationSensorsGetTpl',distillationProcess["sensors"]);
					if (!$.fn.objIsEmpty(distillationProcess["sensors"], false)) {
						$.fn.pasteDistillationSensors(false);
					}
				},
				error: function (err, exception) {
					alertAjaxError(err, exception, $("#error_distillation"));
				}
			});
		}
		if (!$.fn.objIsEmpty(distillationProcess["sensors"], false)) {
			let tpl_cutoff_thead =
				'<div class="row-xs clearfix">' +
				'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-xs-offset-3 col-sm-3 col-sm-offset-2 text-center text-middle text-primary">Отсечка</div>' +
				'</div>';
			let tpl_cutoff_body = '';
			let tpl_all_body = '';
			$.each(distillationProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = i;
				//let name_sensor = e["name"];
				if (e.hasOwnProperty("name") && sensorsDistillationSend[sensor_key].hasOwnProperty("name")) {
					sensorsDistillationSend[sensor_key]["name"] = e["name"];
				}
				if (re_t.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsDistillationSend[sensor_key]["color"] = e["color"];
					sensorsDistillationSend[sensor_key]["member"] = 1;

					let tpl_cutoff = '';
					if (e["cutoff"]) {
						sensorsDistillationSend[sensor_key]["cutoff"] = 1;
						tpl_cutoff = returnTplHtml([{id: "distillation_cutoff_" + sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
						tpl_cutoff_body +=
							'<div class="row row-striped">' + tpl_cutoff_thead +
							'<div id="distillation_alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
							'<div id="distillation_alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-middle text-center-xs text-strong">t&#176' + e["name"] + '</div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="distillation_' + sensor_key + '"></span><span' +
							' class="hidden">&#176С</span></div>' +
							//'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-3 col-sm-offset-2">' + tpl_cutoff +
							'</div>' +
							'</div>' +
							'</div>';

						tpl_cutoff_thead = '';
					}
					if (!e["delta"] && !e["cutoff"]) {
						tpl_all_body += '<div class="row row-striped">' +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-4 text-center-xs text-strong">t&#176' + e["name"] + '</div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="distillation_' + sensor_key + '"></span><span' +
							' class="hidden">&#176С</span></div>' +
							'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-3 col-sm-3"></div>' +
							'</div>' +
							'</div>';
					}
				}
				if (re_out.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsDistillationSend[sensor_key]["member"] = 1;
					tpl_devices_body += '<div class="row row-striped">' +
						'<div class="pt-10 pb-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">' + e["name"] + '</div>' +
						'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-strong">' +
						'<span id="distillation_' + sensor_key + '" class="box-green hidden"><span class="glyphicon">✔</span></span></div>' +
						'</div></div>';
				}
				if (re_in.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsDistillationSend[sensor_key]["member"] = 1;
					tpl_safety_body += '<div class="row row-striped">' +
						'<div id="distillation_alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
						'<div id="distillation_alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-center-xs text-strong">' + e["name"] + '</div>' +
						'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong">' +
						'<span id="distillation_' + sensor_key + '"></span> <span class="hidden"></span></div>' +
						'</div></div>';
				}
			});
			if (tpl_cutoff_body !== '') {
				distillationTemplate += tpl_cutoff_thead + tpl_cutoff_body;
			}
			if (tpl_all_body !== '') {
				distillationTemplate += tpl_all_body;
			}
		}
		if (distillationTemplate !== '') {
			//console.log(sensorsDistillationSend);
			if (sensors_select) {
				sendRequest("distillationSensorsSetSave", sensorsDistillationSend, "json", false, false, $("#error_distillation"), false);
				//console.log("distillationSensorsSetSave",sensorsDistillationSend);
			}
			let timeStepTemplate = '<div class="row row-striped">' +
				'<div id="distillation_step" class="col-xs-12 col-md-6 text-center-xs text-middle text-strong hidden"></div>' +
				'<div id="distillation_time" class="col-xs-12 col-md-6 text-center-xs text-middle text-strong hidden"></div>' +
				'</div>';
			distillationTemplate = timeStepTemplate +
				returnTplHtml([{id_value: "distillation_power_value", id_set: "distillation_power_set"}], powerTempl) +
				returnTplHtml([{id_lower_set: "distillation_power_lower_set"}], powerLowerTempl) +
				distillationTemplate + tpl_devices_body + tpl_safety_body;
			$("#distillation_start_group_button").removeClass("hidden");
		} else {
			$("#distillation_start_group_button").addClass("hidden");
		}
		$("#distillation_process").html(distillationTemplate);
		$("#distillation_power_set").val(distillationProcess["powerHigh"]);
		$("#distillation_power_lower_set").val(distillationProcess["powerLower"]);

		if (distillationProcess["start"] === true) {
			//console.log('distillationProcess');
			getDistillation();
			$('#distillation_start').prop("disabled", true);
			$('#distillation_add_sensor').prop("disabled", true);
		} else {
			$('#distillation_stop').prop("disabled", true);
			$('#distillation_add_sensor').prop("disabled", false);
		}
	};

	function launchDistillation(){
		distillationProcess["start"] = flagSendProcess = true;
		$('#distillation_start').prop("disabled", true);
		$('#distillation_add_sensor').prop("disabled", true);
		$('#distillation_stop').prop("disabled", false);
		clearInterval(sensorsProcessId);
		stopInterval();
		localStorage.setObj('oldStartProcess', 1);
		// setDistillation();
		setTimeout(function () {
			setDistillation();
		}, 1000);
	}
	$(document).on('start-event','#distillation_start', function(e) {
		console.log("start-event-Distillation");
		launchDistillation()
	});
	$(document).on('click', '#distillation_start', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Будет запущен процесс дистилляции, убедитесь в том, что тэн залит жидкостью!</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				launchDistillation();
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
	$(document).on('click', '#distillation_stop', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите остановить процесс дистилляции?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				stopDistillation()
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
	function stopDistillation(){
		$('#distillation_stop').prop("disabled", true);
		$('#distillation_add_sensor').prop("disabled", false);
		$('#distillation_start').prop("disabled", false);
		$("#svg_distillation_start").css('stroke', "#000000");
		$("#svg_distillation_alco_txt").hide();
		$("#svg_distillation_alco_val").hide().text("");
		$("#distillation_time").html('').addClass("hidden");
		$("#distillation_step").html('').addClass("hidden");
		$("#distillation_process").find("div.bg-danger").removeClass("bg-danger");
		$("#distillation_process").find("div.text-danger").removeClass("text-danger");
		clearInterval(sensorsProcessId);
		// clearInterval(sensorsIntervalId);
		stopInterval();
		flagSendProcess = true;
		globalSensorsJson["process"]["allow"] = 0;
		distillationProcess["start"] = false;
		setDistillation();
	}

	$(document).on('stop-event','#distillation_stop', function(e) {
		stopDistillation()
	});

	//Установка значений для дистилляции
	function setDistillation() {
		console.log("setDistillation");
		if ($.fn.objIsEmpty(distillationProcess["sensors"], false)){
			console.log("distillationProcess empty");
			setTimeout(function () {
				setDistillation();
			}, 1000);
		}else {
			let distillationSendData = {
				"process": {"allow": 0, "number": 0},
				"t1": {"allertValue": 0},
				"t2": {"allertValue": 0},
				"t3": {"allertValue": 0},
				"t4": {"allertValue": 0},
				"t5": {"allertValue": 0},
				"t6": {"allertValue": 0},
				"t7": {"allertValue": 0},
				"t8": {"allertValue": 0},
				"powerHigh": 0,
				"powerLower": 0
			};
			//let flag_send = false;
			let power_set = $("#distillation_power_set");
			let power_lower_set = $("#distillation_power_lower_set");
			distillationSendData["process"]["allow"] = (distillationProcess["start"] ? 1 : 0);
			if (distillationProcess["powerHigh"] !== Number(power_set.val())) {
				flagSendProcess = true;
			}
			if (distillationProcess["powerLower"] !== Number(power_lower_set.val())) {
				flagSendProcess = true;
			}
			distillationSendData["powerHigh"] = distillationProcess["powerHigh"] = Number(power_set.val());
			distillationSendData["powerLower"] = distillationProcess["powerLower"] = Number(power_lower_set.val());

			$.each(distillationProcess["sensors"], function (i, e) {
				let sensor_key = i;
				let distillation_cutoff = $("#distillation_cutoff_" + sensor_key);
				if (distillation_cutoff.length) {
					if (e["allertValue"] !== distillation_cutoff.val()) {
						flagSendProcess = true;
					}
					distillationSendData[sensor_key]["allertValue"] = e["allertValue"] = distillation_cutoff.val();
				}
			});
			if (flagSendProcess) {
				flagSendProcess = false;
				clearInterval(sensorsProcessId);
				// clearInterval(sensorsIntervalId);
				stopInterval();
				sendRequest("SensorsIn", distillationSendData, "json", startDistillation, false, $("#error_distillation"), false);
			}
		}
	}
	$(document).on('mousedown',"#distillation_process input", function () {
		flagSendProcess = true;
	});
	$(document).on('change',"#distillation_process input",
		$.debounce(function() {
			flagSendProcess = true;
			if(distillationProcess["start"] === true) {
				setDistillation();
			}
		}, 300)
	);
	function startDistillation() {
		console.log("startDistillation");
		setTimeout(function () {
			clearInterval(sensorsProcessId);
			// clearInterval(sensorsIntervalId);
			stopInterval();
			startInterval();
			// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
			if(distillationProcess["start"] === true) {
				sensorsProcessId = setInterval(getDistillation, 2000);
			}
		}, 2000);
	}

	function getDistillation() {
		console.log(flagSendProcess,"getDistillation");
		//let sek= parseInt(+new Date()/1000);
		//console.log(flagSendProcess,"getDistillation"+sek);
		if (!$.fn.objIsEmpty(globalSensorsJson, false)) {
			let dtoJson = {};
			dtoJson["h"] = Number(globalSensorsJson["power"]);
			dtoJson["t"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);
				let alert_value = Number(globalSensorsJson["sensors"][i][sensor_key]["allertValue"]);
				$.each(distillationProcess["sensors"], function (j, q) {
					if (j === sensor_key && re_t.test(sensor_key)) {
						q["value"] = sensor_value;
						let color_value = q["color"];
						let fillcolor = "#" + dec2hex(color_value);
						if (alert_value > 0 && sensor_value >= alert_value) {
							$("#distillation_alert_bg_" + sensor_key).addClass("bg-danger");
							$("#distillation_alert_text_" + sensor_key).addClass("text-danger");
						} else {
							$("#distillation_alert_bg_" + sensor_key).removeClass("bg-danger");
							$("#distillation_alert_text_" + sensor_key).removeClass("text-danger");
						}
						$("#svg_distillation_color_" + sensor_key).css('fill', colorPersent(fillcolor, sensor_value, alert_value));
						if (Number(q["member"]) !== 0) {
							dtoJson["t"][sensor_key] = sensor_value;
						}

						$("#distillation_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						//убрал пока
						/*if(!flagSendProcess) {
							$("#distillation_cutoff_" + sensor_key).val(alert_value.toFixed(0));
							//$("#distillation_temperature_" + sensor_key).val(temperature);
						}*/
						let allertValue = alert_value;
						allertValue = allertValue > 0 ? allertValue.toFixed(2) : "";
						if (allertValue !== "") {
							$("#distillation_cutoff_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
						}else{
							$("#distillation_cutoff_result_" + sensor_key).text(allertValue).parent().find(".show").removeClass("show").addClass("hidden");
						}
						//svg
						if(sensor_value < 150) {
							$("#svg_distillation_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
						}else{
							$("#svg_distillation_" + sensor_key).text('');
						}
					}
				});
			});
			//Исполнительные устройства
			$.each(globalSensorsJson["devices"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				//console.log(i,e);
				$.each(distillationProcess["sensors"], function (j, q) {
					if (j === sensor_key && re_out.test(sensor_key)) {
						if(Number(distillationProcess["sensors"][sensor_key]["member"]) !== 0) {
							if(Number(e[sensor_key]["allert"]) !== 0){
								$("#distillation_" + sensor_key).removeClass("hidden").addClass("show");
							}else{
								$("#distillation_" + sensor_key).removeClass("show").addClass("hidden");
							}
						}
					}
				})
			});
			//Датчики безопасности
			$.each(globalSensorsJson["safety"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				$.each(distillationProcess["sensors"], function (j, q) {
					if (j === sensor_key && re_in.test(sensor_key)) {
						if(Number(distillationProcess["sensors"][sensor_key]["member"]) !== 0) {
							if(Number(e[sensor_key]["allert"]) !== 0){
								$("#distillation_alert_bg_" + sensor_key).addClass("bg-danger");
								$("#distillation_alert_text_" + sensor_key).addClass("text-danger");
							} else {
								$("#distillation_alert_bg_" + sensor_key).removeClass("bg-danger");
								$("#distillation_alert_text_" + sensor_key).removeClass("text-danger");
							}
						}
					}
				})
			});
			let power_value = Number(globalSensorsJson["power"]);
			let power_higt_value = distillationProcess["powerHigh"];
			let power_lower_value = distillationProcess["powerLower"];
			//заполнение поля регулировки тена и рабочей мощности
			if(!flagSendProcess) {
				$("#distillation_power_set").val(power_higt_value.toFixed(0));
				$("#distillation_power_lower_set").val(power_lower_value.toFixed(0));
			}
			$("#distillation_power_value").text(power_value.toFixed(0)).parent().find(".hidden").removeClass("hidden").addClass("show");

			if(globalSensorsJson["process"]["step"] !== "") {
				let stepProcess = globalSensorsJson["process"]["step"];
				$("#distillation_step").html('Текущая операция: <span class="text-primary">' + stepProcess + '</span>').removeClass("hidden");
			}else{
				$("#distillation_step").html('').addClass("hidden");
			}
			if (Number(globalSensorsJson["process"]["time"])>0) {
				let timeProcess = secToTime(Number(globalSensorsJson["process"]["time"]));
				$("#distillation_time").html('Прошло времени: <span class="text-primary">'+timeProcess+'</span>').removeClass("hidden");
			}else{
				$("#distillation_time").html('').addClass("hidden");
			}

			$("#svg_distillation_ten_t").text(power_value.toFixed(0) + "%");
			$("#svg_distillation_color_ten").css('fill', colorPersent("#FF0000", power_value.toFixed(0), 100));
			$("#view_reflux_chart").html("");
			$("#view_mashing_chart").html("");
			$("#view_pid_chart").html("");
			if (!$.fn.objIsEmpty(dtoJson["t"], false) && drowChart){
				dtoReceiver.start(dtoJson, 'view_distillation_chart');
			}
		}
		if (distillationProcess["start"] === true) {
			$("#svg_distillation_start").css('stroke', "#02b500");
			if(Number(globalSensorsJson["cubeAlcohol"])>0){
				$("#svg_distillation_alco_txt").show();
				$("#svg_distillation_alco_val").show().text(globalSensorsJson["cubeAlcohol"] + "%");
			}else{
				$("#svg_distillation_alco_txt").hide();
				$("#svg_distillation_alco_val").hide().text("");
			}
		}
	}

	//Привязка датчиков к процессу ректификации, и запуск
	let refluxProcess = {
		"sensors": {},
		"stab": 0,
		"point": 0,
		"powerHigh": 0,
		"powerLower": 0,
		"number": 0,
		"start": false
	};//"devices":[],"safety":[],
	$(document).on('click', '#reflux_add_sensor', function (e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("refluxSensorsSetLoad", {}, "json", selectSensorsReflus, _this, $("#error_reflux"), false);
	});

	//Запрос датчиков для ректификации и вывод их в диалоговое окно
	function selectSensorsReflus(data) {
		let sensors = data;//sensorsJson
		//console.log(sensors);
		if (sensors !== null) {
			let section = '<section id="reflux_sensors" class="table-responsive"><table class="table table-noborder">';
			let tpl_temperature = '';
			let tpl_devices = '';
			let tpl_safety = '';
			let tpl_stab = '';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					if (sensor_name !== "") {
						if (re_t.test(key)) {
							let sensor_delta = '<label class="checkbox-inline"><input class="reflux_delta_radio" disabled id="delta_' + key + '" name="reflux_radio_' + key + '" type="radio"' +
								' value="Y">Уставка</label>';
							let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Отсечка</label>';

							let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";

							tpl_temperature += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon" style="background-color: #' + jscolor + '">' + key + '</span>' +
								'<input readonly id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'<input type="hidden" id="reflux_color_' + key + '" value="' + jscolor + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td>' + sensor_delta + '</td>' +
								'<td>' + sensor_cutoff + '</td>' +
								'</tr>';
						}
						if (re_out.test(key)) {
							tpl_devices += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon">' + key + '</span>' +
								'<input readonly id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td></td>' +
								'<td></td>' +
								'</tr>';
						}
						if (re_in.test(key)) {
							tpl_safety += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon">' + key + '</span>' +
								'<input readonly id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td></td>' +
								'<td></td>' +
								'</tr>';
						}
					}
					if(key === "stab"){
						tpl_stab += '<tr>'+
							'<td>Время стабилизации колонны</td>'+
							'<td colspan="3" class="text-center">' + returnTplHtml([{id: "stab", value: sensors[key], min: '0', max: '120', step: '1'}], deltaTempl) + '</td>'+
							'</tr>';
					}
					if(key === "point"){
						tpl_stab += '<tr>'+
							'<td>Время до применения уставки</td>'+
							'<td colspan="3" class="text-center">' + returnTplHtml([{id: "point", value: sensors[key], min: '0', max: '60', step: '1'}], deltaTempl) + '</td>'+
							'</tr>';
					}
				}
			}
			if (tpl_temperature !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики температуры</td></tr>' + tpl_temperature;
			}
			if (tpl_devices !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Исполнительные устройства</td></tr>' + tpl_devices;
			}
			if (tpl_safety !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики безопасности</td></tr>' + tpl_safety;
			}
			if (tpl_stab !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Настройки колонны</td></tr>' + tpl_stab;
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для ректификации', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						refluxProcess["sensors"] = {};
						let sensors_select = $('#reflux_sensors input[type=checkbox]');
						$.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								let key = $(e).data("sensor");
								let tmp = false;
								if (re_t.test(key)) {
									tmp = true;
								}
								let name = $("#reflux_name_" + key).val();
								let val_color = (tmp ? $("#reflux_color_" + key).val() : "");
								let color = (val_color !== "FFFFFF" && val_color !== "") ? Number(hex2dec(val_color)) : 0;
								let delta = (tmp ? Number($("#delta_" + key).prop("checked")) : 0);
								let cutoff = (tmp ? Number($("#cutoff_" + key).prop("checked")) : 0);

								refluxProcess["sensors"][key] = {};
								if (tmp) {
									refluxProcess["sensors"][key] = {"name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue": 0, "value": 0, "member":1};
								} else {
									refluxProcess["sensors"][key] = {"name": name, "value": 0,"member":1};
								}
							}
						});
						refluxProcess["stab"] = Number($("#stab").val());
						refluxProcess["point"] = Number($("#point").val());
						$(this).closest(".modal").modal("hide");
						$.fn.pasteRefluxSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
		}
	}
	$(document).on('click', '#reflux_sensors input[type=checkbox]', function () {
		let checked = !$(this).prop("checked");
		let radio_delta = $("#delta_" + $(this).data("sensor"));
		let radio_cutoff = $("#cutoff_" + $(this).data("sensor"));
		radio_delta.prop("disabled", checked);
		radio_cutoff.prop("disabled", checked);
		if (checked) {
			radio_delta.prop("checked", false);
			radio_cutoff.prop("checked", false);
		}
	});
	$(document).on('click', '#reflux_sensors input.reflux_delta_radio', function () {
		let checked = $(this).prop("checked");
		if(checked){
			$('#reflux_sensors input.reflux_delta_radio').prop("checked",false);
			$(this).prop("checked",true);
		}
	});

	$.fn.pasteRefluxSensors = function (sensors_select) {
		if ($.fn.objIsEmpty(globalSensorsJson, false)) {
			setTimeout(function () {
				$.fn.pasteRefluxSensors(false);
			}, 1000);
		}
		let sensorsRefluxSend = {
			"t1": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t2": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t3": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t4": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t5": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t6": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t7": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"t8": {"name": "", "delta": 0, "cutoff": 0, "color": 0, "member": 0, "priority": 0, "allertValue": 0},
			"out1": {"name": "", "member": 0},
			"out2": {"name": "", "member": 0},
			"out3": {"name": "", "member": 0},
			"out4": {"name": "", "member": 0},
			"out5": {"name": "", "member": 0},
			"out6": {"name": "", "member": 0},
			"out7": {"name": "", "member": 0},
			"out8": {"name": "", "member": 0},
			"in1": {"name": "", "member": 0},
			"in2": {"name": "", "member": 0},
			"in3": {"name": "", "member": 0},
			"in4": {"name": "", "member": 0},
			"stab": 0,
			"point": 0
		};
		let refluxTemplate = '';
		let tpl_devices_body = '';
		let tpl_safety_body = '';
		let tpl_stab = '';
		if (!sensors_select && $.fn.objIsEmpty(refluxProcess["sensors"], false)) {
			$.ajax({
				url: ajax_url_debug + 'refluxSensorsGetTpl',
				data: {},
				type: 'GET',
				dataType: 'json',
				success: function (msg) {
					refluxProcess["sensors"] = msg;
					refluxProcess["stab"] = Number(msg["stab"]);
					refluxProcess["point"] = Number(msg["point"]);
					refluxProcess["number"] = Number(msg["number"]);
					refluxProcess["powerHigh"] = Number(msg["powerHigh"]);
					refluxProcess["powerLower"] = Number(msg["powerLower"]);
					if (!$.fn.objIsEmpty(refluxProcess["sensors"], false)) {
						$.fn.pasteRefluxSensors(false);
					}
				},
				error: function (err, exception) {
					alertAjaxError(err, exception, $("#error_reflux"));
				}
			});
		}
		let flagout = false;
		if (!$.fn.objIsEmpty(refluxProcess["sensors"], false)) {
			let tpl_delta_thead =
				'<div class="row-xs clearfix">' +
				'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary">Дельта</div>' +
				'<div class="col-xs-4 col-sm-2 text-center text-middle text-primary">Уставка</div>' +
				'</div>';
			let tpl_delta_body = '';
			let tpl_cutoff_thead =
				'<div class="row-xs clearfix">' +
				'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-xs-offset-3 col-sm-3 col-sm-offset-2 text-center text-middle text-primary">Отсечка</div>' +
				'</div>';
			let tpl_cutoff_body = '';
			let tpl_all_body = '';

			tpl_stab = '<div class="row row-striped">' +
				'<div class="pt-10 clearfix">' +
				'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">Время стабилизации колонны</div>' +
				'<div class="col-xs-12 col-sm-3 text-center text-middle text-strong"><span id="reflux_stab"></span>' +
				refluxProcess["stab"] +
				' <span>мин.</span></div></div>' +
				'<div class="pt-10 clearfix">' +
				'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">Время до применения уставки</div>' +
				'<div class="col-xs-12 col-sm-3 text-center text-middle text-strong pb-10"><span id="reflux_point"></span>'+
				refluxProcess["point"] +
				' <span>мин.</span></div></div></div>';

			sensorsRefluxSend["stab"] = refluxProcess["stab"];
			sensorsRefluxSend["point"] = refluxProcess["point"];

			$.each(refluxProcess["sensors"], function (i, e) {
				let sensor_key = i;
				if (e.hasOwnProperty("name") && sensorsRefluxSend[sensor_key].hasOwnProperty("name")) {
					sensorsRefluxSend[sensor_key]["name"] = e["name"];
				}
				if (re_t.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["color"] = e["color"];
					sensorsRefluxSend[sensor_key]["member"] = 1;
					let tpl_delta = '';
					let tpl_delta_result = '';
					if (e["delta"]) {
						sensorsRefluxSend[sensor_key]["delta"] = 1;
						tpl_delta = returnTplHtml([{id: "reflux_delta_" + sensor_key, value: e["allertValue"], min: '0', max: '1', step: '0.05'}], deltaTempl);
						tpl_delta_result = '<span id="reflux_delta_result_' + sensor_key + '"></span><span class="hidden">&#176С</span>';
						tpl_delta_body +=
							'<div class="row row-striped">' + tpl_delta_thead +
							'<div id="reflux_alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
							'<div id="reflux_alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-middle text-center-xs text-strong">t&#176' + e["name"] + '</div>' +
							'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong text-nowrap">' +
							'<span id="reflux_' + sensor_key + '"></span><span class="hidden">&#176С</span></div>' +
							'<div class="col-xs-4 col-sm-3">' + tpl_delta + '</div>' +
							'<div class="col-xs-4 col-xs-offset-0 col-sm-2 col-sm-offset-0 text-center text-middle text-strong text-nowrap">' + tpl_delta_result +
							'</div>' +
							'</div>' +
							'</div>';

						tpl_delta_thead = '';
					}
					let tpl_cutoff = '';
					if (e["cutoff"]) {
						sensorsRefluxSend[sensor_key]["cutoff"] = 1;
						tpl_cutoff = returnTplHtml([{id: "reflux_cutoff_" + sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
						tpl_cutoff_body +=
							'<div class="row row-striped">' + tpl_cutoff_thead +
							'<div id="reflux_alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
							'<div id="reflux_alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-middle text-center-xs text-strong">t&#176' + e["name"] + '</div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="reflux_' + sensor_key + '"></span><span' +
							' class="hidden">&#176С</span></div>' +
							//'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-3 col-sm-offset-2">' + tpl_cutoff +
							'</div>' +
							'</div>' +
							'</div>';

						tpl_cutoff_thead = '';
					}
					if (!e["delta"] && !e["cutoff"]) {
						tpl_all_body += '<div class="row row-striped">' +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-4 text-center-xs text-strong">t&#176' + e["name"] + '</div>' +
							'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="reflux_' + sensor_key + '"></span><span class="hidden">&#176С</span></div>' +
							'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-4 col-sm-3"></div>' +
							'</div>' +
							'</div>';
					}
				}
				if (re_out.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["member"] = 1;
					//console.log(sensor_key);
					/*if(sensor_key === "out1") {
						flagout = true;
						let val_head_cycle = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][0]["head"]["timeCycle"] : 5);
						let tpl_head_cycle = returnTplHtml([{id: "reflux_head_cycle", value: val_head_cycle, min: '5', max: '30', step: '1'}], deltaTempl);
						let val_head_time = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][0]["head"]["timeOn"] : 1);
						let tpl_head_time = returnTplHtml([{id: "reflux_head_time", value: val_head_time, min: '1', max: '100', step: '0.5'}], deltaTempl);
						tpl_devices_body += '<div id="reflux_out_3" class="row' + visible_reflux_out_3 +'"><div class="col-xs-12">' +
							'<div class="row row-striped">' +
							head_devices +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-3 text-middle text-center-xs text-strong">Клапан отбора голов</div>' +
							//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong pb-10">Период</div>' +
							'<div class="col-xs-4 col-xs-offset-1_ col-sm-3 col-sm-offset-0_">' + tpl_head_cycle + '</div>' +
							//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong text-nowrap">Откр.%</div>' +
							'<div class="col-xs-4 col-xs-offset-1_ col-sm-3 col-sm-offset-1_">' + tpl_head_time + '</div>' +
							'</div></div>';

						let val_body_cycle = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["timeCycle"] : 5);
						let tpl_body_cycle = returnTplHtml([{id: "reflux_body_cycle", value: val_body_cycle, min: '5', max: '30', step: '1'}], deltaTempl);
						let val_body_time = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["timeOn"] : 0);
						let tpl_body_time = returnTplHtml([{id: "reflux_body_time", value: val_body_time, min: '0', max: '100', step: '0.5'}], deltaTempl);
						let val_body_decline = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["decline"] : 0);
						let tpl_body_decline = returnTplHtml([{id: "reflux_body_decline", value: val_body_decline, min: '0', max: '30', step: '1'}], deltaTempl);
							//(Number(globalSensorsJson["valwe"][1]["body"]["decline"]) > 0 ? " checked" : "");
						//if(flagout1){ head_devices = '';}
						tpl_devices_body += '<div class="row row-striped">' +
							//head_devices +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-3 pxs-10 text-middle text-center-xs text-strong">Клапан отбора тела</div>' +
							//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong pb-10">Период</div>' +
							'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_cycle + '</div>' +
							//'<div class="col-xs-4 col-sm-2 text-center-xs text-middle text-strong text-nowrap">Откр.%</div>' +
							'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_time + '</div>' +
							'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_decline + '</div>' +
							//'<div class="col-xs-2 col-sm-1 text-center text-middle"><label class="checkbox-inline">' +
							//'<input class="noSwipe" id="reflux_body_decline"' + checked_body_decline + ' type="checkbox" value="Y"></label></div>' +
							'</div></div>'+
							'</div></div>';
					}else if(sensor_key === "out2"){
						if(!flagout) {
							let val_head_cycle = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][0]["head"]["timeCycle"] : 5);
							let tpl_head_cycle = returnTplHtml([{id: "reflux_head_cycle", value: val_head_cycle, min: '5', max: '30', step: '1'}], deltaTempl);
							let val_head_time = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][0]["head"]["timeOn"] : 1);
							let tpl_head_time = returnTplHtml([{id: "reflux_head_time", value: val_head_time, min: '1', max: '100', step: '0.5'}], deltaTempl);
							tpl_devices_body += '<div id="reflux_out_3" class="row' + visible_reflux_out_3 +'"><div class="col-xs-12">' +
								'<div class="row row-striped">' +
								head_devices +
								'<div class="pt-10 pb-10 clearfix">' +
								'<div class="col-xs-12 col-sm-3 text-middle text-center-xs text-strong">Клапан отбора голов</div>' +
								//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong pb-10">Период</div>' +
								'<div class="col-xs-4 col-xs-offset-1_ col-sm-3 col-sm-offset-0_">' + tpl_head_cycle + '</div>' +
								//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong text-nowrap">Откр.%</div>' +
								'<div class="col-xs-4 col-xs-offset-1_ col-sm-3 col-sm-offset-1_">' + tpl_head_time + '</div>' +
								'</div></div>';

							let val_body_cycle = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["timeCycle"] : 5);
							let tpl_body_cycle = returnTplHtml([{id: "reflux_body_cycle", value: val_body_cycle, min: '5', max: '30', step: '1'}], deltaTempl);
							let val_body_time = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["timeOn"] : 1);
							let tpl_body_time = returnTplHtml([{id: "reflux_body_time", value: val_body_time, min: '0', max: '100', step: '0.5'}], deltaTempl);
							let val_body_decline = (globalSensorsJson.hasOwnProperty("valwe") ? globalSensorsJson["valwe"][1]["body"]["decline"] : 0);
							let tpl_body_decline = returnTplHtml([{id: "reflux_body_decline", value: val_body_decline, min: '0', max: '30', step: '1'}], deltaTempl);
							//(Number(globalSensorsJson["valwe"][1]["body"]["decline"]) > 0 ? " checked" : "");
							//if(flagout1){ head_devices = '';}
							tpl_devices_body += '<div class="row row-striped">' +
								//head_devices +
								'<div class="pt-10 pb-10 clearfix">' +
								'<div class="col-xs-12 col-sm-3 pxs-10 text-middle text-center-xs text-strong">Клапан отбора тела</div>' +
								//'<div class="col-xs-6 col-sm-2 text-center-xs text-middle text-strong pb-10">Период</div>' +
								'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_cycle + '</div>' +
								//'<div class="col-xs-4 col-sm-2 text-center-xs text-middle text-strong text-nowrap">Откр.%</div>' +
								'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_time + '</div>' +
								'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_decline + '</div>' +
								//'<div class="col-xs-2 col-sm-1 text-center text-middle"><label class="checkbox-inline">' +
								//'<input class="noSwipe" id="reflux_body_decline"' + checked_body_decline + ' type="checkbox" value="Y"></label></div>' +
								'</div></div>'+
								'</div></div>';
						}*/
					if(sensor_key === "out1" || sensor_key === "out2") {
						flagout = true;
					}else {
						tpl_devices_body += '<div class="row row-striped">' +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-4 text-center-xs text-middle text-strong">' + e["name"] + '</div>' +
							'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-strong">' +
							'<span id="reflux_' + sensor_key + '" class="box-green hidden"><span class="glyphicon">✔</span></span></div>' +
							'</div></div>';
					}
				}
				if (re_in.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["member"] = 1;
					tpl_safety_body += '<div class="row row-striped">' +
						'<div id="reflux_alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
						'<div id="reflux_alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-center-xs text-strong">' + e["name"] + '</div>' +
						'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="reflux_' + sensor_key + '"></span> <span class="hidden"></span></div>' +
						'</div></div>';
				}
			});
			if (tpl_delta_body !== '') {
				refluxTemplate += tpl_delta_thead + tpl_delta_body;
			}
			if (tpl_cutoff_body !== '') {
				refluxTemplate += tpl_cutoff_thead + tpl_cutoff_body;
			}
			if (tpl_all_body !== '') {
				refluxTemplate += tpl_all_body;
			}
		}
		if (refluxTemplate !== '') {
			if (sensors_select) {
				sendRequest("refluxSensorsSetSave", sensorsRefluxSend, "json", false, false, $("#error_reflux"), false);
			}
			let timeStepTemplate = '<div class="row row-striped">' +
				'<div id="reflux_step" class="col-xs-12 col-md-6 text-center-xs text-middle text-strong hidden"></div>' +
				'<div id="reflux_time" class="col-xs-12 col-md-6 text-center-xs text-middle text-strong hidden"></div>' +
				'</div>';
			let head_devices = '<div class="row-xs clearfix">' +
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-3 text-center text-middle text-primary text-nowrap">Период сек.</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">Открыт %</div>'+
				'<div id="reflux_devices_out_header_" class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">%&#8595;</div></div>';
			let head_devices_prima = '<div class="row-xs clearfix">' +
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-3 text-center text-middle text-primary text-nowrap">Период сек.</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">Открыт %</div>'+
				'<div id="reflux_devices_out_header_" class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap"></div></div>';
			let head_devices_steam = '<div class="row-xs clearfix">' +
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-3 text-center text-middle text-primary text-nowrap"></div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">Открыт %</div>'+
				'<div id="reflux_devices_out_header_" class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap"></div></div>';
			let body_devices = '<div class="row-xs clearfix">' +
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-3 text-center text-middle text-primary text-nowrap">Открыт %</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">Закрыт %</div>'+
				'<div id="reflux_devices_out_header_" class="col-xs-4 col-sm-3 text-center text-middle text-primary text-nowrap">%&#8595;</div></div>';
			let tpl_devices_out_body = '';
			if(flagout){
				let valwe_head = {};
				let valwe_headSteam = {};
				let valwe_body = {};
				let valwe_bodyPrima = {};
				if(globalSensorsJson.hasOwnProperty("valwe")){
					$.each(globalSensorsJson["valwe"], function (i, e) {
						// console.log(i,e);
						let valwe_key = Object.keys(e).shift();
						switch(valwe_key){
							case "head":
								valwe_head = e;
								break;
							case "headSteam":
								valwe_headSteam = e;
								break;
							case "body":
								valwe_body = e;
								break;
							case "bodyPrima":
								valwe_bodyPrima = e;
								break;
						}
					})
				}


				let visible_reflux_out = ((refluxProcess["number"] === 1 || refluxProcess["number"] === 2 || refluxProcess["number"] === 3) ? "" : " hidden");
				//головы жижа
				let val_head_cycle_rk = (valwe_head.hasOwnProperty("head") ? valwe_head["head"]["timeCycle"] : 5);
				let tpl_head_cycle_rk = returnTplHtml([{id: "reflux_head_cycle_rk", value: val_head_cycle_rk, min: '5', max: '30', step: '1'}], deltaTempl);
				let val_head_time_rk = (valwe_head.hasOwnProperty("head") ? valwe_head["head"]["timeOn"] : 1);
				let tpl_head_time_rk = returnTplHtml([{id: "reflux_head_time_rk", value: val_head_time_rk, min: '1', max: '100', step: '0.5'}], deltaTempl);
				//головы пар
				let val_head_steam = (valwe_headSteam.hasOwnProperty("headSteam") ? valwe_headSteam["headSteam"]["percent"] : 0);
				let tpl_head_steam = returnTplHtml([{id: "reflux_head_steam", value: val_head_steam, min: '0', max: '100', step: '1'}], deltaTempl);
				//головы прима (как у жижи, те же данные)
				let val_head_cycle_prima = (valwe_head.hasOwnProperty("head") ? valwe_head["head"]["timeCycle"] : 5);
				let tpl_head_cycle_prima = returnTplHtml([{id: "reflux_head_cycle_prima", value: val_head_cycle_prima, min: '5', max: '30', step: '1'}], deltaTempl);
				let val_head_time_prima = (valwe_head.hasOwnProperty("head") ? valwe_head["head"]["timeOn"] : 1);
				let tpl_head_time_prima = returnTplHtml([{id: "reflux_head_time_prima", value: val_head_time_prima, min: '1', max: '100', step: '0.5'}], deltaTempl);

				tpl_devices_out_body += '<div id="reflux_devices_out" class="row' + visible_reflux_out +'">' +
					'<div class="col-xs-12">';
				//тпл головы жижа
				tpl_devices_out_body +=
					'<div id="reflux_devices_head_rk" class="row row-striped">' +
					head_devices +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 text-middle text-center-xs text-strong">Клапан отбора голов</div>' +
					'<div class="col-xs-4 col-sm-3">' + tpl_head_cycle_rk + '</div>' +
					'<div class="col-xs-4 col-sm-3">' + tpl_head_time_rk + '</div>' +
					'</div></div>';
				//тпл головы пар
				tpl_devices_out_body +=
					'<div id="reflux_devices_head_steam" class="row row-striped">' +
					head_devices_steam +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 text-middle text-center-xs text-strong">Кран отбора голов</div>' +
					'<div class="col-xs-4 col-sm-3"></div>' +
					'<div class="col-xs-4 col-sm-3">' + tpl_head_steam + '</div>' +
					'</div></div>';
				//тпл головы прима
				tpl_devices_out_body +=
					'<div id="reflux_devices_head_prima" class="row row-striped">' +
					head_devices_prima +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 text-middle text-center-xs text-strong">Клапан отбора голов</div>' +
					'<div class="col-xs-4 col-sm-3">' + tpl_head_cycle_prima + '</div>' +
					'<div class="col-xs-4 col-sm-3">' + tpl_head_time_prima + '</div>' +
					'</div></div>';

				//тело жижа
				let val_body_cycle_rk = (valwe_body.hasOwnProperty("body") ? valwe_body["body"]["timeCycle"] : 5);
				let tpl_body_cycle_rk = returnTplHtml([{id: "reflux_body_cycle_rk", value: val_body_cycle_rk, min: '5', max: '30', step: '1'}], deltaTempl);
				let val_body_time_rk = (valwe_body.hasOwnProperty("body") ? valwe_body["body"]["timeOn"] : 0);
				let tpl_body_time_rk = returnTplHtml([{id: "reflux_body_time_rk", value: val_body_time_rk, min: '0', max: '100', step: '0.5'}], deltaTempl);
				let val_body_decline_rk = (valwe_body.hasOwnProperty("body") ? valwe_body["body"]["decline"] : 0);
				let tpl_body_decline_rk = returnTplHtml([{id: "reflux_body_decline_rk", value: val_body_decline_rk, min: '0', max: '30', step: '1'}], deltaTempl);
				//тело пар (как у примы, те же данные)
				let val_body_start_steam = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["percentStart"] : 0);
				let tpl_body_start_steam = returnTplHtml([{id: "reflux_body_start_steam", value: val_body_start_steam, min: '0', max: '100', step: '1'}], deltaTempl);
				let val_body_stop_steam = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["percentStop"] : 0);
				let tpl_body_stop_steam = returnTplHtml([{id: "reflux_body_stop_steam", value: val_body_stop_steam, min: '0', max: '100', step: '1'}], deltaTempl);
				let val_body_decline_steam = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["decline"] : 0);
				let tpl_body_decline_steam = returnTplHtml([{id: "reflux_body_decline_steam", value: val_body_decline_steam, min: '0', max: '30', step: '1'}], deltaTempl);
				//тело прима
				let val_body_start_prima = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["percentStart"] : 0);
				let tpl_body_start_prima = returnTplHtml([{id: "reflux_body_start_prima", value: val_body_start_prima, min: '0', max: '100', step: '1'}], deltaTempl);
				let val_body_stop_prima = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["percentStop"] : 0);
				let tpl_body_stop_prima = returnTplHtml([{id: "reflux_body_stop_prima", value: val_body_stop_prima, min: '0', max: '100', step: '1'}], deltaTempl);
				let val_body_decline_prima = (valwe_bodyPrima.hasOwnProperty("bodyPrima") ? valwe_bodyPrima["bodyPrima"]["decline"] : 0);
				let tpl_body_decline_prima = returnTplHtml([{id: "reflux_body_decline_prima", value: val_body_decline_prima, min: '0', max: '30', step: '1'}], deltaTempl);
				//тпл тело жижа
				tpl_devices_out_body += '<div id="reflux_devices_body_rk" class="row row-striped">' +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 pxs-10 text-middle text-center-xs text-strong">Клапан отбора тела</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_cycle_rk + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_time_rk + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_decline_rk + '</div>' +
					'</div></div>';
				//тпл тело пар
				tpl_devices_out_body += '<div id="reflux_devices_body_steam" class="row row-striped">' +
					body_devices +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 pxs-10 text-middle text-center-xs text-strong">Кран отбора тела</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_start_steam + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_stop_steam + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_decline_steam + '</div>' +
					'</div></div>';
				//тпл тело прима
				tpl_devices_out_body += '<div id="reflux_devices_body_prima" class="row row-striped">' +
					body_devices +
					'<div class="pt-10 pb-10 clearfix">' +
					'<div class="col-xs-12 col-sm-3 pxs-10 text-middle text-center-xs text-strong">Кран отбора тела</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_start_prima + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_stop_prima + '</div>' +
					'<div class="col-xs-4 col-sm-3 pxs-0">' + tpl_body_decline_prima + '</div>' +
					'</div></div>';

				tpl_devices_out_body += '</div></div>';

				tpl_devices_body = tpl_devices_out_body + tpl_devices_body;
			}

			refluxTemplate = timeStepTemplate +
				returnTplHtml([{id_value: "reflux_power_value", id_set: "reflux_power_set"}], powerTempl) +
				returnTplHtml([{id_lower_set: "reflux_power_lower_set"}], powerLowerTempl) +
				tpl_stab + refluxTemplate + pressureTemplate + tpl_devices_body + tpl_safety_body;

			$("#reflux_start_group_button").removeClass("hidden");
		} else {
			$("#reflux_start_group_button").addClass("hidden");
		}
		$("#reflux_process").html(refluxTemplate);
		$.fn.clearSelect($("#reflux_algorithm_select"));
		$.fn.fillSelect($("#reflux_algorithm_select"), algorithmReflux, false);
		$("#reflux_algorithm_select").val(refluxProcess["number"]).change();
		$("#reflux_algorithm").removeClass("hidden");
		$("#reflux_power_set").val(refluxProcess["powerHigh"]);
		$("#reflux_power_lower_set").val(refluxProcess["powerLower"]);
		if (refluxProcess["start"] === true) {
			getReflux();
			$('#reflux_start').prop("disabled", true);
			$('#reflux_add_sensor').prop("disabled", true);
			$("#reflux_algorithm_select option[value=" + refluxProcess["number"] + "]").prop('selected',true);
			$('#reflux_algorithm_select').prop("disabled", true);
		} else {
			$('#reflux_add_sensor').prop("disabled", false);
			$('#reflux_stop').prop("disabled", true);
			$('#reflux_next').prop("disabled", true);
			$('#reflux_algorithm_select').prop("disabled", false);
		}
	};
	$(document).on('change', '#reflux_algorithm_select', function () {
		let algorithm_val = Number($(this).find(":selected").val());
		refluxProcess["number"] = algorithm_val;
		if($("#reflux_devices_out").length > 0) {
			console.log("reflux_devices_out", algorithm_val);
			if (algorithm_val === 1 || algorithm_val === 2 || algorithm_val === 3) {
				$("#reflux_devices_out").removeClass("hidden");
			}else{
				$("#reflux_devices_out").addClass("hidden");
			}
			if (algorithm_val === 1) {
				$("#reflux_devices_head_rk").addClass("hidden");
				$("#reflux_devices_body_rk").addClass("hidden");
				$("#reflux_devices_head_steam").addClass("hidden");
				$("#reflux_devices_body_steam").addClass("hidden");
				$("#reflux_devices_head_prima").removeClass("hidden");
				$("#reflux_devices_body_prima").removeClass("hidden");
				// $("#reflux_devices_out_header").addClass("hidden");
			}else if (algorithm_val === 2){
				$("#reflux_devices_head_rk").addClass("hidden");
				$("#reflux_devices_body_rk").addClass("hidden");
				$("#reflux_devices_head_steam").removeClass("hidden");
				$("#reflux_devices_body_steam").removeClass("hidden");
				$("#reflux_devices_head_prima").addClass("hidden");
				$("#reflux_devices_body_prima").addClass("hidden");
				// $("#reflux_devices_out_header").removeClass("hidden");
			}else if (algorithm_val === 3){
				$("#reflux_devices_head_rk").removeClass("hidden");
				$("#reflux_devices_body_rk").removeClass("hidden");
				$("#reflux_devices_head_steam").addClass("hidden");
				$("#reflux_devices_body_steam").addClass("hidden");
				$("#reflux_devices_head_prima").addClass("hidden");
				$("#reflux_devices_body_prima").addClass("hidden");
				// $("#reflux_devices_out_header").removeClass("hidden");
			}
		}
	});

	function launchReflux(){
		refluxProcess["start"] = flagSendProcess = true;
		$('#reflux_start').prop("disabled", true);
		$('#reflux_add_sensor').prop("disabled", true);
		$('#reflux_stop').prop("disabled", false);
		$('#reflux_next').prop("disabled", false);
		$("#reflux_algorithm_select option[value=" + refluxProcess["number"] + "]").prop('selected',true);
		$('#reflux_algorithm_select').prop("disabled", true);
		clearInterval(sensorsProcessId);
		stopInterval();
		localStorage.setObj('oldStartProcess', 2);
		setTimeout(function () {
			setReflux();
		}, 1000);
	}
	$(document).on('start-event','#reflux_start', function(e) {
		console.log("start-event-Reflux");
		launchReflux()
	});
	$(document).on('click', '#reflux_start', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Будет запущен процесс ректификации, убедитесь в том, что тэн залит жидкостью!</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				launchReflux();
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
	$(document).on('click', '#reflux_stop', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите остановить процесс ректификации?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				stopReflux()
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
	$(document).on('click', '#reflux_next', function () {
		let step = globalSensorsJson["process"]["step"];
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите прервать «' + step+ '» и перейти на следующий шаг?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				stepRefluxNext = true;
				setReflux();
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
	function stopReflux(){
		$('#reflux_stop').prop("disabled", true);
		$('#reflux_next').prop("disabled", true);
		$('#reflux_add_sensor').prop("disabled", false);
		$('#reflux_start').prop("disabled", false);
		$('#reflux_algorithm_select').prop("disabled", false);
		$("#svg_reflux_start").css('stroke', "#000000");
		$("#svg_reflux_alco_txt").hide();
		$("#svg_reflux_alco_val").hide().text("");
		$("#reflux_time").html('').addClass("hidden");
		$("#reflux_step").html('').addClass("hidden");
		$("#reflux_process").find("div.bg-danger").removeClass("bg-danger");
		$("#reflux_process").find("div.text-danger").removeClass("text-danger");
		clearInterval(sensorsProcessId);
		// clearInterval(sensorsIntervalId);
		stopInterval();
		flagSendProcess = true;
		globalSensorsJson["process"]["allow"] = 0;
		refluxProcess["start"] = false;
		setReflux();
	}

	$(document).on('stop-event','#reflux_stop', function(e) {
		stopReflux()
	});

	//Установка значений для ректификации
	let stepRefluxNext = false;
	function setReflux() {
		console.log("setReflux", stepRefluxNext);
		if ($.fn.objIsEmpty(refluxProcess["sensors"], false)){
			setTimeout(function () {
				setReflux();
			}, 1000);
		}else {
			let refluxSendData = {
				"process": {"allow": 0, "number": 0},
				"t1": {"allertValue": 0},
				"t2": {"allertValue": 0},
				"t3": {"allertValue": 0},
				"t4": {"allertValue": 0},
				"t5": {"allertValue": 0},
				"t6": {"allertValue": 0},
				"t7": {"allertValue": 0},
				"t8": {"allertValue": 0},
				"head":{"timeCycle": 0,"timeOn": 0},
				"headSteam":{"percent":0},
				"body":{"timeCycle": 0,"timeOn": 0,"decline": 0},
				"bodyPrima":{"percentStart":0,"percentStop":0,"decline":0},
				"powerHigh": 0,
				"powerLower": 0,
				"stepNext": 0
			};
			let power_set = $("#reflux_power_set");
			let power_lower_set = $("#reflux_power_lower_set");
			refluxSendData["process"]["allow"] = (refluxProcess["start"] ? 2 : 0);
			if (refluxProcess["powerHigh"] !== power_set.val()) {
				flagSendProcess = true;
			}
			if (refluxProcess["powerLower"] !== power_lower_set.val()) {
				flagSendProcess = true;
			}
			refluxSendData["process"]["number"] = refluxProcess["number"];
			refluxSendData["powerHigh"] = refluxProcess["powerHigh"] = Number(power_set.val());
			refluxSendData["powerLower"] = refluxProcess["powerLower"] = Number(power_lower_set.val());

			$.each(refluxProcess["sensors"], function (i, e) {
				let sensor_key = i;
				let reflux_delta = $("#reflux_delta_" + sensor_key);
				let reflux_cutoff = $("#reflux_cutoff_" + sensor_key);
				if (reflux_delta.length) {
					if (e["allertValue"] !== reflux_delta.val()) {
						flagSendProcess = true;
					}
					refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_delta.val();
				}
				if (reflux_cutoff.length) {
					if (e["allertValue"] !== reflux_cutoff.val()) {
						flagSendProcess = true;
					}
					refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_cutoff.val();
				}
			});
			//клапана
			let valwe_head = {};
			let valwe_headSteam = {};
			let valwe_body = {};
			let valwe_bodyPrima = {};
			if(globalSensorsJson.hasOwnProperty("valwe")) {
				$.each(globalSensorsJson["valwe"], function (i, e) {
					// console.log(i, e);
					let valwe_key = Object.keys(e).shift();
					switch (valwe_key) {
						case "head":
							valwe_head = e;
							break;
						case "headSteam":
							valwe_headSteam = e;
							break;
						case "body":
							valwe_body = e;
							break;
						case "bodyPrima":
							valwe_bodyPrima = e;
							break;
					}
				});

				//прима
				if (Number(refluxSendData["process"]["number"]) === 1) {
					let reflux_head_cycle_prima = $("#reflux_head_cycle_prima");
					let reflux_head_time_prima = $("#reflux_head_time_prima");
					let reflux_body_start_prima = $("#reflux_body_start_prima");
					let reflux_body_stop_prima = $("#reflux_body_stop_prima");
					let reflux_body_decline_prima = $("#reflux_body_decline_prima");
					if (reflux_head_cycle_prima.length) {
						let val_reflux_head_cycle_prima = Number(reflux_head_cycle_prima.val());
						if (Number(valwe_head["head"]["timeCycle"]) !== val_reflux_head_cycle_prima) {
							flagSendProcess = true;
						}
						refluxSendData["head"]["timeCycle"] = val_reflux_head_cycle_prima;
					}
					if (reflux_head_time_prima.length) {
						let val_reflux_head_time_prima = Number(reflux_head_time_prima.val());
						if (Number(valwe_head["head"]["timeOn"]) !== val_reflux_head_time_prima) {
							flagSendProcess = true;
						}
						refluxSendData["head"]["timeOn"] = val_reflux_head_time_prima;
					}
					if (reflux_body_start_prima.length) {
						let val_reflux_body_start_prima = Number(reflux_body_start_prima.val());
						if (Number(valwe_bodyPrima["bodyPrima"]["percentStart"]) !== val_reflux_body_start_prima) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["percentStart"] = val_reflux_body_start_prima;
					}
					if (reflux_body_stop_prima.length) {
						let val_reflux_body_stop_prima = Number(reflux_body_stop_prima.val());
						if (Number(valwe_bodyPrima["bodyPrima"]["percentStop"]) !== val_reflux_body_stop_prima) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["percentStop"] = val_reflux_body_stop_prima;
					}
					if (reflux_body_decline_prima.length) {
						let val_reflux_body_decline_prima = Number(reflux_body_decline_prima.val());
						if (Number(refluxSendData["bodyPrima"]["decline"]) !== val_reflux_body_decline_prima) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["decline"] = val_reflux_body_decline_prima;
					}
				}
				//пар
				if (Number(refluxSendData["process"]["number"]) === 2) {
					let reflux_head_steam = $("#reflux_head_steam");
					let reflux_body_start_steam = $("#reflux_body_start_steam");
					let reflux_body_stop_steam = $("#reflux_body_stop_steam");
					let reflux_body_decline_steam = $("#reflux_body_decline_steam");
					if (reflux_head_steam.length) {
						let val_reflux_head_steam = Number(reflux_head_steam.val());
						if (Number(valwe_headSteam["headSteam"]["percent"]) !== val_reflux_head_steam) {
							flagSendProcess = true;
						}
						refluxSendData["headSteam"]["percent"] = val_reflux_head_steam;
					}
					if (reflux_body_start_steam.length) {
						let val_reflux_body_start_steam = Number(reflux_body_start_steam.val());
						if (Number(valwe_bodyPrima["bodyPrima"]["percentStart"]) !== val_reflux_body_start_steam) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["percentStart"] = val_reflux_body_start_steam;
					}
					if (reflux_body_stop_steam.length) {
						let val_reflux_body_stop_steam = Number(reflux_body_stop_steam.val());
						if (Number(valwe_bodyPrima["bodyPrima"]["percentStop"]) !== val_reflux_body_stop_steam) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["percentStop"] = val_reflux_body_stop_steam;
					}
					if (reflux_body_decline_steam.length) {
						let val_reflux_body_decline_steam = Number(reflux_body_decline_steam.val());
						if (Number(refluxSendData["bodyPrima"]["decline"]) !== val_reflux_body_decline_steam) {
							flagSendProcess = true;
						}
						refluxSendData["bodyPrima"]["decline"] = val_reflux_body_decline_steam;
					}
				}
				//жижа
				if (Number(refluxSendData["process"]["number"]) === 3) {
					let reflux_head_cycle_rk = $("#reflux_head_cycle_rk");
					let reflux_head_time_rk = $("#reflux_head_time_rk");
					let reflux_body_cycle_rk = $("#reflux_body_cycle_rk");
					let reflux_body_time_rk = $("#reflux_body_time_rk");
					let reflux_body_decline_rk = $("#reflux_body_decline_rk");
					if (reflux_head_cycle_rk.length) {
						let val_reflux_head_cycle_rk = Number(reflux_head_cycle_rk.val());
						if (Number(valwe_head["head"]["timeCycle"]) !== val_reflux_head_cycle_rk) {
							flagSendProcess = true;
						}
						refluxSendData["head"]["timeCycle"] = val_reflux_head_cycle_rk;
					}
					if (reflux_head_time_rk.length) {
						let val_reflux_head_time_rk = Number(reflux_head_time_rk.val());
						if (Number(valwe_head["head"]["timeOn"]) !== val_reflux_head_time_rk) {
							flagSendProcess = true;
						}
						refluxSendData["head"]["timeOn"] = val_reflux_head_time_rk;
					}
					if (reflux_body_cycle_rk.length) {
						let val_reflux_body_cycle_rk = Number(reflux_body_cycle_rk.val());
						if (Number(valwe_body["body"]["timeCycle"]) !== val_reflux_body_cycle_rk) {
							flagSendProcess = true;
						}
						refluxSendData["body"]["timeCycle"] = val_reflux_body_cycle_rk;
					}
					if (reflux_body_time_rk.length) {
						let val_reflux_body_time_rk = Number(reflux_body_time_rk.val());
						if (Number(valwe_body["body"]["timeOn"]) !== val_reflux_body_time_rk) {
							flagSendProcess = true;
						}
						refluxSendData["body"]["timeOn"] = val_reflux_body_time_rk;
					}
					if (reflux_body_decline_rk.length) {
						let val_reflux_body_decline_rk = Number(reflux_body_decline_rk.val());
						if (Number(valwe_body["body"]["decline"]) !== val_reflux_body_decline_rk) {
							flagSendProcess = true;
						}
						refluxSendData["body"]["decline"] = val_reflux_body_decline_rk;
					}
				}
			}
			if(stepRefluxNext){
				flagSendProcess = true;
				refluxSendData["stepNext"] = 1;
				stepRefluxNext = false;
			}

			if (flagSendProcess) {
				flagSendProcess = false;
				clearInterval(sensorsProcessId);
				// clearInterval(sensorsIntervalId);
				stopInterval();
				sendRequest("SensorsIn", refluxSendData, "json", startReflux, false, $("#error_reflux"), false);
			}
		}
	}
	$(document).on('mousedown',"#reflux_process input", function () {
		flagSendProcess = true;
	});
	$(document).on('change',"#reflux_process input",
		$.debounce(function() {
			flagSendProcess = true;
			if(refluxProcess["start"] === true) {
				setReflux();
			}
		}, 300)
	);
	function startReflux() {
		console.log("startReflux");
		setTimeout(function () {
			clearInterval(sensorsProcessId);
			// clearInterval(sensorsIntervalId);
			stopInterval();
			startInterval();
			// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
			if(refluxProcess["start"] === true) {
				sensorsProcessId = setInterval(getReflux, 2000);
			}
		}, 2000);
	}

	function getReflux() {
		if (!$.fn.objIsEmpty(globalSensorsJson, false)) {
			let dtoJson = {};
			dtoJson["h"] = Number(globalSensorsJson["power"]);
			dtoJson["t"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);
				let alert_value = Number(globalSensorsJson["sensors"][i][sensor_key]["allertValue"]);
				$.each(refluxProcess["sensors"], function (j, q) {
					if (j === sensor_key && re_t.test(sensor_key)) {
						q["value"] = sensor_value;
						let color_value = q["color"];
						let fillcolor = "#" + dec2hex(color_value);
						if (alert_value > 0 && sensor_value >= alert_value) {
							$("#reflux_alert_bg_" + sensor_key).addClass("bg-danger");
							$("#reflux_alert_text_" + sensor_key).addClass("text-danger");
						} else {
							$("#reflux_alert_bg_" + sensor_key).removeClass("bg-danger");
							$("#reflux_alert_text_" + sensor_key).removeClass("text-danger");
						}
						if (q["delta"] === false) {
							$("#svg_reflux_color_" + sensor_key).css('fill', colorPersent(fillcolor, sensor_value, alert_value));
						} else {
							let delta_alert = $("#reflux_delta_" + sensor_key).val();
							let delta_value = (delta_alert - alert_value + sensor_value).toFixed(2);
							$("#svg_reflux_color_" + sensor_key).css('fill', colorPersent(fillcolor, delta_value, delta_alert));
						}
						//убрал пока
						/*if(!flagSendProcess) {
							//$("#reflux_delta_" + sensor_key).val(alert_value);
							$("#reflux_cutoff_" + sensor_key).val(alert_value.toFixed(0));
						}*/
						$("#reflux_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						let allertValue = alert_value;
						allertValue = allertValue > 0 ? allertValue.toFixed(2) : "";
						if (allertValue !== "") {
							$("#reflux_delta_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
							$("#reflux_cutoff_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
						}else{
							$("#reflux_delta_result_" + sensor_key).text(allertValue).parent().find(".show").removeClass("show").addClass("hidden");
							$("#reflux_cutoff_result_" + sensor_key).text(allertValue).parent().find(".show").removeClass("show").addClass("hidden");
						}
						//svg
						if(sensor_value < 150) {
							$("#svg_reflux_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
						}else{
							$("#svg_reflux_" + sensor_key).text('');
						}

						if (Number(q["member"]) !== 0) {
							dtoJson["t"][sensor_key] = sensor_value;
						}
					}
				});
				//Исполнительные устройства
				$.each(globalSensorsJson["devices"], function (i, e) {
					let sensor_key = Object.keys(e).shift();
					$.each(refluxProcess["sensors"], function (j, q) {
						if (j === sensor_key && re_out.test(sensor_key)) {
							if(Number(refluxProcess["sensors"][sensor_key]["member"]) !== 0) {
								if(Number(e[sensor_key]["allert"]) !== 0){
									$("#reflux_" + sensor_key).removeClass("hidden").addClass("show");
								}else{
									$("#reflux_" + sensor_key).removeClass("show").addClass("hidden");
								}
							}
						}
					})
				});
				//Датчики безопасности
				$.each(globalSensorsJson["safety"], function (i, e) {
					let sensor_key = Object.keys(e).shift();
					$.each(refluxProcess["sensors"], function (j, q) {
						if (j === sensor_key && re_in.test(sensor_key)) {
							if(Number(refluxProcess["sensors"][sensor_key]["member"]) !== 0) {
								if(Number(e[sensor_key]["allert"]) !== 0){
									$("#reflux_alert_bg_" + sensor_key).addClass("bg-danger");
									$("#reflux_alert_text_" + sensor_key).addClass("text-danger");
								} else {
									$("#reflux_alert_bg_" + sensor_key).removeClass("bg-danger");
									$("#reflux_alert_text_" + sensor_key).removeClass("text-danger");
								}
							}
						}
					})
				});

				if (re_p.test(sensor_key)) {
					$("#reflux_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					dtoJson["p"] = globalSensorsJson["sensors"][i]["p1"]["value"];
				}

			});
			$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			let power_value = Number(globalSensorsJson["power"]);
			let power_higt_value = refluxProcess["powerHigh"];
			let power_lower_value = refluxProcess["powerLower"];
			//заполнение поля регулировки тена и рабочей мощности
			if(!flagSendProcess) {
				$("#reflux_power_set").val(power_higt_value.toFixed(0));
				$("#reflux_power_lower_set").val(power_lower_value.toFixed(0));
			}
			$("#reflux_power_value").text(power_value.toFixed(0)).parent().find(".hidden").removeClass("hidden").addClass("show");


			if(globalSensorsJson["process"]["step"] !== "") {
				let stepProcess = globalSensorsJson["process"]["step"];
				$("#reflux_step").html('Текущая операция: <span class="text-primary">' + stepProcess + '</span>').removeClass("hidden");
			}else{
				$("#reflux_step").html('').addClass("hidden");
			}
			if (Number(globalSensorsJson["process"]["time"])>0) {
				let timeProcess = secToTime(Number(globalSensorsJson["process"]["time"]));
				$("#reflux_time").html('Прошло времени: <span class="text-primary">'+timeProcess+'</span>').removeClass("hidden");
			}else {
				$("#reflux_time").html('').addClass("hidden");
			}

			$("#svg_reflux_ten_t").text(power_value.toFixed(0) + "%");
			$("#svg_reflux_color_ten").css('fill', colorPersent("#FF0000", power_value.toFixed(0), 100));
			$("#view_distillation_chart").html("");
			$("#view_mashing_chart").html("");
			$("#view_pid_chart").html("");
			if (!$.fn.objIsEmpty(dtoJson["t"], false) && drowChart){
				dtoReceiver.start(dtoJson, 'view_reflux_chart');
			}
		}
		if (refluxProcess["start"] === true) {
			$("#svg_reflux_start").css('stroke', "#02b500");
			if(Number(globalSensorsJson["cubeAlcohol"])>0){
				$("#svg_reflux_alco_txt").show();
				$("#svg_reflux_alco_val").show().text(globalSensorsJson["cubeAlcohol"] + "%");
			}else{
				$("#svg_reflux_alco_txt").hide();
				$("#svg_reflux_alco_val").hide().text("");
			}
		}
	}

	//Привязка датчиков к процессу затирания, и запуск
	let mashingProcess = {"sensors": {}, "power": 0, "start": false};
	$(document).on('click', '#mashing_add_sensor', function (e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("mashingSensorsSetLoad", {}, "json", selectSensorsMashing, _this, $("#error_mashing"), false);
	});

	//Запрос датчиков для затирания и вывод их в диалоговое окно
	function selectSensorsMashing(data) {
		let sensors = data;
		//console.log(sensors);
		if (sensors !== null) {
			let section = '<section id="mashing_sensors" class="table-responsive"><table class="table table-noborder">';
			let tpl_temperature = '';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)/* && key !== "p1"*/) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					if (sensor_name !== "") {
						if (re_t.test(key)) {
							let sensor_priority = '<label class="checkbox-inline"><input disabled id="priority_' + key + '" name="mashing_radio[]" type="radio" value="Y">Приоритет</label>';
							let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";

							tpl_temperature += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon" style="background-color: #' + jscolor + '">' + key + '</span>' +
								'<input readonly id="mashing_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'<input type="hidden" id="mashing_color_' + key + '" value="' + jscolor + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td>' + sensor_priority + '</td>' +
								'</tr>';
						}
					}
				}
			}
			if (tpl_temperature !== '') {
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики температуры</td></tr>' + tpl_temperature;
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для затирания', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						mashingProcess["sensors"] = {};
						let sensors_select = $('#mashing_sensors input[type=checkbox]');
						$.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								let key = $(e).data("sensor");
								let tmp = false;
								if (re_t.test(key)) {
									tmp = true;
								}
								let name = $("#mashing_name_" + key).val();
								let val_color = (tmp ? $("#mashing_color_" + key).val() : "");
								let color = (val_color !== "FFFFFF" && val_color !== "") ? Number(hex2dec(val_color)) : 0;
								let priority = (tmp ? $("#priority_" + key).prop("checked") : false);

								mashingProcess["sensors"][key] = {};
								if (tmp) {
									if(key === "t2"){
										name = "Датчик в струе";
									}
									mashingProcess["sensors"][key] = {"name": name, "priority": (priority ? 1 : 0), "color": color,"member":1};
								}
							}
						});
						if (!$.fn.objIsEmpty(mashingProcess["sensors"], false)) {
							mashingProcess["sensors"]["pause1"] = {"name":"Кислотная пауза","time":20,"temperature":40,"stop":0};
							mashingProcess["sensors"]["pause2"] = {"name":"Белковая пауза","time":20,"temperature":55,"stop":0};
							mashingProcess["sensors"]["pause3"] = {"name":"Осахаривание","time":30,"temperature":63,"stop":0};
							mashingProcess["sensors"]["pause4"] = {"name":"Осахаривание","time":60,"temperature":67,"stop":0};
							mashingProcess["sensors"]["pause5"] = {"name":"Мэш аут","time":10,"temperature":78,"stop":0};

						}
						$(this).closest(".modal").modal("hide");
						$.fn.pasteMashingSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
		}
	}

	$(document).on('click', '#mashing_sensors input[type=checkbox]', function () {
		let checked = !$(this).prop("checked");
		let radio_priority = $("#priority_" + $(this).data("sensor"));
		radio_priority.prop("disabled", checked);
		if (checked) {
			radio_priority.prop("checked", false);
		}
	});
	$.fn.pasteMashingSensors = function (sensors_select) {
		if ($.fn.objIsEmpty(globalSensorsJson, false)) {
			setTimeout(function () {
				$.fn.pasteMashingSensors(false);
			}, 1000);
		}
		let sensorsMashingSend = {
			"t1": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t2": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t3": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t4": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t5": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t6": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t7": {"name": "", "color": 0, "member": 0, "priority": 0},
			"t8": {"name": "", "color": 0, "member": 0, "priority": 0},
			"pause1": {"name": "", "time": 0, "temperature": 0, "stop": 0},
			"pause2": {"name": "", "time": 0, "temperature": 0, "stop": 0},
			"pause3": {"name": "", "time": 0, "temperature": 0, "stop": 0},
			"pause4": {"name": "", "time": 0, "temperature": 0, "stop": 0},
			"pause5": {"name": "", "time": 0, "temperature": 0, "stop": 0}
		};
		let mashingTemplate = '';
		let tpl_pause_body = '';
		let pause_thead = '<div class="col-xs-5 col-xs-offset-0 col-sm-3 col-sm-offset-3 text-center text-middle text-primary">Время паузы</div>' +
			'<div class="col-xs-5 col-xs-offset-0 col-sm-3 col-sm-offset-0 text-center text-middle text-primary">Температура паузы</div>' +
			'<div class="col-xs-2 col-xs-offset-0 col-sm-3 col-sm-offset-0 text-center text-middle text-primary">Стоп в конце</div>';
		let tpl_timer_body = '<div class="row row-striped">' +
			'<div class="pt-10 pb-10 clearfix">' +
			'<div class="col-xs-12 col-sm-4 text-center-xs text-strong">Время до конца паузы</div>' +
			'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="mashing_timer_text"></span><span class="hidden">&nbsp;мин.</span></div>' +
			'<div class="col-xs-3 col-sm-3"></div>' +
			'<div class="col-xs-4 col-sm-3"></div>' +
			'</div>' +
			'</div>';
		if (!sensors_select && $.fn.objIsEmpty(mashingProcess["sensors"], false)) {
			$.ajax({
				url: ajax_url_debug + 'mashingSensorsGetTpl',
				data: {},
				type: 'GET',
				dataType: 'json',
				success: function (msg) {
					mashingProcess["sensors"] = msg;
					if (!$.fn.objIsEmpty(mashingProcess["sensors"], false)) {
						$.fn.pasteMashingSensors(false);
					}
				},
				error: function (err, exception) {
					alertAjaxError(err, exception, $("#error_mashing"));
				}
			});
		}
		if (!$.fn.objIsEmpty(mashingProcess["sensors"], false)) {
			let tpl_all_body = '';
			$.each(mashingProcess["sensors"], function (i, e) {
				let sensor_key = i;
				let name_sensor = e["name"];
				if(sensor_key === "t2"){
					name_sensor = "Датчик в струе";
				}
				let stop = Number(e["stop"]);
				let time = e["time"];
				let temperature = e["temperature"];
				$.each(globalSensorsJson["mashing"], function (j, q) {
					let pause_key = Object.keys(q).shift();
					if(sensor_key === pause_key) {
						stop = e["stop"] = Number(q[pause_key]["stop"]);
						time = e["time"] = Number(q[pause_key]["time"]);
						temperature = e["temperature"] = Number(q[pause_key]["temperature"]);
					}
				});
				if (sensorsMashingSend[sensor_key].hasOwnProperty("name")) {
					sensorsMashingSend[sensor_key]["name"] = name_sensor;
				}
				if (re_t.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsMashingSend[sensor_key]["color"] = e["color"];
					sensorsMashingSend[sensor_key]["member"] = 1;
					sensorsMashingSend[sensor_key]["priority"] = e["priority"];
					tpl_all_body += '<div class="row row-striped">' +
						'<div class="pt-10 pb-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs text-strong" id="mashing_step_text_'+sensor_key+'">t&#176' + name_sensor + '</div>' +
						'<div class="col-xs-12 col-xs-offset-0 col-sm-3 col-sm-offset-0 text-center text-middle text-strong"><span id="mashing_' + sensor_key + '"></span><span' +
						' class="hidden">&#176С</span></div>' +
						'<div class="col-xs-0 col-sm-3"></div>' +
						'<div class="col-xs-0 col-sm-3"></div>' +
						'</div>' +
						'</div>';
				}
				if (re_pause.test(sensor_key)) {
					tpl_pause_body += returnTplHtml([{pause_thead:pause_thead, pause_name: name_sensor, id_stop: "mashing_stop_"+sensor_key, checked_stop: (stop === 1 ? "checked" : ''), id_time: "mashing_time_"+sensor_key, value_time:time,  id_temperature: "mashing_temperature_"+sensor_key,value_temperature:temperature,id_step_bg:"mashing_step_bg_"+sensor_key,id_step_text:"mashing_step_text_"+sensor_key}], pauseTempl);
					pause_thead = '';
				}
			});
			if (tpl_all_body !== '') {
				mashingTemplate += tpl_all_body;
			}
		}
		if (mashingTemplate !== '') {
			if (sensors_select) {
				sendRequest("mashingSensorsSetSave", sensorsMashingSend, "json", false, false, $("#error_mashing"), false);
			}
			mashingTemplate = mashingTemplate + tpl_timer_body + tpl_pause_body;
			$("#mashing_start_group_button").removeClass("hidden");
		} else {
			$("#mashing_start_group_button").addClass("hidden");
		}
		$("#mashing_process").html(mashingTemplate);
		if (mashingProcess["start"] === true) {
			getMashing();
			$('#mashing_start').prop("disabled", true);
			$('#mashing_add_sensor').prop("disabled", true);
		} else {
			$('#mashing_stop').prop("disabled", true);
			$('#mashing_add_sensor').prop("disabled", false);
		}
	};
	function launchMashing(){
		mashingProcess["start"] = flagSendProcess = true;
		$('#mashing_start').prop("disabled", true);
		$('#mashing_add_sensor').prop("disabled", true);
		$('#mashing_stop').prop("disabled", false);
		clearInterval(sensorsProcessId);
		stopInterval();
		localStorage.setObj('oldStartProcess', 3);
		setTimeout(function () {
			setMashing();
		}, 1000);
	}
	$(document).on('start-event','#mashing_start', function(e) {
		console.log("start-event-Mashing");
		launchMashing()
	});
	$(document).on('click', '#mashing_start', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Будет запущен процесс затирания, убедитесь в том, что тэн залит жидкостью!</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				launchMashing();
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
	$(document).on('click', '#mashing_stop', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите остановить процесс затирания?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				stopMashing()
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
	function stopMashing(){
		$('#mashing_stop').prop("disabled", true);
		$('#mashing_add_sensor').prop("disabled", false);
		$('#mashing_start').prop("disabled", false);
		$("#svg_mashing_start").css('stroke', "#000000");
		clearInterval(sensorsProcessId);
		// clearInterval(sensorsIntervalId);
		stopInterval();
		flagSendProcess = true;
		globalSensorsJson["process"]["allow"] = 0;
		mashingProcess["start"] = false;
		setMashing();
	}

	$(document).on('stop-event','#mashing_stop', function(e) {
		stopMashing()
	});

	//Установка значений для затирания
	function setMashing() {
		console.log("setMashing");
		if ($.fn.objIsEmpty(mashingProcess["sensors"], false)){
			setTimeout(function () {
				setMashing();
			}, 1000);
		}else {
			let mashingSendData = {
				"process": {"allow": 0, "number": 0},
				"pause1": {"time": 0, "temperature": 0, "stop": 0},
				"pause2": {"time": 0, "temperature": 0, "stop": 0},
				"pause3": {"time": 0, "temperature": 0, "stop": 0},
				"pause4": {"time": 0, "temperature": 0, "stop": 0},
				"pause5": {"time": 0, "temperature": 0, "stop": 0}
				//,"power": 0
			};
			mashingSendData["process"]["allow"] = (mashingProcess["start"] ? 3 : 0);

			$.each(mashingProcess["sensors"], function (i, e) {
				let sensor_key = i;
				let mashing_time = $("#mashing_time_" + sensor_key);
				let mashing_temperature = $("#mashing_temperature_" + sensor_key);
				let mashing_stop = $("#mashing_stop_" + sensor_key);
				if (mashing_time.length) {
					if (e["time"] !== mashing_time.val()) {
						flagSendProcess = true;
					}
					mashingSendData[sensor_key]["time"] = e["time"] = mashing_time.val();
				}
				if (mashing_temperature.length) {
					if (e["temperature"] !== mashing_temperature.val()) {
						flagSendProcess = true;
					}
					mashingSendData[sensor_key]["temperature"] = e["temperature"] = mashing_temperature.val();
				}
				if (mashing_stop.length) {
					let stop = Number(mashing_stop.prop("checked"));
					if (e["stop"] !== stop) {
						flagSendProcess = true;
					}
					mashingSendData[sensor_key]["stop"] = e["stop"] = stop;
				}
			});
			if (flagSendProcess) {
				flagSendProcess = false;
				clearInterval(sensorsProcessId);
				// clearInterval(sensorsIntervalId);
				stopInterval();
				sendRequest("SensorsIn", mashingSendData, "json", startMashing, false, $("#error_mashing"), false);

			}
		}
	}
	$(document).on('mousedown',"#mashing_process input", function () {
		flagSendProcess = true;
	});
	$(document).on('change',"#mashing_process input",
		$.debounce(function() {
			flagSendProcess = true;
			if(mashingProcess["start"] === true) {
				setMashing();
			}
		}, 300)
	);
	function startMashing() {
		flagSendProcess = false;
		console.log("startMashing");
		setTimeout(function () {
			clearInterval(sensorsProcessId);
			// clearInterval(sensorsIntervalId);
			stopInterval();
			startInterval();
			// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
			if(mashingProcess["start"] === true) {
				sensorsProcessId = setInterval(getMashing, 2000);
			}
		}, 2000);
	}
	function getMashing() {
		//let sek= parseInt(+new Date()/1000);
		//console.log(flagSendProcess,"getMashing"+sek);
		if (!$.fn.objIsEmpty(globalSensorsJson, false)) {
			let dtoJson = {};
			dtoJson["h"] = Number(globalSensorsJson["power"]);
			dtoJson["t"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);
				$.each(mashingProcess["sensors"], function (j, q) {
					if (j === sensor_key && re_t.test(sensor_key)) {
						q["value"] = sensor_value;
						let color_value = q["color"];
						let fillcolor = "#" + dec2hex(color_value);
						$("#svg_mashing_color_" + sensor_key).css('fill', colorPersent(fillcolor, sensor_value, 0));
						if (Number(q["member"]) !== 0) {
							dtoJson["t"][sensor_key] = sensor_value;
						}
					}
				});
				$("#mashing_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				//svg
				if(sensor_value < 150) {
					$("#svg_mashing_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
				}else{
					$("#svg_mashing_" + sensor_key).text('');
				}
			});
			let time_cur_pause = 0;
			let step_pause = 0;
			let stop_pause = 0;
			$.each(globalSensorsJson["mashing"], function (i, e) {
				let pause_key = Object.keys(e).shift();
				let step = Number(e[pause_key]["step"]);
				let stop = Number(e[pause_key]["stop"]);
				let time = Number(e[pause_key]["time"]);
				if(step>0){
					time_cur_pause = time;
					step_pause = 1;
					if(stop>0){
						stop_pause = 1;
					}
				}

				let temperature = Number(e[pause_key]["temperature"]);
				if(!flagSendProcess) {
					//убрал пока
					//$("#mashing_time_" + pause_key).val(time);
					//$("#mashing_temperature_" + pause_key).val(temperature);
					if (stop > 0) {
						$("#mashing_stop_" + pause_key).prop("checked", true);
					} else {
						$("#mashing_stop_" + pause_key).prop("checked", false);
					}
				}
				if(step>0) {
					$("#mashing_step_bg_" + pause_key).addClass("bg-success");
					$("#mashing_step_text_" + pause_key).addClass("text-success");
				}else{
					$("#mashing_step_bg_" + pause_key).removeClass("bg-success");
					$("#mashing_step_text_" + pause_key).removeClass("text-success");
				}

			});
			let power_value = Number(globalSensorsJson["power"]);
			let global_time = (Number(globalSensorsJson["process"]["time"])/60).toFixed(1);
			let timer_value = (time_cur_pause - global_time).toFixed(1);
			if(time_cur_pause > 0 && step_pause > 0){
				$("#mashing_timer_text").text(">"+time_cur_pause).parent().find(".hidden").removeClass("hidden").addClass("show");
			}
			if(timer_value > 0 && global_time > 0) {
				$("#mashing_timer_text").text(timer_value).parent().find(".hidden").removeClass("hidden").addClass("show");
			}
			if(step_pause > 0 && stop_pause > 0 && timer_value < 0){
				$("#mashing_timer_text").text("Пауза").parent().find(".show").removeClass("show").addClass("hidden");
			}

			$("#svg_mashing_ten_t").text(power_value.toFixed(0) + "%");
			$("#svg_mashing_color_ten").css('fill', colorPersent("#FF0000", power_value.toFixed(0), 100));
			$("#view_reflux_chart").html("");
			$("#view_distillation_chart").html("");
			$("#view_pid_chart").html("");
			if (!$.fn.objIsEmpty(dtoJson["t"], false) && drowChart) {
				dtoReceiver.start(dtoJson, 'view_mashing_chart');
			}
		}
		if (mashingProcess["start"] === true) {
			$("#svg_mashing_start").css('stroke', "#02b500");
		}
	}
	//настройка ПИД
	let pidProcess = {"pid": {}, "start": false};
	function launchPid(){
		pidProcess["start"] = flagSendProcess = true;
		$('#pid_start').prop("disabled", true);
		$('#pid_stop').prop("disabled", false);
		$('#set_pid').prop("disabled", true);
		clearInterval(sensorsProcessId);
		stopInterval();
		localStorage.setObj('oldStartProcess', 4);
		setTimeout(function () {
			setPid();
		}, 1000);
	}
	$(document).on('start-event','#pid_start', function(e) {
		console.log("start-event-Pid");
		launchPid()
	});
	$(document).on('click', '#pid_start', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Будет запущен процесс ПИД регулировки, убедитесь в том, что тэн залит жидкостью!</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				launchPid();
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
	$(document).on('click', '#pid_stop', function () {
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите остановить процесс регулировки ПИД?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				stopPid()
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
	function stopPid(){
		$('#pid_stop').prop("disabled", true);
		$('#pid_start').prop("disabled", false);
		$('#set_pid').prop("disabled", false);
		clearInterval(sensorsProcessId);
		flagSendProcess = true;
		globalSensorsJson["process"]["allow"] = 0;
		pidProcess["start"] = false;
		setPid();
	}
	$(document).on('stop-event','#pid_stop', function(e) {
		stopPid()
	});
	$(document).on('click','#set_pid',function (e) {
		e.preventDefault();
		let _this = $(this);
		let pidSend = {"save":1};

		if (!$.fn.objIsEmpty(pidProcess["pid"],false)) {
			$.each(pidProcess["pid"], function (j,q) {
				let pid_value = Number($("#pid_"+j).val());
				pidSend[j] = {"userSetValue":pid_value}
			});
		}
		if (!$.fn.objIsEmpty(pidSend,false)) {
			sendRequest("pidMashingSet", pidSend, "json", false, _this, $("#error_pid"), false);
		}
	});
	$.fn.pastePidSensors = function (sensors_select) {
		if ($.fn.objIsEmpty(globalSensorsJson, false)) {
			setTimeout(function () {
				$.fn.pastePidSensors(false);
			}, 1000);
		}
		$.each(globalSensorsJson["pid"], function (i, e) {
			let pid_key = Object.keys(e).shift();
			pidProcess["pid"][pid_key] = globalSensorsJson["pid"][i][pid_key];
		});
		$("#pid_start_group_button").removeClass("hidden");
		if (pidProcess["start"] === true) {
			getPid();
			$('#pid_start').prop("disabled", true);
			$('#set_pid').prop("disabled", true);
		} else {
			$('#pid_stop').prop("disabled", true);
			$('#set_pid').prop("disabled", false);
		}
	};
	//Установка значений для ПИД
	function setPid() {
		console.log("setPid");
		/*if (setPid.caller == null) {
			console.log('Эта функция была вызвана из верхнего уровня!');
		} else {
			console.log('Эта функция была вызвана из ' + setPid.caller);
		}*/
		if ($.fn.objIsEmpty(pidProcess["pid"], false)){
			setTimeout(function () {
				setPid();
			}, 1000);
		}else {
			let pidSendData = {
				"process": {"allow": 0},
				"Kp": {"userSetValue": 0},
				"Ki": {"userSetValue": 0},
				"Kd": {"userSetValue": 0},
				"t1": {"userSetValue": 0}
			};
			pidSendData["process"]["allow"] = (pidProcess["start"] ? 4 : 0);

			$.each(pidProcess["pid"], function (i, e) {
				let sensor_key = i;
				let pid_val = $("#pid_" + sensor_key);
				if (pid_val.length) {
					if (Number(e["userSetValue"]) !== Number(pid_val.val())) {
						flagSendProcess = true;
					}
					pidSendData[sensor_key]["userSetValue"] = e["userSetValue"] = Number(pid_val.val());
				}
			});
			if (flagSendProcess) {
				flagSendProcess = false;
				clearInterval(sensorsProcessId);
				// clearInterval(sensorsIntervalId);
				stopInterval();
				sendRequest("pidMashingSet", pidSendData, "json", startPid, false, $("#error_pid"), false);
			}
		}
	}
	$(document).on('mousedown',"#pid_process input", function () {
		flagSendProcess = true;
	});
	$(document).on('change',"#pid_process input",
		$.debounce(function() {
			flagSendProcess = true;
			if(pidProcess["start"] === true) {
				setPid();
			}
		}, 300)
	);
	function startPid(request) {
		flagSendProcess = false;
		console.log("startPid");
		setTimeout(function () {
			clearInterval(sensorsProcessId);
			// clearInterval(sensorsIntervalId);
			stopInterval();
			startInterval();
			// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
			if(pidProcess["start"] === true) {
				if (!$.fn.objIsEmpty(request, false)) {
					$.each(request["settings"], function (j, q) {
						let pid_key = Object.keys(q).shift();
						pidProcess["pid"][pid_key] = request["settings"][j][pid_key];
					});
				}
				sensorsProcessId = setInterval(getPid, 2000);
			}
		}, 2000);
	}
	function getPid() {
		// let sek= parseInt(+new Date()/1000);
		//console.log(flagSendProcess,"getPid "+sek);
		// console.log(pidProcess);
		if (!$.fn.objIsEmpty(globalSensorsJson, false)) {
			let dtoJson = {};
			dtoJson["t"] = {};
			if (!$.fn.objIsEmpty(pidProcess, false)) {
				$.each(pidProcess["pid"], function (pid_key, q) {
					if (!re_t.test(pid_key)) {
						if(pidProcess["pid"][pid_key].hasOwnProperty("deviceOutValue")) {
							let pid_value = Number(pidProcess["pid"][pid_key]["deviceOutValue"]);
							if (pid_value > 0) {
								if (pid_key === "Ki") {
									$(".pid_device_" + pid_key).text(pid_value.toFixed(2));
								} else {
									$(".pid_device_" + pid_key).text(pid_value.toFixed(0));
								}
							}
						}
						//убрал пока
						/*if(pidProcess["pid"][pid_key].hasOwnProperty("userSetValue")) {
							let pid_value = Number(pidProcess["pid"][pid_key]["userSetValue"]);
							if (!flagSendProcess) {
								$("#pid_" + pid_key).val(pid_value);
							}
						}*/
					}
				});
			}
			let sensor_value = Number(globalSensorsJson["sensors"][0]['t1']["value"]);
			$("#pid_value_t1").text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			if (pidProcess["start"] === true){
				dtoJson["t"]['t1'] = sensor_value;
			}
			$("#view_reflux_chart").html("");
			$("#view_distillation_chart").html("");
			$("#view_mashing_chart").html("");
			if (!$.fn.objIsEmpty(dtoJson["t"], false) && drowChart) {
				dtoReceiver.start(dtoJson, 'view_pid_chart');
			}
		}
	}


	//заполнение разных полей данными датчиков
	function fillSensorsData() {
		if (!$.fn.objIsEmpty(globalSensorsJson, false)) {
			if (pidProcess["start"] !== true) {
				$.each(globalSensorsJson["pid"], function (j, q) {
					let pid_key = Object.keys(q).shift();
					let user_value = Number(globalSensorsJson["pid"][j][pid_key]["userSetValue"]);
					if(user_value > 0 && Number($("#pid_" + pid_key).val()) === 0) {
						if(pid_key === "Ki"){
							$("#pid_" + pid_key).val(user_value.toFixed(2));
						}else {
							$("#pid_" + pid_key).val(user_value.toFixed(0));
						}
					}
				});
			}
			let process = Number(globalSensorsJson["process"]["allow"]);
			if (process !== 0){
				$("a#toggle_settings").addClass("disabled").css('cursor', 'not-allowed');
				$("#nav-tabs li a").addClass("disabled").css('cursor', 'not-allowed');

				switch (process) {
					case 1:
						$('#nav-tabs li a[data-target="#distillation"]').css('cursor', 'pointer');
						break;
					case 2:
						$('#nav-tabs li a[data-target="#reflux"]').css('cursor', 'pointer');
						break;
					case 3:
						$('#nav-tabs li a[data-target="#mashing"]').css('cursor', 'pointer');
						break;
					case 4:
						$('#nav-tabs li a[data-target="#pid"]').css('cursor', 'pointer');
						break;

				}
			}else{
				$("a#toggle_settings").removeClass("disabled").css('cursor', 'pointer');
				$("#nav-tabs li a").removeClass("disabled").css('cursor', 'pointer');
			}

			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				//заполнение вкладки датчики
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);

				if ($("#sensor_val_" + sensor_key).length && sensor_value < 150) {
					$("#sensor_val_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#svg_sensor_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
				}
				if (sensor_key === "p1") {
					$("#sensor_val_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				}
				//заполнение процесса дистилляции
				if (distillationProcess["start"] !== true) {
					$("#distillation_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#distillation_power_value").text(Number(globalSensorsJson["power"]).toFixed(0)).parent().find(".hidden").removeClass("hidden").addClass("show");
					if(sensor_value < 150) {
						$("#svg_distillation_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
					}else{
						$("#svg_distillation_" + sensor_key).text('');
					}
					$("#svg_distillation_ten_t").text(Number(globalSensorsJson["power"]).toFixed(0) + '%');
				}

				//console.log('fillSensorsData', sensor_key, sensor_value, $("#distillation_" + sensor_key).text());

				//заполнение процесса ректификации
				if (refluxProcess["start"] !== true) {
					$("#reflux_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					if (sensor_key === "p1") {
						$("#reflux_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#reflux_power_value").text(Number(globalSensorsJson["power"]).toFixed(0)).parent().find(".hidden").removeClass("hidden").addClass("show");
					}

					if(sensor_value < 150) {
						$("#svg_reflux_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
					}else{
						$("#svg_reflux_" + sensor_key).text('');
					}
					$("#svg_reflux_ten_t").text(Number(globalSensorsJson["power"]).toFixed(0) + '%');
				}
				//заполнение процесса затирания
				if (mashingProcess["start"] !== true) {
					$("#mashing_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					if(sensor_value < 150) {
						$("#svg_mashing_" + sensor_key).text(sensor_value.toFixed(1) + '°С');
					}else{
						$("#svg_mashing_" + sensor_key).text('');
					}
					$("#svg_mashing_ten_t").text(Number(globalSensorsJson["power"]).toFixed(0) + '%');
				}
				//заполнение ПИД регулировки
				if (pidProcess["start"] !== true) {
					$("#pid_value_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				}
			});
			//старт/стоп дистилляции
			if (distillationProcess["start"] !== true && process === 1) {
				$('#nav-tabs li a[data-target="#distillation"]').tab('show');
				$("#distillation_start").trigger("start-event");
			}
			if (distillationProcess["start"] === true && process !== 1) {
				$("#distillation_stop").trigger("stop-event");
			}
			//старт/стоп ректификации
			if (refluxProcess["start"] !== true && process === 2) {
				$('#nav-tabs li a[data-target="#reflux"]').tab('show');
				$("#reflux_start").trigger("start-event");
			}
			if (refluxProcess["start"] === true && process !== 2) {
				$("#reflux_stop").trigger("stop-event");
			}
			//старт/стоп затирания
			if (mashingProcess["start"] !== true && process === 3) {
				$('#nav-tabs li a[data-target="#mashing"]').tab('show');
				$("#mashing_start").trigger("start-event");
			}
			if (mashingProcess["start"] === true && process !== 3) {
				$("#mashing_stop").trigger("stop-event");
			}
			//старт/стоп ПИД
			if (pidProcess["start"] !== true && process === 4) {
				$('#nav-tabs li a[data-target="#pid"]').tab('show');
				$("#pid_start").trigger("start-event");
			}
			if (pidProcess["start"] === true && process !== 4) {
				$("#pid_stop").trigger("stop-event");
			}
			//очистка данных графиков и запуск
			let oldTimeStart = Number(localStorage.getObj('timeStartProcess'));
			//console.log(oldTimeStart);
			if(oldTimeStart !== Number(globalSensorsJson["process"]["timeStart"])) {
				localStorage.setObj('timeStartProcess', globalSensorsJson["process"]["timeStart"]);
				//очищаем графики
				clearChart();
			}
			let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
			if (process > 0 && oldStartProcess !== process) {
				//очищаем графики
				clearChart();
			}
			if (process > 0 && !drowChart && Number(globalSensorsJson["process"]["timeStart"])>0){
				startChart();
			}
			if(Number(globalSensorsJson["sound"]) > 0 ){
				playSound();
			}
		}
	}
	//запрос постоянно всех датчиков
	let tmpTime = 0;
	let stopTime = 30;
	let openModalError = false;
	let countError = 0;
	function getIntervalSensors() {
		$.ajax({
			url: ajax_url_debug + 'SensorsOut',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				//clearInterval(sensorsIntervalId);
				countError = 0;
				//console.log('Sensors',msg);
				globalSensorsJson = msg;
				fillSensorsData();
				tmpTime = 0;
				if(openModalError){
					$('.modal').modal('hide');
				}
				openModalError = false;
			},
			error: function (err, exception) {
				countError ++;
				if(countError > 10) {
					globalSensorsJson = {};
					if (!openModalError) {
						openModalError = true;
						$.fn.openModal('',
							'<p class="text-center text-danger text-strong">Ошибка загрузки данных датчиков, проверьте питание контроллера и обновите страницу</p>' +
							'<p class="text-center text-strong" id="modal_time_out"></p>',
							"modal-sm", false, {
								text: "Запустить",
								id: "return_interval",
								class: "btn btn-success hidden",
								click: function () {
									$(this).closest(".modal").modal("hide");
									startInterval();
									// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
								}
							});
					}
					$("#modal_time_out").text('Запрос датчиков прекратится через ' + (stopTime - tmpTime) + ' сек.');
					tmpTime++;
					if (tmpTime > stopTime) {
						$("#modal_time_out").text('Запрос датчиков остановлен');
						$("#return_interval").removeClass("hidden");
						// clearInterval(sensorsIntervalId);
						stopInterval();
						clearInterval(sensorsProcessId);
						tmpTime = 0;
						openModalError = false;
					}
				}
			},
		});
		//if(tmpTime<100 && refluxProcess["start"] === true)
		//tmpTime ++;
	}
	function startInterval(){
		sensorsIntervalId = setInterval(getIntervalSensors, 1000);
	}
	function stopInterval(){
		clearInterval(sensorsIntervalId);
	}

	//clearInterval(sensorsIntervalId);
	$(document).ready(function () {
		console.log('ready');
		startInterval();
		//setTimeout(getSettings, 2000);
		// sensorsIntervalId = setInterval(getIntervalSensors, 1000);
		/*setTimeout(function () {
			startInterval();
		}, 3000);*/
	});

////////////////////////////////////////////////////////////////


	//Свойства TODO - переделать все свойства в один запрос
	function getSettings() {
		$.ajax({
			url: ajax_url_debug + 'configs.json',
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				// console.log('Settings', msg);
				$("#settings_ssdp").val(msg["SSDP"]);
				$("#settings_ssid").val(msg["ssid"]);
				$("#version").text("v."+msg["version"]);
				// $("#settings_password").val(msg["password"]);
				$("#settings_ssidap").val(msg["ssidAP"]);
				// $("#settings_passwordap").val(msg["passwordAP"]);
				$("#settings_timezone").val(msg["timezone"]);
			}
		});
	}

	setTimeout(getSettings, 2000);

	//Обновление прошивки
	$("#file_update").on("change", function () {
		let fileLength = $(this)[0].files.length;
		let fileName = $(this)[0].files[0].name;
		// LuckyBox.ino.bin - файл обновления ПО
		// LuckyBox.spiffs.bin - файл обновления web
		console.log(fileName);
		if(fileName !== "LuckyBox.ino.bin" && fileName !== "LuckyBox.spiffs.bin"){
			$.fn.openModal('',
				'<p id="modal_text_info" class="text-center text-danger text-strong">Не корректное имя файла обновления прошивки</p>' +
				'<p>Файлы обновления должны иметь названия:</p>' +
				'<p><strong>LuckyBox.ino.bin</strong> - файл обновления ПО</p>' +
				'<p><strong>LuckyBox.spiffs.bin</strong> - файл обновления web</p>'
			);
		}else {
			if (fileLength !== 0) {
				$("#settings_update").prop("disabled", false);
			}
		}
	});

	let modal_interval = 0;

	$("form#firmware_update").submit(function (e) {
		e.preventDefault();
		// clearInterval(sensorsIntervalId);
		stopInterval();
		let formData = new FormData();
		formData.append('update', $('#file_update')[0].files[0]);
		//console.log(formData,$('#file_update')[0].files[0]);
		$.ajax({
			url: ajax_url_debug + 'update',
			type: 'POST',
			data: formData,
			cache: false,
			contentType: false,
			dataType: 'json',
			enctype: 'multipart/form-data',
			processData: false,
			beforeSend: function () {
				console.log("beforeSend");
				$("#settings_update").ajaxLoading({disabled: true});
				$.fn.openModal('',
					'<p id="modal_text_info" class="text-center text-danger text-strong">Происходит загрузка файла в контроллер, пожалуйста дождитесь окончания загрузки</p>' +
					'<p class="text-center text-strong" id="modal_time_out"></p>',
					"modal-sm", false, [{
						text: "Закрыть",
						id: "modal_update_close",
						class: "btn btn-primary btn-sm hidden",
						click: function () {
							$(this).closest(".modal").modal("hide");
						}
					}], {buttons: "replace", id: "modal_update", data:{backdrop: 'static'}});
			},
			success: function (msg) {
				if(msg.hasOwnProperty('update') && msg['update'] === 'ok'){
					setTimeout(function () {
						clearInterval(modal_interval);
						$("#modal_text_info").html('Файл обновления успешно загружен, контроллер будет перезагружен.<br><br>Страница будет автоматически обновлена, через 1 минуту').removeClass('text-danger').addClass('text-success');
						// console.log("success", msg);
						tmpTime = 0;
						modal_interval = setInterval(function () {
							tmpTime++;
							$("#modal_time_out").text((tmpTime) + ' сек.');
							if(tmpTime >= 60) {
							 	location.reload(true)
							}
						}, 1000)
					},1000);
				}
				if(msg.hasOwnProperty('update') && msg['update'] === 'err'){
					setTimeout(function () {
						$("#modal_text_info").html('Ошибка обновления прошивки, попробуйте загрузить файл еще раз');
						$("#modal_update_close").removeClass('hidden');
						// console.log("success", msg);
					},1000);
				}

			},
			error: function (err, exception) {
				setTimeout(function () {
					$("#modal_text_info").html('Ошибка обновления прошивки, попробуйте загрузить файл еще раз');
					$("#modal_update_close").removeClass('hidden');
					// console.log("success", msg);
				},1000);
				// console.log("error", err, exception);
				// alertAjaxError(err, exception, $("#error_settings"));
			},
			complete: function () {
				// console.log("complete", msg);
				$("#settings_update").ajaxLoading('stop').prop("disabled", false);
			}
		});
		//return false;
	});

	$(document).on("focus", "#settings_password, #settings_passwordap", function () {
		$(this).prop("type", "text");
	});
	$(document).on("blur", "#settings_password, #settings_passwordap", function () {
		$(this).prop("type", "password");
	});
	$("#settings_set_ssdp").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssdp = $("#settings_ssdp").val();
		if(ssdp !== "") {
			sendRequest("ssdp", {"ssdp": ssdp}, "text", false, _this, $("#error_settings"), false);
		}else{
			$.fn.openModal('', '<p class="text-center text-danger text-strong">Заполните поле</p>', "modal-sm", true, false);
		}
	});
	$("#settings_set_ssid").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssid = $("#settings_ssid").val();
		let pass = $("#settings_password").val();
		if(ssid !== "" && pass.length >= 8) {
			sendRequest("ssid", {
				"ssid": ssid,
				"password": pass
			}, "text", false, _this, $("#error_settings"), '<p class="text-center text-success text-strong">Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.</p>');
		}else{
			$.fn.openModal('', '<p class="text-center text-danger text-strong">Заполните поля, пароль не может быть меньше 8 знаков</p>', "modal-sm", true, false);
		}
	});
	$("#settings_set_ssidap").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssidap = $("#settings_ssidap").val();
		let pass = $("#settings_passwordap").val();
		if(ssidap !== "" && pass.length >= 8) {
		sendRequest("ssidap", {
				"ssidAP": ssidap,
				"passwordAP": pass
			}, "text", false, _this, $("#error_settings"), '<p class="text-center text-success text-strong">Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.</p>');
		}else{
			$.fn.openModal('', '<p class="text-center text-danger text-strong">Заполните поля, пароль не может быть меньше 8 знаков</p>', "modal-sm", true, false);
		}
	});
	$("#settings_auto_timezone").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let date = new Date();
		let timezone = Math.abs(date.getTimezoneOffset() / 60);
		$("#settings_timezone").val(timezone);
		sendRequest("TimeZone", {"timezone": timezone}, "text", false, _this, $("#error_settings"), '<p class="text-center text-success text-strong">Изменения временной зоны сохранены</p>');


	});
	$("#settings_set_timezone").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let timezone = $("#settings_timezone").val();
		if(timezone !== "") {
			sendRequest("TimeZone", {"timezone": timezone}, "text", false, _this, $("#error_settings"), '<p class="text-center text-success text-strong">Изменения временной зоны сохранены</p>');
		}else{
			$.fn.openModal('', '<p class="text-center text-danger text-strong">Заполните поле</p>', "modal-sm", true, false);
		}
	});
	$("#settings_restart").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		$.fn.openModal('', '<p class="text-center text-danger text-strong">Вы действительно хотите перезагрузить устройство?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				sendRequest("restart", {"device": "ok"}, "text", false, _this, false, false);
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
	//Поворот экрана
	$("#settings_set_rotate").on("click", function (e) {
		e.preventDefault();
		let _this = $(this);
		let tft_rotate = ($("#tft_rotate").prop("checked") ? 1 : 0);
		let touchpad_rotate = ($("#touchpad_rotate").prop("checked") ? 1 : 0);
		sendRequest("rotate", {
			"tft_rotate": tft_rotate,
			"touchpad_rotate": touchpad_rotate
		}, "text", false, _this, $("#error_settings"), false);
	});
});