$(document).ready(function() {
    const deviceUrl = 'http://192.168.1.106/';

    // Считывание предыдущих сохранённых значений
    let deviceConditions = localStorage.getObj('deviceConditions');

    // Проверка на существование сохранённых значений
    if (deviceConditions == null) {
        deviceConditions = [];
    }


    Highcharts.setOptions({
        global: {
            useUTC: false
        }
    });

    // Create the chart
    var plot = Highcharts.stockChart('viewPort', {
        chart: {
           
        },
        xAxis: {
            type: 'datetime'
        },
        yAxis: [{ // Primary yAxis
            labels: {
                format: '{value}°C',
                style: {
                    color:'black'
                }
            },
            title: {
                text: 'Temperature',
                style: {
                    color: 'black'
                }
            }

        }, { // Secondary yAxis
                gridLineWidth: 0,
                title: {
                    text: 'Power',
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
        ],

        series: [
            { name: "Power", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 0, showInLegend: true},
            { name: "T1", data: deviceConditions.map(function (dc) { return [dc.dateTime, dc.temperature] })},
            { name: "T2", data: deviceConditions.map(function (dc) { return [dc.dateTime, dc.temperature2] })},
            { name: "T3", data: deviceConditions.map(function (dc) { return [dc.dateTime, dc.temperature3] })},
            { name: "T4", data: deviceConditions.map(function (dc) { return [dc.dateTime, dc.temperature4] })}
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
        legend: {
            enabled: true
        },
        exporting: {
            enabled: true
        },
        //tooltip: {
        //    formatter: function () {
        //        return 'The value for <b>' + this.x +
        //            '</b> is <b>' + this.y + '</b>';
        //    }
        //}

        //tooltip: {
        //    pointFormat: '{series.name}: <b>{point.y.toFixed(2)}</b><br/>',
        //    //valueSuffix: ' cm',
        //    shared: true
        //}
    });

    function getRandomInt(min, max) {
        return Math.floor(Math.random() * (max - min)) + min;
    }

    var power = 0;
    var i = 0;

    $(document).on("receivedNewCondition", function (e, deviceCondition) {
        // console.log(arg1);

        if ((i++) % 10 === 0) power = getRandomInt(0, 100);

        let time = (new Date()).getTime();
        plot.series[0].addPoint([time, power], false);

        plot.series[1].addPoint([time, deviceCondition.temperature], false);
        plot.series[2].addPoint([time, deviceCondition.temperature2], false);
        plot.series[3].addPoint([time, deviceCondition.temperature3], false);
        plot.series[4].addPoint([time, deviceCondition.temperature4], false);


        plot.redraw();
    });

    $('#plotClear').click(function() {
        localStorage.setObj('deviceConditions', []);
        localStorage.clear();

        plot.series.forEach(function(s) {
            s.data = [];
        });
        plot.redraw();
    });
});


