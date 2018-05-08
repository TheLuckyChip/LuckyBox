/**
 * Глобальный объект dtoReceiver служит для опроса МК.
 */
var dtoReceiver = {
    
    dtos: [],                               // Контейнер состояний в ОЗУ
    frequencyRecordingToLocalStorage: 5,    // Частота архивации (Через сколько опросов осуществляется запись в localStorage)
    reqestDelay: 1000,                      // Период между запросами (в мс) на сервер TODO вынести в UI

    dtoGet: function () {  
        const self = dtoReceiver;  // Для доступа к this в jquery

        let deviceUrl = '';
        if (window.location.hostname == 'localhost' || window.location.hostname == '') { // Для отладки 
            deviceUrl = 'http://192.168.1.106/';
        }
         
        let requestСounter = 0;    // Счётчик запросов, служит для записи в localStorage каждые frequencyRecordingToLocalStorage раз

        $.ajax({
            url: deviceUrl + 'GetDto',
            data: {},
            async: false,
            type: 'GET',
            dataType: 'text',   // json не пропускает с чужого хоста
            success: function (json) {

                self.dtoCurrent = JSON.parse(json);
                self.dtoCurrent.dateTime = (new Date()).getTime();  // Пользуемся временем клиента, для несчастных без доступа к NTP

                self.dtos.push(self.dtoCurrent);

                // Запись в хранилище
                if ((requestСounter++ % self.frequencyRecordingToLocalStorage) === 0) {
                    localStorage.setObj('dtos', self.dtos);
                }

                // Вызов события что данные получены
                $(document).trigger('newDTOreceived', [self.dtoCurrent]);
            }
        }).done();
    },

    // Очистка LocalStorage
    clearDeviceConditions: function() {
        this.dtos = [];
        localStorage.removeItem('dtos');
    },

    // Запуск опроса ESP
    start: function () {
        // Функция записи в LocalStorage
        Storage.prototype.setObj = function(key, obj) {

            try {
                return this.setItem(key, JSON.stringify(obj));
            } catch (e) {
                if (e == QUOTA_EXCEEDED_ERR) {
                    alert('Превышен лимит localStorage');
                }
                return null;
            }
        }

        // Функция чтения из LocalStorage
        Storage.prototype.getObj = function (key) {
            return JSON.parse(this.getItem(key));
        }

        // Считывание предыдущих сохранённых значений
        this.dtos = localStorage.getObj('dtos');

        // Проверка на существование сохранённых значений
        if (this.dtos == null) {
            this.dtos = [];
        }
        setInterval(this.dtoGet, this.reqestDelay);
    }
}

dtoReceiver.start();