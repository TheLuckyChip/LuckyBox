(function ($) {
    /**
     * Проверка объекта на пустоту
     * @param {object} obj - Объект
     * @param {boolean} key - Проверять ключи в объекте
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
    };
    /**
	 * Удаление элемента из массива
     * @param arr
     * @param value
     * @returns {Array}
     */
    $.fn.arrayUnset = function(arr,value) {
        for(let i = 0; i < arr.length; i++) {
            if(arr[i] === value) {
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
        if (optgroup === undefined) optgroup = false;

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
    $.fn.clearSelect = function(select) {
        if (select.prop('tagName') === 'SELECT') {
            select.prop('options').length = 0;
        }
    }
})(jQuery);
//$(document).ready(function () {
$(function() {
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
	function colorPersent(color,current,max)
	{
		let rgb = hexToRgb(color);
		let alpha = 1;
		if(Number(max)>0){
			alpha = ((current*100)/max)/100
		}
		return 'rgba('+rgb.r+','+rgb.g+','+rgb.b+','+alpha+')';
		//return "#FF0000";
	}

	/**
	 * Проверка на мобильную версию
	 * @returns {boolean}
	 */
	function isMobile() {
		try {
			if(/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini|Opera Mobile|Kindle|Windows Phone|PSP|AvantGo|Atomic Web Browser|Blazer|Chrome Mobile|Dolphin|Dolfin|Doris|GO Browser|Jasmine|MicroB|Mobile Firefox|Mobile Safari|Mobile Silk|Motorola Internet Browser|NetFront|NineSky|Nokia Web Browser|Obigo|Openwave Mobile Browser|Palm Pre web browser|Polaris|PS Vita browser|Puffin|QQbrowser|SEMC Browser|Skyfire|Tear|TeaShark|UC Browser|uZard Web|wOSBrowser|Yandex.Browser mobile/i.test(navigator.userAgent)) {
				return true;
			}
			return false
		}
		catch(e){ console.log("Error in isMobile"); return false; }
	}
	/*function isMobile(){
		let mobile = false;
		if( /Android|webOS|iPhone|iPad|iPod|pocket|psp|kindle|avantgo|blazer|midori|Tablet|Palm|maemo|plucker|phone|BlackBerry|symbian|IEMobile|mobile|ZuneWP7|Windows Phone|Opera Mini/i.test(navigator.userAgent) ) {
			mobile = true;
		}
		return mobile;
	}*/

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
	let timeout = false;
    $(document).on('mousedown','.minus',function(e) {
        e.preventDefault();
        let _this = $(this);
        let count_interval = 0;
        let time = 500;
        timeout = setInterval(function(){
            count_interval ++;
            if(count_interval>3) {
                time = time / 2;
                count_interval = 0;
            }
            //console.log(count_interval,time);
            let $input = _this.parent().find('input');
            let step = Number($input.attr("step"));
            let min = Number($input.attr("min"));
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
        }, time);
        /*let $input = $(this).parent().find('input');
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
        $input.val(count);*/
        //$input.change();
        //return false;
    });
    $(document).on('mouseup','.minus',function(e) {
		e.preventDefault();
        clearInterval(timeout);
	});
	$(document).on('click','.minus',function(e) {
        e.preventDefault();
        let $input = $(this).parent().find('input');
        let step = Number($input.attr("step"));
        let min = Number($input.attr("min"));
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
        $input.val(count);
    });
	$(document).on('mousedown','.plus',function(e) {
    //$(".plus").on('mouseup',function(e) {
		e.preventDefault();
        let _this = $(this);
        let count_interval = 0;
        let time = 500;
        timeout = setInterval(function() {
            count_interval++;
            if (count_interval > 3) {
                time = time / 2;
                count_interval = 0;
            }
            let $input = _this.parent().find('input');
            let step = Number($input.attr("step"));
            let max = Number($input.attr("max"));
            let fixed = 0;
            const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
            if (typeof max === typeof undefined && max === false)
                max = 100;
            if (typeof step !== typeof undefined && step !== false) {
                step = parseFloat(step);
                fixed = f(step);
            } else {
                step = 1;
            }
            let count = Number($input.val()) + step;
            count = count > max ? max : count;
            count = count.toFixed(fixed);

            $input.val(count);//
            //$input.change();
            //return false;
        }, time);
	});
    $(document).on('mouseup','.plus',function(e) {
        e.preventDefault();
        clearInterval(timeout);
    });
    $(document).on('click','.plus',function(e) {
        e.preventDefault();
       // if(timeout === false) {
        let $input = $(this).parent().find('input');
        let step = Number($input.attr("step"));
        let max = Number($input.attr("max"));
        let fixed = 0;
        const f = x => ((x.toString().includes('.')) ? (x.toString().split('.').pop().length) : (0));
        if (typeof max === typeof undefined && max === false)
            max = 100;
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
            //return false;
       // }
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
	 * @param {string|boolean} success_text - текстовое сообщение в диалоговое окно при успешной отправке данных
	 */
	function sendRequest(url,data,dataType,success_action,load_target,error_target,success_text) {
		//console.log(url,data,target);
		$.ajax({
			url: url,
			data: data,
			type: 'GET',
			dataType: dataType,
			beforeSend: function(){
				if(load_target !== false)
					$(load_target).ajaxLoading({disabled:true,color:"#FF0000"});
			},
			success: function (msg) {
				if(success_action !== false)
					success_action(msg);
				if(success_text !== false)
					$.fn.openModal('', success_text, "modal-sm", true, false);
			},
			error:function (err,exception) {
				if(error_target !== false)
					alertAjaxError(err,exception,error_target);
			},
			complete:function () {
				if(load_target !== false)
					$(load_target).ajaxLoading('stop');
			}
		});
	}
//////////////////////////////////////////////////////////////////////////
	//Определение датчиков
	let sensorsJson = {};
	let globalSensorsJson = {};
	let sensorsIntervalId = 0;
	//регекспы для датчиков
	const re_t = new RegExp(/^t/);
	const re_out = new RegExp(/^out/);
	const re_in = new RegExp(/^in/);
	//Глобальный объект dtoReceiver служит для опроса МК.
	let dtoReceiver = {
		dtos: [],                               // Контейнер состояний в ОЗУ
		frequencyRecordingToLocalStorage: 5,    // Частота архивации (Через сколько опросов осуществляется запись в localStorage)
		reqestDelayDefalt: 1000,

		dtoGet: function (json,target) {
			const self = dtoReceiver;  // Для доступа к this в jquery
			let requestCounter = 0;    // Счётчик запросов, служит для записи в localStorage каждые frequencyRecordingToLocalStorage раз

			self.dtoCurrent = json;
			self.dtoContainer = target;
			self.dtoCurrent.dateTime = (new Date()).getTime();  // Пользуемся временем клиента, для несчастных без доступа к NTP

			/*let process = Number(globalSensorsJson["process"]["allow"]);
			let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
			if(process > 0 && oldStartProcess !== process) {
				clearChart();
			}*/
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
			$(document).trigger('newDTOreceived', [self.dtoCurrent,self.dtoContainer]);
		},

		// Очистка LocalStorage
		clearDeviceConditions: function() {
			this.dtos = [];
			localStorage.removeItem('dtos');
		},
		// Запуск опроса ESP
		start: function (dtoJson,target) {
			//let dtoJson = {};
			//dtoJson["heaterPower"] = globalSensorsJson["power"];
			//dtoJson["temperatures"] = globalSensorsJson["sensors"];
			//console.log(dtoJson);
			/*this.clearDeviceConditions();
			if(plot.hasOwnProperty("series")) {
				plot.series.forEach(function (s) {
					s.setData([])
				});
				plot.redraw();
			}*/
			console.log("startChart",target);
			this.dtoGet(dtoJson,target);
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
	/*$(document).one("newDTOreceived", function (e) {
		console.log(e,dtoReceiver.dtoContainer);
		let process = Number(globalSensorsJson["process"]["allow"]);
		/!*let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
		if(process > 0 && oldStartProcess !== process) {
			clearChart();
		}*!/
		if(process !== 0)
			plot = getPlot();
	});*/

	function startChart() {
		$(document).one("newDTOreceived", function (e) {
			console.log(e,dtoReceiver.dtoContainer);
			let process = Number(globalSensorsJson["process"]["allow"]);
			/*let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
			if(process > 0 && oldStartProcess !== process) {
				clearChart();
			}*/
			if(process !== 0)
				plot = getPlot();
		});
	}

	function clearChart() {
		$(document).off("newDTOreceived");
		plot = {};
		console.log("clearChart");
		dtoReceiver.clearDeviceConditions();
		if(plot.hasOwnProperty("series")) {
			plot.series.forEach(function (s) {
				s.setData([])
			});
			plot.redraw();
		}
	}

	//очищаем по старту
	/*$('#plotClear').click(function() {
		dtoReceiver.clearDeviceConditions();
		plot.series.forEach(function (s) { s.setData([]) });

		plot.redraw();
	});*/

	/*$("#reqestDelay").val(window.dtoReceiver.reqestDelayDefalt / 1000);
	$('#changeSpeed').click(function () {

		let reqestDelay = $("#reqestDelay").val();

		if (reqestDelay>=1) {
			window.dtoReceiver.changeSpeed($("#reqestDelay").val() * 1000);
		} else {
			$("#reqestDelay").val(window.dtoReceiver.reqestDelayDefalt / 1000);
			alert('Минимальный интервал 1 сек');
		}
	});*/

	function getPlot() {
		//console.log("Запуск графиков!");
		let jsonPlot = {
			chart: {
			},
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
			}, { // Secondary yAxis
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
			}/*, { // Secondary yAxis
				gridLineWidth: 0,
				title: {
					text: 'Атм. давление',
					style: {
						color: 'black'
					}
				},
				labels:false,
				max: 800,
				opposite: false
			}*/
			],

			series: [
				{
					name: "Мощность", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 0, showInLegend: true,
					data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.heaterPower] })
				}/*,
				{
					name: "Атмосферное давление", yAxis: 2, type: "area", step: 'left', fillOpacity: 0.05, color: "#00e4f0", lineWidth: 1, showInLegend: true,
					data: dtoReceiver.dtos.map(function (dc) {
						return [dc.dateTime, dc.pressure]
					})
				}*/
			],
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
		};

		if(globalSensorsJson["process"]["allow"] === 2){
			//view_chart = 'view_reflux_chart';
			jsonPlot.yAxis[2] = { // Secondary yAxis
				gridLineWidth: 0,
					title: {
				text: 'Атм. давление',
					style: {
					color: 'black'
				}
			},
				labels:false,
					/*labels: {
						format: '{value} мм рт.ст.',
						style: {
							color: 'black'
						},
						align: 'left',
						x: 10,
					},*/
					max: 800,
				opposite: false
			};
			jsonPlot.series[1] = {
				name: "Атмосферное давление", yAxis: 2, type: "area", step: 'left', fillOpacity: 0.05, color: "#00e4f0", lineWidth: 1, showInLegend: true,
				data: dtoReceiver.dtos.map(function (dc) {
					return [dc.dateTime, dc.pressure]
				})
			};
		}

		let view_chart = dtoReceiver.dtoContainer;
		let plotNew = Highcharts.stockChart(view_chart, jsonPlot);

		dtoReceiver.dtos[0].temperatures.forEach(function (t, i) {
			console.log("plot",t, i);
			//if (t["key"] !== "p1") {
			/*if (re_t.test(key)){
				plotNew.addSeries({
					name: t["name"],
					//color: "#" + t["color"],
					color: "#"+dec2hex(t["color"]),
					data: dtoReceiver.dtos.map(function (dc) {
						//console.log(dc.temperatures[i]["value"]);
						return [dc.dateTime, dc.temperatures[i]["value"]]
					})
				});
			}*/
		});

		$(document).on("newDTOreceived", function (e, dto) {
			if(plot.hasOwnProperty("series")) {
				plot.series[0].addPoint([dto.dateTime, dto.heaterPower], false);
				let count = 0;
				let process = Number(globalSensorsJson["process"]["allow"]);
				if (process === 2) {
					plot.series[1].addPoint([dto.dateTime, dto.pressure], false);
					count = 1;
				}
				/*let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
				if(process > 0 && oldStartProcess !== process) {
					clearChart();
				}*/
				dto.temperatures.forEach(function (t, i) {
					console.log("newDTOreceived", t, i);
					/*if (t["key"] !== "p1") {
						//plot.series[1].addPoint([dto.dateTime, dto.temperatures[i]["value"]], false);
						//console.log(t,i);
						//}else {
						plotNew.series[count + 1].addPoint([dto.dateTime, dto.temperatures[i]["value"]], false);
					}*/
					count++;
				});
			}
			plotNew.redraw();
		});

		return plotNew;
	}

	//опрос датчиков
	$(document).on('click','#get_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("sensorsOutSet",{},"json",getSensors,_this,$("#error_sensors"),false);
	});

	//сортировка датчиков
	$(document).on('change',"#sensors_settings select",function () {
        let arrSortSensors = [];
		$.each($('#sensors_settings select'),function (i,e) {
			if($(e).val() !== ""){
				arrSortSensors.push($(e).val());
			}
		});
		$('#sensors_settings select option').show();
		$.each(arrSortSensors,function (i,e) {
			$.each($('#sensors_settings select'),function (j,s) {
				if ($(s).val() !== e) {
					$('option[value="' + e + '"]',$(s)).hide();
				}
			});
		});
    });
	//Заполнение вкладки датчики данными после запроса
	function getSensors(data) {
		//console.log(data);
		sensorsJson = data;
		let sensors = data;
		for (let key in sensors) {
			if(sensors.hasOwnProperty(key)) {
				let sensor_value =  sensors[key]["value"];
				if (re_t.test(key)) {
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
					if (sensor_value < 150) {
						$("#sensor_name_" + key).val(sensors[key]["name"]);
						$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
						$("#sensor_val_" + key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#svg_sensor_" + key).html(sensor_value.toFixed(0) + '&#176С');
						$("#svg_sensor_color_" + key).css('fill', jscolor);
					} else {
						if (!$.fn.objIsEmpty(sensors[key]["name"], false))
							$("#sensor_name_" + key).val("");
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
	$(document).on('click','#set_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		let nameError = false;
        let arrSortSensors = [1,2,3,4,5,6,7,8];
		let sensorsSend = {};
		for (let key in sensorsJson) {
			if(sensorsJson.hasOwnProperty(key)){
				sensorsSend[key] = {};
				if (re_t.test(key)) {
					let color_val = $("#sensor_color_" + key).val();
					let sensor_val = $("#sensor_name_" + key).val();
					if (sensor_val === "" && sensorsJson[key]["value"] < 150)
						nameError = true;
					sensorsSend[key]["name"] = sensorsJson[key]["name"] = sensor_val;
					//console.log(key,color_val);
					sensorsSend[key]["color"] = sensorsJson[key]["color"] = (color_val !== "FFFFFF" && color_val !== "") ? hex2dec(color_val) : 0;

					if (key !== "p1") {
						let sort_number = Number($("#sensor_number_" + key).val());
						if (sort_number !== 0)
							$.fn.arrayUnset(arrSortSensors, sort_number);
						sensorsSend[key]["number"] = sensorsJson[key]["number"] = sort_number;
					}
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
		if(arrSortSensors.length > 0){
            for (let key in sensorsJson) {
                if (sensorsJson.hasOwnProperty(key) /*&& key !== "p1"*/) {
                    if(sensorsSend[key]["number"] === 0){
                        sensorsSend[key]["number"] = sensorsJson[key]["number"] = arrSortSensors[count];
                        count++;
                    }
                }
            }
		}
		if(nameError) {
			$.fn.openModal('', '<p class="text-center text-danger"><strong>Заполните названия подключенных датчиков</strong></p>', "modal-sm", true, false);
		}else {
            //console.log(sensorsJson);
			sendRequest("sensorsInSet", sensorsSend, "json", setSensors, _this, $("#error_sensors"),false);
		}
	});
	function setSensors(data) {
		//console.log(data);
        sendRequest("sensorsOutSet",{},"json",getSensors,$('#get_sensors'),$("#error_sensors"),'<p class="text-center text-success"><strong>Настройки датчиков сохранены</strong></p>');
		//$.fn.openModal('', '<p class="text-center text-success"><strong>Тестовое сообщение, УРА!</strong></p>', "modal-sm", true, false);
		//localStorage.setObj('sensors', sensorsJson);
	}
	//Шаблоны
	const pressureTemplate =
		'<div class="row row-striped">' +
		'<div class="pt-10 clearfix">' +
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle"><strong>Атмосферное давление</strong></div>' +
		'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_pressure"></span> <span class="hidden">мм рт.ст.</span></strong></div>' +
		'</div>' +
		//'<div class="row row-striped">' +
		'<div class="pb-10 clearfix">' +
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle"><strong>t&#176 кипения спирта при данном давлении</strong></div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_alco_boil"></span><span class="hidden">&#176С</span></strong></div>' +
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
		'<div class="col-xs-12 col-sm-4 text-center-xs text-middle"><strong>Мощность тена</strong></div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="${id_value}"></span><span class="hidden">%</span></strong></div>' +
		'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 col-centered">' +
		returnTplHtml([{id:"${id_set}", value: '0', min: '0', max: '100', step: '1'}], deltaTempl) +
		'</div>' +
		'</div>' +
		'</div>';
	//Привязка датчиков к процессу ректификации, и запуск
	let refluxProcess = {"sensors":[],"power":0,"start":false};//"devices":[],"safety":[],
	$(document).on('click','#reflux_add_sensor',function(e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("refluxSensorsSetLoad", {}, "json", selectSensorsReflus, _this, $("#error_reflux"),false);
	});
	//Запрос датчиков для ректификации и вывод их в диалоговое окно
	function selectSensorsReflus(data){
		let sensors = data;//sensorsJson
		console.log(sensors);
		if(sensors !== null) {
			let section = '<section id="reflux_sensors" class="table-responsive"><table class="table table-noborder">';
			let tpl_temperature = '';
			let tpl_devices = '';
			let tpl_safety = '';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					if(sensor_name !== "") {
						//if (key !== "p1") {
						if (re_t.test(key)) {
							let sensor_delta = '<label class="checkbox-inline"><input disabled id="delta_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Уставка</label>';
							let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Отсечка</label>';
							//console.log(sensor_name);
							//sensor_delta = sensor_cutoff = '';
							//sensor_name = "Атмосферное давление";

							let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
							//let disabled_check = "";
							//if (sensor_name === "")
								//disabled_check = "disabled";

							tpl_temperature += '<tr><td>' +
								'<div class="input-group input-group-sm">' +
								'<span class="input-group-addon" style="background-color: #' + jscolor + '">' + key + '</span>' +
								'<input id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
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
								'<input id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
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
								'<input id="reflux_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">' +
								'</div></td>' +
								'<td><input data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
								'<td></td>' +
								'<td></td>' +
								'</tr>';
						}
					}
				}
			}
			if(tpl_temperature !== ''){
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики температуры</td></tr>' + tpl_temperature;
			}
			if(tpl_devices !== ''){
				section += '<tr><td colspan="4" class="text-center text-strong">Клапана</td></tr>' + tpl_devices;
			}
			if(tpl_safety !== ''){
				section += '<tr><td colspan="4" class="text-center text-strong">Датчики безопасности</td></tr>' + tpl_safety;
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для ректификации', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						let sensors_select = $('#reflux_sensors input[type=checkbox]');
						refluxProcess["sensors"] = $.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								//console.log($(e).data("sensor"));
								let key = $(e).data("sensor");
								let tmp = false;
								if (re_t.test(key)) {
									tmp = true;
								}
								let name = $("#reflux_name_" + key).val();
								let val_color = (tmp ? $("#reflux_color_" + key).val() : "");
								//let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
								let color = (val_color !== "FFFFFF" && val_color !== "") ? val_color : "FFFFFF";
								let delta = (tmp ? $("#delta_" + key).prop("checked") : false);
								let cutoff = (tmp ? $("#cutoff_" + key).prop("checked") : false);

								let obj = {};
								if(tmp) {
									//"t1":{"value":54.00,"name":"В кубе","color":64588,"delta":false,"cutoff":true,"priority":0,"allertValue":53.00},
									return obj[key] = {"name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue": 0, "value": 0};
								}else{
									return obj[key] = {"name": name, "value": 0};
								}
							}
						});
						//refluxProcess["sensors"] = reflux_sensors;
						//localStorage.setObj('reflux', refluxProcess);
						//console.log(ar_sensors);
						$(this).closest(".modal").modal("hide");
						$.fn.pasteRefluxSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
			//jscolor.installByClassName("jscolor");
		}
	}
	/*$(document).on('shown.bs.modal',"#modal_sensors_select", function(event){
		//console.log(event);
		jscolor.installByClassName("jscolor");
	});*/
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

	$.fn.pasteRefluxSensors = function(sensors_select){
		//let sensors = localStorage.getObj('sensors');//sensorsJson
		let sensorsRefluxSend = {
			"t1":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t2":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t3":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t4":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t5":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t6":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t7":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"t8":{"name":"","delta":false,"cutoff":false,"color":0,"member":0,"priority":0,"allertValue":0},
			"out1":{"name":"","member":0},
			"out2":{"name":"","member":0},
			"out3":{"name":"","member":0},
			"out4":{"name":"","member":0},
			"out5":{"name":"","member":0},
			"out6":{"name":"","member":0},
			"out7":{"name":"","member":0},
			"out8":{"name":"","member":0},
			"in1":{"name":"","member":0},
			"in2":{"name":"","member":0},
			"in3":{"name":"","member":0},
			"in4":{"name":"","member":0}
			//,"p1":{"value":760.00,"color":0,"member":0}
		};
		let refluxTemplate = '';
		let tpl_devices_body = '';
		let tpl_safety_body = '';
		//let localReflux = localStorage.getObj('reflux');
		//if(localReflux !== null) //refluxProcess = localReflux;
		if(!sensors_select && $.fn.objIsEmpty(refluxProcess["sensors"],false)){
			//console.log('empty');
			$.ajax({
				url: 'refluxSensorsGetTpl',
				data: {},
				type: 'GET',
				dataType: 'json',
				success: function(msg) {
					refluxProcess["sensors"] = msg;
					//console.log(refluxProcess["sensors"]);
					$.fn.pasteRefluxSensors(false);
				}
			});
		}
		if(!$.fn.objIsEmpty(refluxProcess["sensors"],false)) {
			let tpl_delta_thead = //'<div class="col-xs-hidden col-sm-4"></div>' +
				'<div class="row-xs">'+
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary">Дельта</div>' +
				'<div class="col-xs-4 col-sm-2 text-center text-middle text-primary">Уставка</div>'+
				'</div>';
			let tpl_delta_body = '';
			let tpl_cutoff_thead =
				//'<div class="col-xs-hidden col-sm-4"></div>' +
				'<div class="row-xs">'+
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				//'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary"></div>' +
				'<div class="col-xs-4 col-xs-offset-4 col-sm-2 col-sm-offset-3 text-center text-middle text-primary">Отсечка</div>' +
				'</div>';
			let tpl_cutoff_body = '';
			let tpl_all_body = '';
			$.each(refluxProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = i;
				let name_sensor = e["name"];
				if(sensorsRefluxSend[sensor_key].hasOwnProperty("name"))
					sensorsRefluxSend[sensor_key]["name"] = name_sensor;
				if (re_t.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["color"] = e["color"];
					sensorsRefluxSend[sensor_key]["member"] = 1;
					let tpl_delta = '';
					let tpl_delta_result = '';
					if (e["delta"]) {
						tpl_delta = returnTplHtml([{id: "reflux_delta_" + sensor_key, value: e["allertValue"], min: '0', max: '1', step: '0.05'}], deltaTempl);
						tpl_delta_result = '<span id="reflux_delta_result_' + sensor_key + '"></span><span class="hidden">&#176С</span>';
						tpl_delta_body +=
							'<div class="row row-striped">' + tpl_delta_thead +
							'<div id="alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
							'<div id="alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
							'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
							'<div class="col-xs-3 col-sm-3">' + tpl_delta + '</div>' +
							'<div class="col-xs-4 col-xs-offset-1 col-sm-2 col-sm-offset-0 text-center text-middle"><strong>' + tpl_delta_result +
							'</strong></div>' +
							'</div>' +
							'</div>';

						tpl_delta_thead = '';
					}
					let tpl_cutoff = '';
					if (e["cutoff"]) {
						tpl_cutoff = returnTplHtml([{id: "reflux_cutoff_" + sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
						tpl_cutoff_body +=
							'<div class="row row-striped">' + tpl_cutoff_thead +
							'<div id="alert_bg_' + sensor_key + '" class="pt-10 pb-10 clearfix">' +
							'<div id="alert_text_' + sensor_key + '" class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
							'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
							//'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-4 col-xs-offset-3 col-sm-2 col-sm-offset-3">' + tpl_cutoff +
							'</div>' +
							'</div>' +
							'</div>';

						tpl_cutoff_thead = '';
					}
					if (/*sensor_key !== "p1" && */!e["delta"] && !e["cutoff"]) {
						tpl_all_body += '<div class="row row-striped">' +
							'<div class="pt-10 pb-10 clearfix">' +
							'<div class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
							'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
							'<div class="col-xs-3 col-sm-3"></div>' +
							'<div class="col-xs-4 col-sm-3"></div>' +
							'</div>' +
							'</div>';
					}
				}
				if (re_out.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["member"] = 1;
					tpl_devices_body += '<div class="row row-striped">' +
						'<div class="pt-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs text-middle"><strong>' + name_sensor + '</strong></div>' +
						'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_' + sensor_key + '"></span> <span class="hidden"></span></strong></div>' +
						'</div></div>';
				}
				if (re_in.test(sensor_key) && Number(e["member"]) !== 0) {
					sensorsRefluxSend[sensor_key]["member"] = 1;
					tpl_safety_body += '<div class="row row-striped">' +
						'<div class="pt-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs text-middle"><strong>' + name_sensor + '</strong></div>' +
						'<div class="col-xs-5 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="reflux_' + sensor_key + '"></span> <span class="hidden"></span></strong></div>' +
						'</div></div>';
				}
			});
			//sensors["process"]["allow"] = 2;
			if(tpl_delta_body !== '') {
				refluxTemplate += tpl_delta_thead + tpl_delta_body;
			}
			if(tpl_cutoff_body !== '') {
				refluxTemplate += tpl_cutoff_thead + tpl_cutoff_body;
			}
			if(tpl_all_body !== '') {
				refluxTemplate += tpl_all_body;
			}
		}
		if(refluxTemplate !== '') {
			if(sensors_select)
				sendRequest("refluxSensorsSetSave", sensorsRefluxSend, "json", false, false, $("#error_reflux"),false);
			//localStorage.setObj('sensors', sensors);
			refluxTemplate = returnTplHtml([{id_value:"reflux_power_value",id_set:"reflux_power_set"}], powerTempl) + refluxTemplate + pressureTemplate + tpl_devices_body + tpl_safety_body;

			$("#reflux_start_group_button").removeClass("hidden");
			$("#svg_reflux").show();
		}else{
			$("#reflux_start_group_button").addClass("hidden");
		}
		$("#reflux_process").html(refluxTemplate);
		$("#reflux_power_set").val(refluxProcess["power"]);
		//console.log(refluxProcess);
		if(refluxProcess["start"] === true) {
			getReflux();
			$('#reflux_start').prop("disabled",true);
			$('#reflux_add_sensor').prop("disabled",true);
		}else{
			$('#reflux_add_sensor').prop("disabled",false);
			$('#reflux_stop').prop("disabled",true);
		}
	};

	//Флаг отправки данных процесса в МК
	let flagSendProcess = false;
	$(document).on('click','#reflux_start',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#reflux_add_sensor').prop("disabled",true);
		$('#reflux_stop').prop("disabled",false);
		//curStartProcess = 2;
		localStorage.setObj('oldStartProcess', 2);
		refluxProcess["start"] = flagSendProcess = true;
		//очищаем графики
		clearChart();
		startChart();
		setReflux();
		setTimeout(getReflux, 2000);
	});
	$(document).on('click','#reflux_stop',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#reflux_add_sensor').prop("disabled",false);
		$('#reflux_start').prop("disabled",false);
		$("#svg_reflux_start").css('stroke',"#000000");
		flagSendProcess = true;
		refluxProcess["start"] = false;
		//запущенный процесс в текущее время и предыдущий запущенный процесс
		let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
		if(oldStartProcess !== 2){
			//clearChart();
		}else {
			setReflux();
		}
	});
	//Установка значений для ректификации
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
		//let flag_send = false;
		let power_set = $("#reflux_power_set");
		refluxSendData["process"]["allow"] = (refluxProcess["start"] ? 2 : 0);
		if(refluxProcess["power"] !== power_set.val())
			flagSendProcess = true;
		refluxSendData["power"] = refluxProcess["power"] = power_set.val();

		$.each(refluxProcess["sensors"], function (i, e) {
			let sensor_key = e["key"];
			let reflux_delta = $("#reflux_delta_"+sensor_key);
			let reflux_cutoff = $("#reflux_cutoff_"+sensor_key);
			if(reflux_delta.length) {
				if(e["allertValue"] !== reflux_delta.val())
					flagSendProcess = true;
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_delta.val();
			}
			if(reflux_cutoff.length) {
				if(e["allertValue"] !== reflux_cutoff.val())
					flagSendProcess = true;
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_cutoff.val();
			}
		});
		//console.log(flagSendProcess);
		if(flagSendProcess) {
			//tmpSendProcess = true;
			//console.log(tmpSendProcess);
			flagSendProcess = false;
			//console.log(flagSendProcess);
			localStorage.setObj('reflux', refluxProcess);
			sendRequest("SensorsIn", refluxSendData, "json", false, false, $("#error_reflux"),false);
		}
	}
	function getReflux() {
		//console.log("getReflux");
		//очистка данных графиков
		let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
		if(oldStartProcess !== 2) {
			//clearChart();
		}
		setReflux();
		if(!$.fn.objIsEmpty(globalSensorsJson,false)) {
			let dtoJson = {};
			dtoJson["heaterPower"] = globalSensorsJson["power"];
			dtoJson["temperatures"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);
				let alert_value = Number(globalSensorsJson["sensors"][i][sensor_key]["allertValue"]);
				$.each(refluxProcess["sensors"], function (j, q) {
					//console.log(j, q);
					if (q["key"] === sensor_key) {
						q["value"] = sensor_value;
						if (sensor_key !== "p1") {
							let color_value = q["color"];
							let fillcolor = "#" + color_value;
							if (alert_value > 0 && sensor_value >= alert_value) {
								$("#alert_bg_" + sensor_key).addClass("bg-danger");
								$("#alert_text_" + sensor_key).addClass("text-danger");
								// Вибрация поддерживается
								/*if (window.navigator && window.navigator.vibrate) {
									navigator.vibrate(1000);
								}*/
							} else {
								$("#alert_bg_" + sensor_key).removeClass("bg-danger");
								$("#alert_text_" + sensor_key).removeClass("text-danger");
							}
							//console.log(sensor_key, fillcolor,sensor_value,alert_value);
							if (q["delta"] === false) {
								$("#svg_reflux_color_" + sensor_key).css('fill', colorPersent(fillcolor, sensor_value, alert_value));
							} else {
								let delta_alert = $("#reflux_delta_" + sensor_key).val();
								let delta_value = (delta_alert - alert_value + sensor_value).toFixed(2);
								//console.log(fillcolor, delta_value, delta_alert);
								$("#svg_reflux_color_" + sensor_key).css('fill', colorPersent(fillcolor, delta_value, delta_alert));
							}
						}
					}
				});
				$("#reflux_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				let allertValue = globalSensorsJson["sensors"][i][sensor_key]["allertValue"];
				allertValue = allertValue > 0 ? allertValue.toFixed(2) : "";
				if (allertValue > 0) {
					$("#reflux_delta_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#reflux_cutoff_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
				}
				if (sensor_key === "p1") {
					$("#reflux_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					dtoJson["pressure"] = globalSensorsJson["sensors"][i]["p1"]["value"];
				}
				//svg
				$("#svg_reflux_" + sensor_key).html(sensor_value.toFixed(0) + '&#176С');
				//let fillcolor = temperaturePalette(sensor_value.toFixed(0));

			});
			$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			let power_value = Number(globalSensorsJson["power"]);
			$("#reflux_power_value").text(power_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			$("#svg_reflux_ten_t").text(power_value.toFixed(0) + "%");
			$("#svg_reflux_color_ten").css('fill', colorPersent("#FF0000", power_value.toFixed(0), 100));
			dtoJson["temperatures"] = refluxProcess["sensors"];
			$("#view_distillation_chart").html("");
			//let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
			//if(oldStartProcess === 2) {
				dtoReceiver.start(dtoJson, 'view_reflux_chart');
			//}
		}
		//console.log(globalSensorsJson);
		//console.log(refluxProcess);
		if (refluxProcess["start"] === true) {
			setTimeout(getReflux, 2000);
			$("#svg_reflux_start").css('stroke', "#02b500");
		}
	}
	//Привязка датчиков к процессу дистиляции, и запуск
	let distillationProcess = {"sensors":[],"power":0,"start":false};
	$(document).on('click','#distillation_add_sensor',function(e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("distillationSensorsSetLoad", {}, "json", selectSensorsDistillation, _this, $("#error_distillation"),false);
	});
	//Запрос датчиков для дистиляции и вывод их в диалоговое окно
	function selectSensorsDistillation(data){
		let sensors = data;//sensorsJson
		//console.log(sensors);
		if(sensors !== null) {
			let section = '<section id="distillation_sensors" class="table-responsive"><table class="table table-noborder">';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)/* && key !== "p1"*/) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					//let sensor_delta = '<label class="checkbox-inline"><input disabled id="delta_' + key + '" name="distillation_radio_' + key + '" type="radio" value="Y">Уставка</label>';
					let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="distillation_radio_' + key + '" type="radio" value="Y">Отсечка</label>';
					/*if(key === "p1") {
						sensor_delta = sensor_cutoff = '';
						sensor_name = "Атмосферное давление";
					}*/
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
					let disabled_check = "";
					if(sensor_name === "")
						disabled_check = "disabled";

					section += '<tr><td>' +
						'<div class="input-group input-group-sm">'+
						'<span class="input-group-addon" style="background-color: #'+jscolor+'">' + key + '</span>'+
						'<input id="distillation_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">'+
						'<input type="hidden" id="distillation_color_' + key + '" value="'+jscolor+'">'+
						'</div></td>'+
						'<td><input '+disabled_check+' data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
						//'<td>'+sensor_delta+'</td>' +
						'<td>'+sensor_cutoff+'</td>' +
						'</tr>';
				}
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для дистиляции', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						let sensors_select = $('#distillation_sensors input[type=checkbox]');
						distillationProcess["sensors"] = $.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								//console.log($(e).data("sensor"));
								let key = $(e).data("sensor");
								let name = $("#distillation_name_" + key).val();
								let val_color = $("#distillation_color_" + key).val();
								//let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
								let color = (val_color !== "FFFFFF" && val_color !== "") ? val_color : "FFFFFF";
								//let delta = $("#delta_" + key).prop("checked");
								let cutoff = $("#cutoff_" + key).prop("checked");
								return {"key": key, "name": name, "cutoff": cutoff, "color": color, "allertValue":0, "value":0};
								//return {"key": key, "name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue":0, "value":0};
							}
						});
						//distillationProcess["sensors"] = distillation_sensors;
						localStorage.setObj('distillation', distillationProcess);
						//console.log(ar_sensors);
						$(this).closest(".modal").modal("hide");
						$.fn.pasteDistillationSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
			//jscolor.installByClassName("jscolor");
		}
	}
	$(document).on('click','#distillation_sensors input[type=checkbox]',function() {
		let checked = !$(this).prop("checked");
		//let radio_delta = $("#delta_"+$(this).data("sensor"));
		let radio_cutoff = $("#cutoff_"+$(this).data("sensor"));
		//radio_delta.prop("disabled",checked);
		radio_cutoff.prop("disabled",checked);
		if(checked) {
			//radio_delta.prop("checked", false);
			radio_cutoff.prop("checked",false);
		}
	});

	$.fn.pasteDistillationSensors = function(sensors_select){
		let sensorsDistillationSend = {
			"t1":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t2":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t3":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t4":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t5":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t6":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t7":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t8":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0}
			//,"p1":{"value":760.00,"color":0,"member":0}
		};
		let distillationTemplate = '';
		let localDistillation = localStorage.getObj('distillation');
		if(localDistillation !== null)
			distillationProcess = localDistillation;
		if(distillationProcess["sensors"].length > 0) {
			/*let tpl_delta_thead =
				'<div class="row-xs">'+
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-sm-3 text-center text-middle text-primary">Дельта</div>' +
				'<div class="col-xs-4 col-sm-2 text-center text-middle text-primary">Уставка</div>'+
				'</div>';
			let tpl_delta_body = '';*/
			let tpl_cutoff_thead =
				'<div class="row-xs">'+
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-xs-offset-4 col-sm-2 col-sm-offset-3 text-center text-middle text-primary">Отсечка</div>' +
				'</div>';
			let tpl_cutoff_body = '';
			let tpl_all_body = '';
			$.each(distillationProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = e["key"];
				let name_sensor = e["name"];
				if(sensorsDistillationSend[sensor_key].hasOwnProperty("name"))
					sensorsDistillationSend[sensor_key]["name"] = name_sensor;
				sensorsDistillationSend[sensor_key]["color"] = hex2dec(e["color"]);
				sensorsDistillationSend[sensor_key]["member"] = 1;
				/*let tpl_delta = '';
				let tpl_delta_result = '';
				if (e["delta"]) {
					tpl_delta = returnTplHtml([{id:"distillation_delta_"+sensor_key, value: e["allertValue"], min: '0', max: '1', step: '0.05'}], deltaTempl);
					tpl_delta_result = '<span id="distillation_delta_result_'+sensor_key+'"></span><span class="hidden">&#176С</span>';
					tpl_delta_body +=
						'<div class="row row-striped">' + tpl_delta_thead +
						'<div id="alert_bg_'+sensor_key+'" class="pt-10 pb-10 clearfix">' +
						'<div id="alert_text_'+sensor_key+'" class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
						'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="distillation_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
						'<div class="col-xs-3 col-sm-3">' + tpl_delta + '</div>' +
						'<div class="col-xs-4 col-xs-offset-1 col-sm-2 col-sm-offset-0 text-center text-middle"><strong>' + tpl_delta_result +
						'</strong></div>' +
						'</div>' +
						'</div>';

					tpl_delta_thead = '';
				}*/
				let tpl_cutoff = '';
				if (e["cutoff"]) {
					tpl_cutoff = returnTplHtml([{id:"distillation_cutoff_"+sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
					tpl_cutoff_body +=
						'<div class="row row-striped">' + tpl_cutoff_thead +
						'<div id="alert_bg_'+sensor_key+'" class="pt-10 pb-10 clearfix">' +
						'<div id="alert_text_'+sensor_key+'" class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
						'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="distillation_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
						//'<div class="col-xs-3 col-sm-3"></div>' +
						'<div class="col-xs-4 col-xs-offset-3 col-sm-2 col-sm-offset-3">' + tpl_cutoff +
						'</div>' +
						'</div>' +
						'</div>';

					tpl_cutoff_thead = '';
				}
				if(/*sensor_key !== "p1" && */!e["delta"] && !e["cutoff"]) {
					tpl_all_body += '<div class="row row-striped">' +
						'<div class="pt-10 pb-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
						'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="distillation_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
						'<div class="col-xs-3 col-sm-3"></div>' +
						'<div class="col-xs-4 col-sm-3"></div>' +
						'</div>' +
						'</div>';
				}
			});
			//sensors["process"]["allow"] = 2;
			/*if(tpl_delta_body !== '') {
				distillationTemplate += tpl_delta_thead + tpl_delta_body;
			}*/
			if(tpl_cutoff_body !== '') {
				distillationTemplate += tpl_cutoff_thead + tpl_cutoff_body;
			}
			if(tpl_all_body !== '') {
				distillationTemplate += tpl_all_body;
			}
		}
		if(distillationTemplate !== '') {
			if(sensors_select)
				sendRequest("distillationSensorsSetSave", sensorsDistillationSend, "json", false, false, $("#error_distillation"),false);
			//localStorage.setObj('sensors', sensors);
			distillationTemplate = returnTplHtml([{id_value:"distillation_power_value",id_set:"distillation_power_set"}], powerTempl) + distillationTemplate/* + pressureTemplate*/;
			$("#distillation_start_group_button").removeClass("hidden");
			$("#svg_distillation").show();
		}else{
			$("#distillation_start_group_button").addClass("hidden");
		}
		$("#distillation_process").html(distillationTemplate);
		$("#distillation_power_set").val(distillationProcess["power"]);
		//console.log(distillationProcess);
		if(distillationProcess["start"] === true) {
			getDistillation();
			$('#distillation_start').prop("disabled",true);
			$('#distillation_add_sensor').prop("disabled",true);
		}else{
			$('#distillation_stop').prop("disabled",true);
			$('#distillation_add_sensor').prop("disabled",false);
		}
	};
	$(document).on('click','#distillation_start',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#distillation_add_sensor').prop("disabled",true);
		$('#distillation_stop').prop("disabled",false);
		distillationProcess["start"] = flagSendProcess = true;
		//curStartProcess = 1;
		localStorage.setObj('oldStartProcess', 1);
		//очищаем графики
		clearChart();
		startChart();
		setDistillation();
		setTimeout(getDistillation, 2000);
	});
	$(document).on('click','#distillation_stop',function() {
		let _this = $(this);
		_this.prop("disabled",true);
		$('#distillation_add_sensor').prop("disabled",false);
		$('#distillation_start').prop("disabled",false);
		$("#svg_distillation_start").css('stroke',"#000000");
		flagSendProcess = true;
		distillationProcess["start"] = false;
		//запущенный процесс в текущее время и предыдущий запущенный процесс
		let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
		if(oldStartProcess !== 1){
			//clearChart();
		}else {
			setDistillation();
		}
	});
	//Установка значений для дистиляции
	function setDistillation() {
		let distillationSendData = {
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
		//let flag_send = false;
		let power_set = $("#distillation_power_set");
		distillationSendData["process"]["allow"] = (distillationProcess["start"] ? 1 : 0);
		if(distillationProcess["power"] !== power_set.val())
			flagSendProcess = true;
		distillationSendData["power"] = distillationProcess["power"] = power_set.val();

		$.each(distillationProcess["sensors"], function (i, e) {
			let sensor_key = e["key"];
			//let distillation_delta = $("#distillation_delta_"+sensor_key);
			let distillation_cutoff = $("#distillation_cutoff_"+sensor_key);
			/*if(distillation_delta.length) {
				if(e["allertValue"] !== distillation_delta.val())
					flagSendProcess = true;
				distillationSendData[sensor_key]["allertValue"] = e["allertValue"] = distillation_delta.val();
			}*/
			if(distillation_cutoff.length) {
				if(e["allertValue"] !== distillation_cutoff.val())
					flagSendProcess = true;
				distillationSendData[sensor_key]["allertValue"] = e["allertValue"] = distillation_cutoff.val();
			}
		});
		//console.log(flagSendProcess);
		if(flagSendProcess) {
			flagSendProcess = false;
			localStorage.setObj('distillation', distillationProcess);
			sendRequest("SensorsIn", distillationSendData, "json", false, false, $("#error_distillation"),false);
		}
	}
	function getDistillation() {
		//console.log("getDistillation");
		//очистка данных графиков
		let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
		if(oldStartProcess !== 1) {
			//clearChart();
		}
		setDistillation();
		if(!$.fn.objIsEmpty(globalSensorsJson,false)) {
			let dtoJson = {};
			dtoJson["heaterPower"] = globalSensorsJson["power"];
			dtoJson["temperatures"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				let sensor_value = Number(globalSensorsJson["sensors"][i][sensor_key]["value"]);
				let alert_value = Number(globalSensorsJson["sensors"][i][sensor_key]["allertValue"]);
				$.each(distillationProcess["sensors"], function (j, q) {
					//console.log(j, q);
					if (q["key"] === sensor_key) {
						q["value"] = sensor_value;
						if (sensor_key !== "p1") {
							let color_value = q["color"];
							let fillcolor = "#" + color_value;
							if (alert_value > 0 && sensor_value >= alert_value) {
								$("#alert_bg_" + sensor_key).addClass("bg-danger");
								$("#alert_text_" + sensor_key).addClass("text-danger");
								// Вибрация поддерживается
								/*if (window.navigator && window.navigator.vibrate) {
									navigator.vibrate(1000);
								}*/
							} else {
								$("#alert_bg_" + sensor_key).removeClass("bg-danger");
								$("#alert_text_" + sensor_key).removeClass("text-danger");
							}
							//console.log(sensor_key, fillcolor,sensor_value,alert_value);
							//if (q["delta"] === false) {
								$("#svg_distillation_color_" + sensor_key).css('fill', colorPersent(fillcolor, sensor_value, alert_value));
							/*} else {
								let delta_alert = $("#distillation_delta_" + sensor_key).val();
								let delta_value = (delta_alert - alert_value + sensor_value).toFixed(2);
								//console.log(fillcolor, delta_value, delta_alert);
								$("#svg_distillation_color_" + sensor_key).css('fill', colorPersent(fillcolor, delta_value, delta_alert));
							}*/
						}
					}
				});
				$("#distillation_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
				let allertValue = globalSensorsJson["sensors"][i][sensor_key]["allertValue"];
				allertValue = allertValue > 0 ? allertValue.toFixed(2) : "";
				if (allertValue > 0) {
					//$("#distillation_delta_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#distillation_cutoff_result_" + sensor_key).text(allertValue).parent().find(".hidden").removeClass("hidden").addClass("show");
				}
				/*if (sensor_key === "p1") {
					$("#distillation_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					dtoJson["pressure"] = globalSensorsJson["sensors"][i]["p1"]["value"];
				}*/
				//svg
				$("#svg_distillation_" + sensor_key).html(sensor_value.toFixed(0) + '&#176С');
				//let fillcolor = temperaturePalette(sensor_value.toFixed(0));

			});
			//$("#distillation_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			let power_value = Number(globalSensorsJson["power"]);
			$("#distillation_power_value").text(power_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			$("#svg_distillation_ten_t").text(power_value.toFixed(0) + "%");
			$("#svg_distillation_color_ten").css('fill', colorPersent("#FF0000", power_value.toFixed(0), 100));
			dtoJson["temperatures"] = distillationProcess["sensors"];
			$("#view_reflux_chart").html("");
			//let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
			//if(oldStartProcess === 1) {
				//clearChart();
				dtoReceiver.start(dtoJson, 'view_distillation_chart');
			//}
		}
		if (distillationProcess["start"] === true) {
			setTimeout(getDistillation, 2000);
			$("#svg_distillation_start").css('stroke', "#02b500");
		}
	}
	//Привязка датчиков к процессу затирания, и запуск
	let brewingProcess = {"sensors":[],"power":0,"start":false};
	$(document).on('click','#brewing_add_sensor',function(e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("brewingSensorsSetLoad", {}, "json", selectSensorsBrewing, _this, $("#error_brewing"),false);
	});
	//Запрос датчиков для затирания и вывод их в диалоговое окно
	function selectSensorsBrewing(data){
		let sensors = data;//sensorsJson
		//console.log(sensors);
		if(sensors !== null) {
			let section = '<section id="brewing_sensors" class="table-responsive"><table class="table table-noborder">';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)/* && key !== "p1"*/) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="brewing_radio_' + key + '" type="radio" value="Y">Отсечка</label>';
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
					let disabled_check = "";
					if(sensor_name === "")
						disabled_check = "disabled";

					section += '<tr><td>' +
						'<div class="input-group input-group-sm">'+
						'<span class="input-group-addon" style="background-color: #'+jscolor+'">' + key + '</span>'+
						'<input id="brewing_name_' + key + '" class="form-control input-sm" type="text" value="' + sensor_name + '">'+
						'<input type="hidden" id="brewing_color_' + key + '" value="'+jscolor+'">'+
						'</div></td>'+
						'<td><input '+disabled_check+' data-sensor="' + key + '" type="checkbox" value="' + key + '"></td>' +
						'<td>'+sensor_cutoff+'</td>' +
						'</tr>';
				}
			}
			section += '</table></section>';
			$.fn.openModal('Выбор датчиков для затирания', section, "modal-md", false, {
					text: "Выбрать",
					id: "sensors_select",
					class: "btn btn-success",
					click: function () {
						let sensors_select = $('#brewing_sensors input[type=checkbox]');
						brewingProcess["sensors"] = $.map(sensors_select, function (e) {
							if ($(e).is(":checked")) {
								//console.log($(e).data("sensor"));
								let key = $(e).data("sensor");
								let name = $("#brewing_name_" + key).val();
								let val_color = $("#brewing_color_" + key).val();
								//let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
								let color = (val_color !== "FFFFFF" && val_color !== "") ? val_color : "FFFFFF";
								let cutoff = $("#cutoff_" + key).prop("checked");
								return {"key": key, "name": name, "cutoff": cutoff, "color": color, "allertValue":0, "value":0};
							}
						});
						localStorage.setObj('brewing', brewingProcess);
						//console.log(ar_sensors);
						$(this).closest(".modal").modal("hide");
						$.fn.pasteBrewingSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
		}
	}
	$(document).on('click','#brewing_sensors input[type=checkbox]',function() {
		let checked = !$(this).prop("checked");
		let radio_cutoff = $("#cutoff_"+$(this).data("sensor"));
		radio_cutoff.prop("disabled",checked);
		if(checked) {
			radio_cutoff.prop("checked",false);
		}
	});
	$.fn.pasteBrewingSensors = function(sensors_select){
		let sensorsBrewingSend = {
			"t1":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t2":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t3":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t4":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t5":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t6":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t7":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t8":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0}
		};
		let brewingTemplate = '';
		let localBrewing = localStorage.getObj('brewing');
		if(localBrewing !== null)
			brewingProcess = localBrewing;
		if(brewingProcess["sensors"].length > 0) {
			let tpl_cutoff_thead =
				'<div class="row-xs">'+
				'<div class="col-xs-4 col-xs-offset-0 col-sm-3 col-sm-offset-4 text-center text-middle text-primary">Значение</div>' +
				'<div class="col-xs-4 col-xs-offset-4 col-sm-2 col-sm-offset-3 text-center text-middle text-primary">Отсечка</div>' +
				'</div>';
			let tpl_cutoff_body = '';
			let tpl_all_body = '';
			$.each(brewingProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = e["key"];
				let name_sensor = e["name"];
				if(sensorsBrewingSend[sensor_key].hasOwnProperty("name"))
					sensorsBrewingSend[sensor_key]["name"] = name_sensor;
				sensorsBrewingSend[sensor_key]["color"] = hex2dec(e["color"]);
				sensorsBrewingSend[sensor_key]["member"] = 1;
				let tpl_cutoff = '';
				if (e["cutoff"]) {
					tpl_cutoff = returnTplHtml([{id:"brewing_cutoff_"+sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
					tpl_cutoff_body +=
						'<div class="row row-striped">' + tpl_cutoff_thead +
						'<div id="alert_bg_'+sensor_key+'" class="pt-10 pb-10 clearfix">' +
						'<div id="alert_text_'+sensor_key+'" class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
						'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="brewing_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
						//'<div class="col-xs-3 col-sm-3"></div>' +
						'<div class="col-xs-4 col-xs-offset-3 col-sm-2 col-sm-offset-3">' + tpl_cutoff +
						'</div>' +
						'</div>' +
						'</div>';

					tpl_cutoff_thead = '';
				}
				if(/*sensor_key !== "p1" && */!e["delta"] && !e["cutoff"]) {
					tpl_all_body += '<div class="row row-striped">' +
						'<div class="pt-10 pb-10 clearfix">' +
						'<div class="col-xs-12 col-sm-4 text-center-xs"><strong>t&#176' + name_sensor + '</strong></div>' +
						'<div class="col-xs-3 col-xs-offset-1 col-sm-3 col-sm-offset-0 text-center text-middle"><strong><span id="brewing_' + sensor_key + '"></span><span class="hidden">&#176С</span></strong></div>' +
						'<div class="col-xs-3 col-sm-3"></div>' +
						'<div class="col-xs-4 col-sm-3"></div>' +
						'</div>' +
						'</div>';
				}
			});
			//sensors["process"]["allow"] = 2;
			if(tpl_cutoff_body !== '') {
				brewingTemplate += tpl_cutoff_thead + tpl_cutoff_body;
			}
			if(tpl_all_body !== '') {
				brewingTemplate += tpl_all_body;
			}
		}
		if(brewingTemplate !== '') {
			if(sensors_select)
				sendRequest("brewingSensorsSetSave", sensorsBrewingSend, "json", false, false, $("#error_brewing"),false);
			//localStorage.setObj('sensors', sensors);
			brewingTemplate = returnTplHtml([{id_value:"brewing_power_value",id_set:"brewing_power_set"}], powerTempl) + brewingTemplate/* + pressureTemplate*/;
			$("#brewing_start_group_button").removeClass("hidden");
			$("#svg_brewing").show();
		}else{
			$("#brewing_start_group_button").addClass("hidden");
		}
		$("#brewing_process").html(brewingTemplate);
		$("#brewing_power_set").val(brewingProcess["power"]);
		//console.log(brewingProcess);
		if(brewingProcess["start"] === true) {
			getBrewing();
			$('#brewing_start').prop("disabled",true);
			$('#brewing_add_sensor').prop("disabled",true);
		}else{
			$('#brewing_stop').prop("disabled",true);
			$('#brewing_add_sensor').prop("disabled",false);
		}
	};
	//заполнение разных полей данными датчиков
	function fillSensorsData() {
		//$("#width_page").css({"border-bottom":"solid 1px red","text-align":"center","color":"red"}).text("thisWidth: "+$("#width_page").outerWidth()+", documentWidth: "+$(document).width());
		if(!$.fn.objIsEmpty(globalSensorsJson,false)) {
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				//заполнение вкладки датчики
				let sensor_value = globalSensorsJson["sensors"][i][sensor_key]["value"];
				if($("#sensor_val_" + sensor_key).length && sensor_value < 150) {
					$("#sensor_val_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#svg_sensor_" + sensor_key).html(sensor_value.toFixed(0) + '&#176С');
				}
				let process = Number(globalSensorsJson["process"]["allow"]);
				//очистка данных графиков
				let oldStartProcess = Number(localStorage.getItem('oldStartProcess'));
				if(process > 0 && oldStartProcess !== process) {
					clearChart();
				}
				//заполнение процесса ректификации
				if(refluxProcess["start"] !== true /*&& $.trim($("#reflux_process").html()) !== ""*/){
					$("#reflux_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					if (sensor_key === "p1") {
						$("#reflux_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");

						$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#reflux_power_value").text(globalSensorsJson["power"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					}

					$("#svg_reflux_" + sensor_key).html(sensor_value.toFixed(0)+'&#176С');
					$("#svg_reflux_ten_t").text(globalSensorsJson["power"]+'%');

					if(process === 2) {
						$('li.swipe-tab a[data-target="#reflux"]').tab('show');
						$("#reflux_start").trigger("click");
					}

					/*if(process > 0 && process !== 2)
						$("#reflux_start_group_button").addClass("hidden");
					if(process === 0)
						$("#reflux_start_group_button").removeClass("hidden");*/
				}
				if(refluxProcess["start"] === true){
					if(process !== 2) {
						$("#reflux_stop").trigger("click");
					}
				}
				//заполнение процесса дистиляции
				if(distillationProcess["start"] !== true /*&& $.trim($("#distillation_process").html()) !== ""*/){
					$("#distillation_" + sensor_key).text(sensor_value.toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					/*if (sensor_key === "p1") {
						$("#distillation_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");

						$("#distillation_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");

					}*/
					$("#distillation_power_value").text(globalSensorsJson["power"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#svg_distillation_" + sensor_key).html(sensor_value.toFixed(0)+'&#176С');
					$("#svg_distillation_ten_t").text(globalSensorsJson["power"]+'%');

					if(process === 1) {
						$('li.swipe-tab a[data-target="#distillation"]').tab('show');//$("#distillation").tab('show');
						$("#distillation_start").trigger("click");
					}

					/*if(process > 0 && process !== 1)
						$("#distillation_start_group_button").addClass("hidden");
					if(process === 0)
						$("#distillation_start_group_button").removeClass("hidden");*/
				}
				if(distillationProcess["start"] === true){
					if(process !== 1) {
						$("#distillation_stop").trigger("click");
					}
				}

				if(process > 0 && process !== 2) {
					$("#reflux_start_group_button").addClass("hidden");
				}
				if(process > 0 && process !== 1) {
					$("#distillation_start_group_button").addClass("hidden");
				}
				if(process === 0) {
					$("#reflux_start_group_button").removeClass("hidden");
					$("#distillation_start_group_button").removeClass("hidden");
				}
			});
		}
	}
	//запрос постоянно всех датчиков
	//let tmpTime = 1;
	function getIntervalSensors() {
		$.ajax({
			//url: 'sensors.php',
			url: 'SensorsOut',
			//data: {"t":tmpTime},
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				//console.log('Sensors',msg);
				globalSensorsJson = msg;
				fillSensorsData();
			}
		});
		//if(tmpTime<100 && refluxProcess["start"] === true)
			//tmpTime ++;
	}
	//clearInterval(sensorsIntervalId);
	sensorsIntervalId = setInterval(getIntervalSensors,1000);

////////////////////////////////////////////////////////////////




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
			}
		});
	}
    //Обновление прошивки
	$("#file_update").on("change",function () {
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
		sendRequest("ssdp",{"ssdp":ssdp},"text",false,_this,$("#error_settings"),false);
	});
	$("#settings_set_ssid").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssid = $("#settings_ssid").val();
		let pass = $("#settings_password").val();
		sendRequest("ssid",{"ssid":ssid,"password":pass},"text",false,_this,$("#error_settings"),'<p class="text-center text-success"><strong>Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.</strong></p>');
	});
	$("#settings_set_ssidap").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let ssidap = $("#settings_ssidap").val();
		let pass = $("#settings_passwordap").val();
		sendRequest("ssidap",{"ssidAP":ssidap,"passwordAP":pass},"text",false,_this,$("#error_settings"),'<p class="text-center text-success"><strong>Изменения вступят в силу после перезагрузки. Пожалуйста перезагрузите устройство.</strong></p>');
	});
	$("#settings_auto_timezone").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let date = new Date();
		let timezone = Math.abs(date.getTimezoneOffset()/60);
		$("#settings_timezone").val(timezone);
		sendRequest("TimeZone",{"timezone":timezone},"text",false,_this,$("#error_settings"),'<p class="text-center text-success"><strong>Изменения временной зоны сохранены</strong></p>');
	});
	$("#settings_set_timezone").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		let timezone = $("#settings_timezone").val();
		sendRequest("TimeZone",{"timezone":timezone},"text",false,_this,$("#error_settings"),'<p class="text-center text-success"><strong>Изменения временной зоны сохранены</strong></p>');
	});
	$("#settings_restart").on("click",function (e) {
		e.preventDefault();
		let _this = $(this);
		$.fn.openModal('', '<p class="text-center text-danger">Вы действительно хотите перезагрузить устройство?</p>', "modal-sm", false, [{
			text: "Да",
			id: "return_restart",
			class: "btn btn-primary btn-sm",
			click: function () {
				$(this).closest(".modal").modal("hide");
				sendRequest("restart",{"device":"ok"},"text",false,_this,false,false);
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


	//TODO не используется все что ниже
	//Затирание
	// нужно думать с этими переменными (согласовать с ардуино), что бы можно было запускать затирание с любого шага
	let start_brewing = 0;
	let step_brewing = 0;
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

		sendRequest("SettingBrewing",data_send,"text",false,$(this),$("#error_brewing"),false);
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

	$("#start_brewing").on("click",function () {
		startBrewing(1,1);
	});

	$("#stop_brewing").on("click",function () {
		start_brewing = 0;
		step_brewing = 0;
		stopBrewing(1,1);
		sendRequest("SetHeaterPower",{"heaterPower":0},"text",false,$(this),$("#error_brewing"),false);
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

		sendRequest("SetHeaterPower",{"heaterPower":pover},"text",false,$(this),$("#error_heater"),false);
	});

	setTimeout(getSettings, 2000);
	//setInterval(getDistillation,2000);
});