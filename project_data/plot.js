$(document).ready(function() {

    Highcharts.setOptions({
        global: {
            useUTC: false
        }
    });

    var plot;
    $(document).one("newDTOreceived", function () {
        plot = getPlot();
    });

    $('#plotClear').click(function() {
        window.dtoReceiver.clearDeviceConditions();
        plot.series.forEach(function (s) { s.setData([]) });

        plot.redraw();
    });

    function getPlot() {

        var plotNew = Highcharts.stockChart('viewPort', {
            chart: {

            },
            xAxis: {
                type: 'datetime'
            },
            yAxis: [{ // Primary yAxis
                    labels: {
                        format: '{value}°C',
                        style: {
                            color: 'black'
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
                { name: "Power", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 0, showInLegend: true },
                { name: "T1", data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.temperature] }) },
                { name: "T2", data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.temperature2] }) },
                { name: "T3", data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.temperature3] }) },
                { name: "T4", data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.temperature4] }) }
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

        $(document).on("newDTOreceived", function (e, deviceCondition) {

            //plot.series[0].addPoint([deviceCondition.dateTime, deviceCondition.power], false);

            plotNew.series[1].addPoint([deviceCondition.dateTime, deviceCondition.temperature], false);
            plotNew.series[2].addPoint([deviceCondition.dateTime, deviceCondition.temperature2], false);
            plotNew.series[3].addPoint([deviceCondition.dateTime, deviceCondition.temperature3], false);
            plotNew.series[4].addPoint([deviceCondition.dateTime, deviceCondition.temperature4], false);

            plotNew.redraw();
        });

        return plotNew;
    }
});


