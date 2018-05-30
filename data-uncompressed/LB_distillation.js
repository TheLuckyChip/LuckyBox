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
xmlHttp.open('PUT','/distillation.json',true);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse;
}

setTimeout('process()',1000);
}
function handleServerResponse(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

// Блок переменных отвечающих за вывод температуры при дистилляции
document.getElementById('temperatureTank').value = allData.temperature.toFixed(2);       //T1
document.getElementById('temperatureTakeOff').value = allData.temperature2.toFixed(2);   //T2
document.getElementById('temperatureWater').value = allData.temperature3.toFixed(2);     //T3
document.getElementById('settingTank').value = allData.settingTank.toFixed(2);     //T3
//document.getElementById('time').value = allData.time;

var settingAlarm = allData.settingAlarm;
var settingAlarmDistillation = allData.settingAlarmDistillation;

// Блок отвечающий за световую метку достижения уставки
if (settingAlarmDistillation){                
  document.getElementById('statusTank').className = 'danger';
   } else {
     document.getElementById('statusTank').className = 'success';
   }   
   
}
}



// Функция установки уставки при дистилляции и передачи данных в программу
function set_Tank(){              
var SetTank = document.getElementById('setTank').value;
document.getElementById('settingTank').value = SetTank;
var SettingTank = document.getElementById('settingTank').value;


server = "/SetTempTank?SettingTank="+ SettingTank;
request = new XMLHttpRequest();
request.open("GET", server, false);
request.send();
if(request.status != 200) {
alert( request.status + ': ' + request.statusText );
}
}
  