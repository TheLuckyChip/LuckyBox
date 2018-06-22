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
	var globalSensorsJson = {};
	var sensorsIntervalId = 0;
	//Глобальный объект dtoReceiver служит для опроса МК.
	var dtoReceiver = {
		dtos: [],                               // Контейнер состояний в ОЗУ
		frequencyRecordingToLocalStorage: 5,    // Частота архивации (Через сколько опросов осуществляется запись в localStorage)
		reqestDelayDefalt: 1000,

		dtoGet: function (json) {
			const self = dtoReceiver;  // Для доступа к this в jquery
			let requestCounter = 0;    // Счётчик запросов, служит для записи в localStorage каждые frequencyRecordingToLocalStorage раз

			self.dtoCurrent = json;
			self.dtoCurrent.dateTime = (new Date()).getTime();  // Пользуемся временем клиента, для несчастных без доступа к NTP

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
			$(document).trigger('newDTOreceived', [self.dtoCurrent]);
		},

		// Очистка LocalStorage
		clearDeviceConditions: function() {
			this.dtos = [];
			localStorage.removeItem('dtos');
		},
		// Запуск опроса ESP
		start: function (dtoJson) {
			//let dtoJson = {};
			//dtoJson["heaterPower"] = globalSensorsJson["power"];
			//dtoJson["temperatures"] = globalSensorsJson["sensors"];
			console.log(dtoJson);
			this.dtoGet(dtoJson);
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

	var plot;
	//TODO сделать передачу в «getPlot» ID контейнера, для вывода графиков в требуемом процессе, и скорее всего еще сами датчики (для каждого процесса разные) или он сам подтянет нужные?
	$(document).one("newDTOreceived", function (e) {
		console.log(e);
		plot = getPlot();
	});

	$('#plotClear').click(function() {
		dtoReceiver.clearDeviceConditions();
		plot.series.forEach(function (s) { s.setData([]) });

		plot.redraw();
	});

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
		console.log("Запуск графиков!");

		let plotNew = Highcharts.stockChart('viewPort', {

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
					x: 10,
				},
				max: 100,
				opposite: false
			}
				, { // Secondary yAxis
					gridLineWidth: 0,
					title: {
						text: 'Атмосферное давление',
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
				}
			],

			series: [
				{
					name: "Мощность", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 0, showInLegend: true,
					data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.heaterPower] })
				},
				{
					name: "Атмосферное давление", yAxis: 2, type: "area", step: 'left', fillOpacity: 0.05, color: "#00e4f0", lineWidth: 1, showInLegend: true,
					data: dtoReceiver.dtos.map(function (dc) {
						return [dc.dateTime, dc.pressure]
					})
					/*,
					fillColor: {
						linearGradient: {
							x1: 0,
							y1: 0,
							x2: 0,
							y2: 1
						},
						stops: [
							[0, Highcharts.getOptions().colors[0]],
							[1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
						]
					},
					marker: {
						radius: 2
					}*/
					//,
					//lineWidth: 1,
					// states: {
					// 	hover: {
					// 		lineWidth: 1
					// 	}
					// },
					// threshold: null
				}
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
			/*plotOptions: {
				line: {
					dataLabels: {
						enabled: true
					},
					enableMouseTracking: false
				}
			},*/
			/*plotOptions: {
				area: {
					fillColor: {
						linearGradient: {
							x1: 0,
							y1: 0,
							x2: 0,
							y2: 1
						},
						stops: [
							[0, Highcharts.getOptions().colors[0]],
							[1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
						]
					},
					marker: {
						radius: 2
					},
					lineWidth: 1,
					states: {
						hover: {
							lineWidth: 1
						}
					},
					threshold: null
				}
			},*/
			title: {
				text: 'Данные датчиков'
			},
			legend: {
				enabled: true
			},
			exporting: {
				enabled: true
			},
		});

		dtoReceiver.dtos[0].temperatures.forEach(function(t, i) {
			//console.log(t, i);
			if( t["key"] !== "p1") {
				plotNew.addSeries({
					name: t["name"],
					color: "#"+dec2hex(t["color"]),
					data: dtoReceiver.dtos.map(function (dc) {
						//console.log(dc.temperatures[i]["value"]);
						return [dc.dateTime, dc.temperatures[i]["value"]]
					})
				});
			//}else{

			}
		});


		$(document).on("newDTOreceived", function (e, dto) {

			plot.series[0].addPoint([dto.dateTime, dto.heaterPower], false);
			plot.series[1].addPoint([dto.dateTime, dto.pressure], false);
			let count = 1;
			dto.temperatures.forEach(function(t,i) {
				//console.log(t,i);
				if( t["key"] !== "p1"){
					//plot.series[1].addPoint([dto.dateTime, dto.temperatures[i]["value"]], false);
					//console.log(t,i);
				//}else {
					plotNew.series[count + 1].addPoint([dto.dateTime, dto.temperatures[i]["value"]], false);
				}
				count++;
			});
			plotNew.redraw();
		});

		return plotNew;
	}

	$(document).on('click','#get_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		sendRequest("sensorsOutSet",{},"json",getSensors,_this,$("#error_sensors"));
	});

	$(document).on('change',"#sensors_settings select",function () {
        let arrSortSensors = [];
        let _this = $(this);
        let _this_val = _this.val();
        /*if(_this_val !== ""){
        	$('#sensors_settings select option[value="'+_this_val+'"]').hide();
		}else{*/
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
		//}
    });
	function getSensors(data) {
		//console.log(data);
		let sensors = data;
		for (let key in sensors) {
			if(sensors.hasOwnProperty(key)) {
				let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF";
				if (sensors[key]["value"] < 150) {
					$("#sensor_name_" + key).val(sensors[key]["name"]);
					$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
					$("#sensor_val_" + key).text(sensors[key]["value"]).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#svg_sensor_" + key).html(sensors[key]["value"]+'&#176С');
					$("#svg_sensor_color_" + key).css('fill',jscolor);
				} else {
					if (!$.fn.objIsEmpty(sensors[key]["name"], false))
						$("#sensor_name_" + key).val("");
					$("#sensor_color_" + key).val(jscolor).next("button").css("background-color", "#" + jscolor);
					$("#sensor_val_" + key).text("").parent().find(".show").removeClass("show").addClass("hidden");
				}
			}
		}
		sensorsJson = data;
		//localStorage.setObj('dtos', self.dtos);
	}
	$(document).on('click','#set_sensors',function(e) {
		e.preventDefault();
		let _this = $(this);
		let nameError = false;
        let arrSortSensors = [1,2,3,4,5,6,7,8];
		let sensorsSend = {};
		for (let key in sensorsJson) {
			if(sensorsJson.hasOwnProperty(key)){
				sensorsSend[key] = {};
				let val_color = $("#sensor_color_"+key).val();
				if($("#sensor_name_"+key).val() === ""  && sensorsJson[key]["value"]<150)
					nameError = true;
				sensorsSend[key]["name"] = sensorsJson[key]["name"] = $("#sensor_name_"+key).val();
				sensorsSend[key]["color"] = sensorsJson[key]["color"] = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;

                if(key !== "p1") {
                	let sort_number = Number($("#sensor_number_"+key).val());
                    if(sort_number !== 0)
                        $.fn.arrayUnset(arrSortSensors, sort_number);
                    sensorsSend[key]["number"] = sensorsJson[key]["number"] = sort_number;
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
			sendRequest("sensorsInSet", sensorsSend, "json", setSensors, _this, $("#error_sensors"));
		}
	});
	function setSensors(data) {
		//console.log(data);
        sendRequest("sensorsOutSet",{},"json",getSensors,$('#get_sensors'),$("#error_sensors"));
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
		let _this = $(this);
		sendRequest("refluxSensorsSetLoad", {}, "json", selectSensorsReflus, _this, $("#error_reflux"));
		/*let sensors = localStorage.getObj('sensors');//sensorsJson
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
							let name = $("#reflux_name_" + key).val();
							let val_color = $("#reflux_color_" + key).val();
							let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
							let delta = $("#delta_" + key).prop("checked");
							let cutoff = $("#cutoff_" + key).prop("checked");
							return {"key": key, "name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue":0};
						}
					});
					//refluxProcess["sensors"] = reflux_sensors;
					localStorage.setObj('reflux', refluxProcess);
					//console.log(ar_sensors);
					$(this).closest(".modal").modal("hide");
					pasteRefluxSensors(true);
				}
			},
			{id: "modal_sensors_select"}
			);
			//jscolor.installByClassName("jscolor");
		}*/
	});
	//Запрос датчиков для ректификации и вывод их в диалоговое окно
	function selectSensorsReflus(data){
		let sensors = data;//sensorsJson
		//console.log(sensors);
		if(sensors !== null) {
			let section = '<section id="reflux_sensors" class="table-responsive"><table class="table table-noborder">';
			for (let key in sensors) {
				if (sensors.hasOwnProperty(key)) {
					let sensor_name = (sensors[key].hasOwnProperty("name") ? sensors[key]["name"] : "");
					let sensor_delta = '<label class="checkbox-inline"><input disabled id="delta_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Уставка</label>';
					let sensor_cutoff = '<label class="checkbox-inline"><input disabled id="cutoff_' + key + '" name="reflux_radio_' + key + '" type="radio" value="Y">Отсечка</label>';
					if(key === "p1") {
						sensor_delta = sensor_cutoff = '';
						sensor_name = "Атмосферное давление";
					}
					let jscolor = sensors[key]["color"] > 0 ? dec2hex(sensors[key]["color"]) : "FFFFFF"; //"background-color","#"+jscolor
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
						'<td>'+sensor_delta+'</td>' +
						'<td>'+sensor_cutoff+'</td>' +
						'</tr>';
				}
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
								let name = $("#reflux_name_" + key).val();
								let val_color = $("#reflux_color_" + key).val();
								let color = (val_color !== "FFFFFF" && val_color !== "") ? hex2dec(val_color) : 0;
								let delta = $("#delta_" + key).prop("checked");
								let cutoff = $("#cutoff_" + key).prop("checked");
								return {"key": key, "name": name, "delta": delta, "cutoff": cutoff, "color": color, "allertValue":0, "value":0};
							}
						});
						//refluxProcess["sensors"] = reflux_sensors;
						localStorage.setObj('reflux', refluxProcess);
						//console.log(ar_sensors);
						$(this).closest(".modal").modal("hide");
						pasteRefluxSensors(true);
					}
				},
				{id: "modal_sensors_select"}
			);
			//jscolor.installByClassName("jscolor");
		}
	}
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
	pasteRefluxSensors = function(sensors_select){
		//let sensors = localStorage.getObj('sensors');//sensorsJson
		let sensorsRefluxSend = {
			"t1":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t2":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t3":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t4":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t5":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t6":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t7":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"t8":{"value":150.00,"name":"","color":0,"member":0,"priority":0,"allertValue":0},
			"p1":{"value":760.00,"color":0,"member":0}
		};
		let refluxTemplate = '';
		let localReflux = localStorage.getObj('reflux');
		if(localReflux !== null)
			refluxProcess = localReflux;
		if(refluxProcess["sensors"].length > 0) {
			$.each(refluxProcess["sensors"], function (i, e) {
				//console.log(i,e);
				let sensor_key = e["key"];
				let name_sensor = e["name"];
				if(sensorsRefluxSend[sensor_key].hasOwnProperty("name"))
					sensorsRefluxSend[sensor_key]["name"] = name_sensor;
				sensorsRefluxSend[sensor_key]["color"] = e["color"];
				sensorsRefluxSend[sensor_key]["member"] = 1;
				let tpl_delta = '';
				let tpl_delta_result = '';
				if (e["delta"]) {
					tpl_delta = returnTplHtml([{id:"reflux_delta_"+sensor_key, value: e["allertValue"], min: '0', max: '1', step: '0.05'}], deltaTempl);
					tpl_delta_result = '<span id="reflux_delta_result_'+sensor_key+'"></span> <span class="hidden">&#176С</span>'
				}
				if (e["cutoff"]) {
					tpl_delta = returnTplHtml([{id:"reflux_cutoff_"+sensor_key, value: e["allertValue"], min: '0', max: '105', step: '0.5'}], deltaTempl);
					tpl_delta_result = '<span id="reflux_cutoff_result_'+sensor_key+'"></span> <span class="hidden">&#176С</span>'
				}
				if(sensor_key !== "p1") {
					refluxTemplate += '<tr><td>t&#176' + name_sensor + '</td>' +
						'<td><span id="reflux_' + sensor_key + '"></span> <span class="hidden">&#176С</span></td>' +
						'<td>' + tpl_delta + '</td>' +
						'<td>' + tpl_delta_result + '</td>' +
						'</tr>'
				}
			});
			//sensors["process"]["allow"] = 2;
		}
		if(refluxTemplate !== '') {
			if(sensors_select)
				sendRequest("refluxSensorsSetSave", sensorsRefluxSend, "json", false, false, $("#error_reflux"));
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
			//dtoReceiver.start();
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
		//dtoReceiver.start();
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
		//dtoReceiver.stop();
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
		refluxSendData["process"]["allow"] = (refluxProcess["start"] ? 2 : 0);
		if(refluxProcess["power"] !== power_set.val())
			flag_send = true;
		refluxSendData["power"] = refluxProcess["power"] = power_set.val();

		$.each(refluxProcess["sensors"], function (i, e) {
			let sensor_key = e["key"];
			let reflux_delta = $("#reflux_delta_"+sensor_key);
			let reflux_cutoff = $("#reflux_cutoff_"+sensor_key);
			if(reflux_delta.length) {
				if(e["allertValue"] !== reflux_delta.val())
					flag_send = true;
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_delta.val();
			}
			if(reflux_cutoff.length) {
				if(e["allertValue"] !== reflux_cutoff.val())
					flag_send = true;
				refluxSendData[sensor_key]["allertValue"] = e["allertValue"] = reflux_cutoff.val();
			}
		});
		console.log(refluxSendData);
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
		if(!$.fn.objIsEmpty(globalSensorsJson,false)) {
			let dtoJson = {};
			dtoJson["heaterPower"] = globalSensorsJson["power"];
			dtoJson["temperatures"] = {};
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				$.each(refluxProcess["sensors"], function (j, q) {
					//console.log(j, q);
					if(q["key"] === sensor_key){
						q["value"] = globalSensorsJson["sensors"][i][sensor_key]["value"];
					}
				});
				//if(refluxProcess["sensors"].hasOwnProperty(i) )
					//console.log(refluxProcess["sensors"][i]);
					//console.log(i,e,sensor_key);
					//refluxProcess["sensors"][i][sensor_key]["value"] = globalSensorsJson["sensors"][i][sensor_key]["value"];
				$("#reflux_" + sensor_key).text(globalSensorsJson["sensors"][i][sensor_key]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
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
			});
			$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			$("#power_value").text(globalSensorsJson["power"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
			dtoJson["temperatures"] = refluxProcess["sensors"];
			dtoReceiver.start(dtoJson);
		}
		//console.log(globalSensorsJson);
		//console.log(refluxProcess);
		if(refluxProcess["start"] === true)
			setTimeout(getReflux,2000);
		/*$.ajax({
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

				// if(msg["settingAlarm"] === false){
				// 	$("#reflux_status").removeClass("success").addClass("danger");
				// }else{
				// 	$("#reflux_status").removeClass("danger").addClass("success");
				// }
				if(refluxProcess["start"] === true)
					setTimeout(getReflux,2000);
			}
		});*/

	}
	//заполнение разных полей данными датчиков
	function fillSensorsData() {
		if(!$.fn.objIsEmpty(globalSensorsJson,false)) {
			$.each(globalSensorsJson["sensors"], function (i, e) {
				let sensor_key = Object.keys(e).shift();
				//заполнение вкладки датчики
				if($("#sensor_val_" + sensor_key).length) {
					$("#sensor_val_" + sensor_key).text(globalSensorsJson["sensors"][i][sensor_key]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					$("#svg_sensor_" + sensor_key).html(globalSensorsJson["sensors"][i][sensor_key]["value"] + '&#176С');
				}
				//заполнение процесса ректификации
				if(refluxProcess["start"] !== true && $("#reflux_pressure").length){
					$("#reflux_" + sensor_key).text(globalSensorsJson["sensors"][i][sensor_key]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					if (sensor_key === "p1") {
						$("#reflux_pressure").text(globalSensorsJson["sensors"][i]["p1"]["value"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");

						$("#reflux_alco_boil").text(globalSensorsJson["temperatureAlcoholBoil"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
						$("#power_value").text(globalSensorsJson["power"].toFixed(2)).parent().find(".hidden").removeClass("hidden").addClass("show");
					}
				}
			});
		}
	}

	//запрос постоянно всех датчиков
	function getIntervalSensors() {
		$.ajax({
			url: 'refluxModeSensorsOut',//'refluxModeSensorsIn',//'reflux.json',//refluxModeSensorsOut
			data: {},
			type: 'GET',
			dataType: 'json',
			success: function (msg) {
				//console.log('Sensors',msg);
				globalSensorsJson = msg;
				fillSensorsData();
				/*$.each(msg["sensors"],function (i,e) {
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
				*/
				//if(refluxProcess["start"] === true)
					//setTimeout(getIntervalSensors,2000);
			}
		});
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