//Версия 1.3 от 04.04.18. ПО распространяется на условиях лицензии GNU GPL v.3. www.LuckyCenter.ru

var T1; // Глобальные переменные для рисования графиков температуры
var T2;
var T3;
var T4;

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
xmlHttp.open('PUT','/configs.json',false);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse;
}

setTimeout('process()',1000);
}
function handleServerResponse(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

T1 = allData.temperature.toFixed(2);
T2 = allData.temperature2.toFixed(2);
T3 = allData.temperature3.toFixed(2);
T4 = allData.temperature4.toFixed(2);
}
}



