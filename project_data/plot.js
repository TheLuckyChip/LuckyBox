$(document).ready(function() {

    Highcharts.setOptions({
        global: {
            useUTC: false
        }
    });

    var plot;
    //TODO сделать передачу в «getPlot» ID контейнера, для вывода графиков в требуемом процессе, и скорее всего еще сами датчики (для каждого процесса разные) или он сам подтянет нужные?
    $(document).one("newDTOreceived", function () {
        plot = getPlot();
    });

    $('#plotClear').click(function() {
        window.dtoReceiver.clearDeviceConditions();
        plot.series.forEach(function (s) { s.setData([]) });

        plot.redraw();
    });

    $("#reqestDelay").val(window.dtoReceiver.reqestDelayDefalt / 1000);
    $('#changeSpeed').click(function () {

        let reqestDelay = $("#reqestDelay").val();

        if (reqestDelay>=1) {
            window.dtoReceiver.changeSpeed($("#reqestDelay").val() * 1000);
        } else {
            $("#reqestDelay").val(window.dtoReceiver.reqestDelayDefalt / 1000);
            alert('Минимальный интервал 1 сек');
        }
    });

    function getPlot() {
		//console.log("Запуск графиков!");

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
                {
                    name: "Power", yAxis: 1, type: "area", step: 'left', fillOpacity: 0.05, color: "#f00000", lineWidth: 0, showInLegend: true,
                    data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.heaterPower] }) 
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
        
        dtoReceiver.dtos[0].temperatures.forEach(function(t, i) {
            plotNew.addSeries({
                name: "T" + i,
                data: dtoReceiver.dtos.map(function (dc) { return [dc.dateTime, dc.temperatures[i]] })
            });
        });
        

        $(document).on("newDTOreceived", function (e, dto) {

            plot.series[0].addPoint([dto.dateTime, dto.heaterPower], false);

            dto.temperatures.forEach(function(t,i) {
                //console.log(t,i);
                plotNew.series[i + 1].addPoint([dto.dateTime, dto.temperatures[i]], false);
            });
            plotNew.redraw();
        });

        return plotNew;
    }
});