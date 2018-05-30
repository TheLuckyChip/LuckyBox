//Версия 1.3 от 04.04.18. ПО распространяется на условиях лицензии GNU GPL v.3. www.LuckyCenter.ru

var startBrewing = 0;
var stepBrewing = 0;

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
xmlHttp.open('PUT','/brewing.json',true);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse;
process2();
}

//setTimeout('process()',1000);
}

function handleServerResponse(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

// Блок переменных отвечающих за вывод температурных пауз при затирании

document.getElementById('pauseTemp1').value = allData.pauseTemp1.toFixed(0);  
document.getElementById('pauseTemp2').value = allData.pauseTemp2.toFixed(0);  
document.getElementById('pauseTemp3').value = allData.pauseTemp3.toFixed(0);  
document.getElementById('pauseTemp4').value = allData.pauseTemp4.toFixed(0);  

document.getElementById('pauseTime1').value = allData.pauseTime1.toFixed(0);  
document.getElementById('pauseTime2').value = allData.pauseTime2.toFixed(0);  
document.getElementById('pauseTime3').value = allData.pauseTime3.toFixed(0);  
document.getElementById('pauseTime4').value = allData.pauseTime4.toFixed(0);  

//document.getElementById('startBrewing').value = allData.startBrewing;

}
}

function process2(){
if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
xmlHttp.open('PUT','/brewing.json',true);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse2;

}
setTimeout('process2()',1000);
}

function handleServerResponse2(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

document.getElementById('time').value = allData.time;
var stepBrewing = allData.stepBrewing.toFixed(0);

// Блок отвечающий за световую метку пауз затирания

switch(stepBrewing) {
  case '0':  // 
  //  document.getElementById('statusPause1').className = 'default'; 
  //  document.getElementById('statusPause2').className = 'default'; 
  //  document.getElementById('statusPause3').className = 'default'; 
  //  document.getElementById('statusPause4').className = 'default'; 
    break;

  case '1':
  case '2':
    document.getElementById('statusPause2').className = 'default'; 
    document.getElementById('statusPause3').className = 'default'; 
    document.getElementById('statusPause4').className = 'default';
    document.getElementById('statusPause1').className = 'success';
    break;
 
  case '3':
  case '4':
    document.getElementById('statusPause2').className = 'success';
    break;

  case '5':
  case '6':
    document.getElementById('statusPause3').className = 'success';
    break;

  case '7':
  case '8':
    document.getElementById('statusPause4').className = 'success';
    break;
    
}


}
}

// Функция передачи значений температуры и времени пауз
function setting_Brew(){          

server = "/SettingBrewing?startBrewing=" + startBrewing + "&stepBrewing=" + stepBrewing + "&pauseTemp1="+ document.getElementById('pauseTemp1').value + "&pauseTemp2=" + document.getElementById('pauseTemp2').value +
"&pauseTemp3=" + document.getElementById('pauseTemp3').value + "&pauseTemp4=" + document.getElementById('pauseTemp4').value +
"&pauseTime1=" + document.getElementById('pauseTime1').value + "&pauseTime2=" + document.getElementById('pauseTime2').value +
"&pauseTime3=" + document.getElementById('pauseTime3').value + "&pauseTime4=" + document.getElementById('pauseTime4').value;

request = new XMLHttpRequest();
request.open("GET", server, false);
request.send();
if(request.status != 200) {
alert( request.status + ': ' + request.statusText );
}
}
  
  
function start_Brew(){          
startBrewing = 1;
stepBrewing = 1;
setting_Brew();
}  

function stop_Brew(){          
startBrewing = 0;
stepBrewing = 0;
setting_Brew();

server = "/SetHeaterPower?heaterPower="+ 0;
request = new XMLHttpRequest();
request.open("GET", server, false);
request.send();
if(request.status != 200) {
alert( request.status + ': ' + request.statusText );
}

} 