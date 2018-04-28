var luckyBoxReceiver = {
    
    deviceConditions: [],   // Контейнер состояний в ОЗУ
    frequencyRecordingToLocalStorage: 5,    // Частота архивации
    frequencyReqest: 1000,                  // Частота запросов на сервер TODO вынести в UI

    getDeviceCondition: function () {  
        const self = luckyBoxReceiver;
        const deviceUrl = 'http://192.168.1.106/'; // Для отладки графиков !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        let recordingTicker = 0;
        let power = 10;

        $.ajax({
            url: deviceUrl + 'configs.json',
            data: {},
            async: false,
            type: 'GET',
            dataType: 'text',
            success: function (msg) {
                console.log('Settings', msg);
                self.deviceConditionCurrent = JSON.parse(msg);
                self.deviceConditionCurrent.dateTime = (new Date()).getTime();
                self.deviceConditionCurrent.power = power;                   // TODO Эмуляция

                self.deviceConditions.push(self.deviceConditionCurrent);

                // Запись в хранилище
                if ((recordingTicker++ % self.frequencyRecordingToLocalStorage) === 0) {
                    localStorage.setObj('deviceConditions', self.deviceConditions);
                    self.recordingTicker = 0;

                    power = getRandomInt(0, 100);
                }

                // Вызов события что данные получены
                $(document).trigger('receivedNewCondition', [self.deviceConditionCurrent]);
            }
        }).done();


        function getRandomInt(min, max) {
            return Math.floor(Math.random() * (max - min)) + min;
        }

    },

    clearDeviceConditions() {
        this.deviceConditions = [];
        localStorage.removeItem('deviceConditions');
    },

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
        this.deviceConditions = localStorage.getObj('deviceConditions');

        // Проверка на существование сохранённых значений
        if (this.deviceConditions == null) {
            this.deviceConditions = [];
        }
        setInterval(this.getDeviceCondition, this.frequencyReqest);
    }
}

luckyBoxReceiver.start();