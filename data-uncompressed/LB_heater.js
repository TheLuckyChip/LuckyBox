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
xmlHttp.open('PUT','/heater.json',true);
xmlHttp.send(null);

xmlHttp.onreadystatechange=handleServerResponse;
}

//setTimeout('process()',1000);
}

function handleServerResponse(){
if(xmlHttp.readyState==4 && xmlHttp.status==200){
var allData=JSON.parse(xmlHttp.responseText);

// Блок переменных отвечающих за вывод модности ТЭНа
document.getElementById('heaterPower').value = allData.heaterPower.toFixed(0);  
}
}


// Функция передачи значений мощности ТЭНа
function setting_Heater(){          

server = "/SetHeaterPower?heaterPower="+ document.getElementById('heaterPower').value;
request = new XMLHttpRequest();
request.open("GET", server, false);
request.send();
if(request.status != 200) {
alert( request.status + ': ' + request.statusText );
}
}
  