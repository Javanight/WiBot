var map     = Array.prototype.map;
var forEach = Array.prototype.forEach;

var requestPerSecond  = 50;
var isReadyToRequest  = false;
var isCommandDirty    = false;
var inputRange        = document.querySelectorAll('input[type=range]');
var buttons           = document.querySelectorAll('input[type=button]');
var meterBar          = document.querySelector('meter');
var hostname          = location.hostname || prompt('host ip', '192.168.43.108');
var ws                = new WebSocket("ws://" + hostname + ':81');



ws.onopen   = function(evt) {
  console.log("Connection open ...");
  forEach.call(inputRange, function (input) {input.disabled = false});
  forEach.call(buttons, function (input) {input.disabled = false});
  if (meterBar) {
    meterBar.classList.remove("meter-bar--disabled");
  }
};
ws.onclose  = function(evt) {
  console.log("Connection closed.");
  forEach.call(inputRange, function (input) {input.disabled = true});
  forEach.call(buttons, function (input) {input.disabled = true});
  if (meterBar) {
    meterBar.classList.add("meter-bar--disabled");
  }
};
ws.onerror  = function(evt) { console.log("WebSocket error : " + e.data) };



ws.onmessage = function(evt) {
  var data = (JSON.parse(evt.data)).data;
  // console.log(data);

  var i = 0;
  forEach.call(inputRange, function (input) {
    input.value = data[i++];
  });

  if (meterBar) {
    meterBar.value = data[i++];
  }

  isReadyToRequest = true;
};




function sendCommand(command) {
  command = typeof command !== 'undefined' ? command : {'data':[]};
  // console.log(command);

  isReadyToRequest  = false;
  isCommandDirty    = false;

  ws.send(JSON.stringify(command));
}





var command = {'data' : []};
// var command = {'data' : {}};

function prepareCommandResquest(event) {
  // command.data[event.target.name] = event.target.value;
  command = { 'data': map.call(inputRange, function (input) { return input.value; }) };
  isCommandDirty = true;
}
prepareCommandResquest();

forEach.call(inputRange, function (input) { input.addEventListener('input', prepareCommandResquest, false); });





setInterval(function () {
  if (ws.readyState == 1 && isReadyToRequest && isCommandDirty) {
    sendCommand(command);
    // command = {'data' : {}};
  } },
  1000/requestPerSecond
);
