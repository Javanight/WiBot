const DELAY_DEFAULT = 100;


function execCommandList(commandList, delay) {
  delay = delay || DELAY_DEFAULT;
  if (commandList.length > 0) {
    sendCommand({ data: commandList.shift() });

    if (commandList.length > 0) {
      setTimeout(function () {
        execCommandList(commandList);
      }, delay);
    }
  }
}



const LEGS_LEFT_MIN      = 65;
const LEGS_LEFT_CENTER   = 90;
const LEGS_LEFT_MAX      = 115;

const LEGS_MIDDLE_MIN    = 75;
const LEGS_MIDDLE_CENTER = 90;
const LEGS_MIDDLE_MAX    = 105;

const LEGS_RIGHT_MIN     = 65;
const LEGS_RIGHT_CENTER  = 90;
const LEGS_RIGHT_MAX     = 115;


function stepForward() {
  commandList = [
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_MIN,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MIN ],     // demi avance gauche
    [ 32,   LEGS_LEFT_MIN,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MIN ],
    [ 128,  LEGS_LEFT_MAX,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MAX ],     // avance droite
    [ 32,   LEGS_LEFT_MAX,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],  // demi avance gauche
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList);
}


function stepBackward() {
  commandList = [
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_MIN,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MIN ],     // demi avance gauche
    [ 32,   LEGS_LEFT_MIN,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MIN ],
    [ 128,  LEGS_LEFT_MAX,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MAX ],     // avance droite
    [ 32,   LEGS_LEFT_MAX,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],  // demi avance gauche
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList);
}


function turnLeft() {
  commandList = [
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_MIN,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MAX ],     // demi step tour
    [ 32,   LEGS_LEFT_MIN,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_MAX,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MIN ],     // step tour
    [ 32,   LEGS_LEFT_MAX,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MIN ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],  // demi step tour
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList);
}


function turnRight() {
  commandList = [
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_MIN,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MAX ],     // demi step tour
    [ 32,   LEGS_LEFT_MIN,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_MAX,     LEGS_MIDDLE_MIN,    LEGS_RIGHT_MIN ],     // step tour
    [ 32,   LEGS_LEFT_MAX,     LEGS_MIDDLE_MAX,    LEGS_RIGHT_MIN ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],  // demi step tour
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList);
}



function command01() {
  commandList = [
    [ 32,   LEGS_LEFT_MIN,  LEGS_MIDDLE_CENTER,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_MAX,  LEGS_MIDDLE_CENTER,    LEGS_RIGHT_MIN ],

    [ 32,   LEGS_LEFT_MIN,  LEGS_MIDDLE_CENTER,    LEGS_RIGHT_MAX ],
    [ 128,  LEGS_LEFT_MAX,  LEGS_MIDDLE_CENTER,    LEGS_RIGHT_MIN ],

    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList, 300);
}


function command02() {
  commandList = [
    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],

    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_MIN,    LEGS_RIGHT_CENTER ],
    [ 128,  LEGS_LEFT_CENTER,  LEGS_MIDDLE_MAX,    LEGS_RIGHT_CENTER ],

    [ 32,   LEGS_LEFT_CENTER,  LEGS_MIDDLE_CENTER, LEGS_RIGHT_CENTER ]
  ];
  execCommandList(commandList, 300);
}





var buttonStepForwardEl = document.getElementById('button-step-forward');
buttonStepForwardEl.addEventListener('click', stepForward, false);

var buttonStepBackwardEl = document.getElementById('button-step-backward');
buttonStepBackwardEl.addEventListener('click', stepBackward, false);

var buttonTurnLeftEl = document.getElementById('button-turn-left');
buttonTurnLeftEl.addEventListener('click', turnLeft, false);

var buttonTurnRightEl = document.getElementById('button-turn-right');
buttonTurnRightEl.addEventListener('click', turnRight, false);



var buttonCommand01El = document.getElementById('button-command-01');
buttonCommand01El.addEventListener('click', command01, false);

var buttonCommand02El = document.getElementById('button-command-02');
buttonCommand02El.addEventListener('click', command02, false);
