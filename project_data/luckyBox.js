class LuckyBox {

    constructor() {
        this.subscribers = [];
        setInterval(getDeviceCondition, 2000);
    }

    getDeviceCondition() {
        const self = this;
        const deviceUrl = 'http://192.168.1.106/'; // Для отладки графиков

        $.ajax({
            url: deviceUrl + 'configs.json',
            data: {},
            async: false,
            type: 'GET',
            dataType: 'text',
            success: function (msg) {
                console.log('Settings', msg);
                self.deviceConditionCurrent = JSON.parse(msg);
                localStorage.sensorsConditions.push(self.deviceConditionCurrent);

                subscribers.forEach(s => s(self.deviceConditionCurrent));
            }
        }).done();
    }

    subscribeReceivingCondition(action) {
        subscribers.push(action);
    }
}