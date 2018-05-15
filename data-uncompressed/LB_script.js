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
xmlHttp.open('PUT','/configs.json',true);
xmlHttp.send(null);
xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
}
setTimeout('process()',1000);
}

function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
}

function val(id){
 var v = document.getElementById(id).value;
 return v;
}


function send_request(submit,server){ 
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}


function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Подождите...';
 submit_disabled(true);
}
function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}

function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}

// Блок отвечающий за световую метку достижения уставки

if (settingAlarm){                
 //$("#statusColumn").show();
  document.getElementById('statusColumn').className = 'danger'; 
  
} else {
    // $("#statusColumn").hide();
    document.getElementById('statusColumn').className = 'success';
   }

if (settingAlarmDistillation){                
  //$("#statusTank").show();
  document.getElementById('statusTank').className = 'danger';
   } else {
     //$("#statusTank").hide();
     document.getElementById('statusTank').className = 'success';
   }   
   

// Функция установки уставки при ректификации и передачи данных в программу
function setting_Column(){          
var Delta = document.getElementById('delta').value;
var Setting = document.getElementById('setting').value;

if(Delta !== 0) {
document.getElementById('setting').value=(+document.getElementById('temperature2').value + +Delta).toFixed(2);
Setting = document.getElementById('setting').value;
}

server = "/SetTemp?delta="+Delta + "&setting="+ Setting;
request = new XMLHttpRequest();
request.open("GET", server, false);
request.send();
if(request.status != 200) {
alert( request.status + ': ' + request.statusText );
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
  