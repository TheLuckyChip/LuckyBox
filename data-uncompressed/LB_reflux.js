//Версия 1.3 от 04.04.18. ПО распространяется на условиях лицензии GNU GPL v.3. www.LuckyCenter.ru

var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function process(){
  
if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
xmlHttp.open('PUT','/reflux.json',true);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse;
}

setTimeout('process()',1000);
}
function handleServerResponse(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

// Блок переменных отвечающих за вывод температуры при ректификации
document.getElementById('temperature').value=allData.temperature.toFixed(2);    //T1
document.getElementById('temperature2').value=allData.temperature2.toFixed(2);  //T2
document.getElementById('temperature3').value=allData.temperature3.toFixed(2);  //T3
document.getElementById('temperature4').value=allData.temperature4.toFixed(2);  //T4

document.getElementById('setting').value=allData.setting.toFixed(2);
document.getElementById('temperatureAlcoholBoil').value=allData.temperatureAlcoholBoil.toFixed(2);
document.getElementById('pressure').value=allData.pressure.toFixed(2);

var settingAlarm = allData.settingAlarm;

// Блок отвечающий за световую метку достижения уставки
if (settingAlarm){                
  document.getElementById('statusColumn').className = 'danger'; 
   } else {
    document.getElementById('statusColumn').className = 'success';
   }
   
}
}


// Функция установки уставки при ректификации и передачи данных в программу
     function setting_Column(){          
      var Delta = document.getElementById('delta').value;
      var Setting = document.getElementById('setting').value;

      if(Delta !== 0) {
       document.getElementById('setting').value=(+document.getElementById('temperature2').value + +Delta).toFixed(2);
       Setting = document.getElementById('setting').value;
      }

      server = "/SetTemp?delta="+Delta + "&setting="+ Setting +"&temperatureAlcoholBoil=" + document.getElementById('temperatureAlcoholBoil').value;
      request = new XMLHttpRequest();
      request.open("GET", server, false);
      request.send();
      if(request.status != 200) {
      alert( request.status + ': ' + request.statusText );
      }
     }





  
   




