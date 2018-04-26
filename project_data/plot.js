$(document).ready(function() {
    const deviceUrl = 'http://192.168.1.106/';

    const deviceConditions = [];
    const data = [];

    Highcharts.setOptions({
        global: {
            useUTC: false
        }
    });

    // Create the chart
    var plot = Highcharts.stockChart('viewPort', {
        chart: {
        },
        series: [
            { name: "T1" },
            { name: "T2" },
            { name: "T3" },
            { name: "T4" }
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
            text: 'Данные температур'
        },

        exporting: {
            enabled: true
        }


    });

    function getDeviceCondition() {
        $.ajax({
            url: deviceUrl + 'configs.json',
            data: {},
            async: false,
            type: 'GET',
            dataType: 'text',
            success: function(msg) {
                console.log('Settings', msg);

                let deviceCondition = JSON.parse(msg);
                deviceConditions.push(JSON.parse(msg));

                let time = (new Date()).getTime();
                plot.series[0].addPoint([time, deviceCondition.temperature], false);
                plot.series[1].addPoint([time, deviceCondition.temperature2], false);
                plot.series[2].addPoint([time, deviceCondition.temperature3], false);
                plot.series[4].addPoint([time, deviceCondition.temperature4], false);


                plot.redraw();
            }
        }).done();
    }


    



    setInterval(getDeviceCondition, 1000);

});


