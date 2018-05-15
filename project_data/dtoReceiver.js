/**
 * Глобальный объект dtoReceiver служит для опроса МК.
 */
var dtoReceiver = {
    
    dtos: [],                               // Контейнер состояний в ОЗУ
    frequencyRecordingToLocalStorage: 5,    // Частота архивации (Через сколько опросов осуществляется запись в localStorage)
    reqestDelayDefalt: 1000,                // Период между запросами (в мс) на сервер TODO вынести в UI
    intervalId: 0,

    dtoGet: function () {
        const self = dtoReceiver;  // Для доступа к this в jquery

        let deviceUrl = 'handleConfigJSON';
		//TODO Для отладки
        if (window.location.hostname === 'localhost' || window.location.hostname === '') {
            //deviceUrl = 'http://192.168.1.106/' + 'GetDto';
			deviceUrl = 'dto.json';
        }
         
        let requestCounter = 0;    // Счётчик запросов, служит для записи в localStorage каждые frequencyRecordingToLocalStorage раз

        $.ajax({
            url: deviceUrl,
            data: {},
            //async: false,
            type: 'GET',
			//TODO Для отладки
            dataType: 'json',   // json не пропускает с чужого хоста
            success: function (json) {
				//console.log("dtoGet",json);
				self.dtoCurrent = json;
                self.dtoCurrent.dateTime = (new Date()).getTime();  // Пользуемся временем клиента, для несчастных без доступа к NTP

                self.dtos.push(self.dtoCurrent);

                // Запись в хранилище
                if ((requestCounter++ % self.frequencyRecordingToLocalStorage) === 0) {
                    localStorage.setObj('dtos', self.dtos);
                }

				// Вызов события что данные получены
                $(document).trigger('newDTOreceived', [self.dtoCurrent]);
            }
        });//.done();
    },

    // Очистка LocalStorage
    clearDeviceConditions: function() {
        this.dtos = [];
        localStorage.removeItem('dtos');
    },

    // Изменить скорость обновления
    changeSpeed: function(intervar) {
        clearInterval(this.intervalId);
		this.reqestDelayDefalt = intervar;
		this.start()
         //this.intervalId = setInterval(this.dtoGet, intervar);
    },

    // Запуск опроса ESP
    start: function () {
        // Функция записи в LocalStorage
        Storage.prototype.setObj = function(key, obj) {

            try {
                return this.setItem(key, JSON.stringify(obj));
            } catch (e) {
                //TODO кажется нужно указывать e.name и то не для всех браузеров, нужно покурить мануал
                //TODO есть пример функции (isQuotaExceeded) отлова ошибки для разных браузеров http://crocodillon.com/blog/always-catch-localstorage-security-and-quota-exceeded-errors
                if (e === 'QUOTA_EXCEEDED_ERR') {
                    alert('Превышен лимит localStorage');
                }
                return null;
            }
        };

        // Функция чтения из LocalStorage
        Storage.prototype.getObj = function(key) {
            return JSON.parse(this.getItem(key));
        };

        // Считывание предыдущих сохранённых значений
        this.dtos = localStorage.getObj('dtos');

        // Проверка на существование сохранённых значений
        if (this.dtos == null) {
            this.dtos = [];
        }

        this.intervalId = setInterval(this.dtoGet, this.reqestDelayDefalt);
    }
};
//TODO запускаем кнопкой
//dtoReceiver.start();