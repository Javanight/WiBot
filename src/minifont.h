#include <Adafruit_SSD1306.h>
#include <Ethernet.h>

#define MINIFONT_WIDTH  3
#define MINIFONT_HEIGHT 5

#define __ false
#define XX true

#define MINIFONT_0 0
#define MINIFONT_1 1
#define MINIFONT_2 2
#define MINIFONT_3 3
#define MINIFONT_4 4
#define MINIFONT_5 5
#define MINIFONT_6 6
#define MINIFONT_7 7
#define MINIFONT_8 8
#define MINIFONT_9 9

#define MINIFONT_DOT    10
#define MINIFONT_COLON  11
#define MINIFONT_SLASH  12
#define MINIFONT_SPACE  13

#define MINIFONT_UP          14
#define MINIFONT_ANALOG_UP   15

#define MINIFONT_A 16
#define MINIFONT_D 17
#define MINIFONT_I 18
#define MINIFONT_N 19
#define MINIFONT_O 20
#define MINIFONT_P 21


const bool minifont[][15] = {
  {
    __,XX,__,
    XX,__,XX,
    XX,__,XX,
    XX,__,XX,
    __,XX,__
  },

  {
    __,XX,__,
    XX,XX,__,
    __,XX,__,
    __,XX,__,
    __,XX,__
  },

  {
    XX,XX,__,
    __,__,XX,
    XX,XX,XX,
    XX,__,__,
    XX,XX,XX
  },

  {
    XX,XX,__,
    __,__,XX,
    XX,XX,__,
    __,__,XX,
    XX,XX,__
  },

  {
    __,XX,__,
    __,XX,__,
    XX,__,__,
    XX,XX,XX,
    __,XX,__
  },

  {
    XX,XX,XX,
    XX,__,__,
    XX,XX,XX,
    __,__,XX,
    XX,XX,__
  },

  {
    __,XX,XX,
    XX,__,__,
    XX,XX,__,
    XX,__,XX,
    __,XX,__
  },

  {
    XX,XX,XX,
    __,__,XX,
    __,__,XX,
    __,__,XX,
    __,__,XX
  },

  {
    __,XX,__,
    XX,__,XX,
    __,XX,__,
    XX,__,XX,
    __,XX,__
  },

  {
    __,XX,__,
    XX,__,XX,
    __,XX,XX,
    __,__,XX,
    XX,XX,__
  },



  {
    __,__,__,
    __,__,__,
    __,__,__,
    __,__,__,
    XX,__,__
  },

  {
    __,__,__,
    __,__,__,
    XX,__,__,
    __,__,__,
    XX,__,__
  },

  {
    __,XX,__,
    __,XX,__,
    __,XX,__,
    XX,__,__,
    XX,__,__
  },

  {
    __,__,__,
    __,__,__,
    __,__,__,
    __,__,__,
    __,__,__
  },




  {
    __,XX,__,
    XX,XX,XX,
    XX,XX,XX,
    XX,XX,XX,
    XX,__,XX
  },

  {
    __,XX,__,
    XX,__,XX,
    __,__,__,
    __,XX,__,
    XX,__,XX
  },





  {
    __,XX,__,
    XX,__,XX,
    XX,XX,XX,
    XX,__,XX,
    XX,__,XX
  },

  {
    XX,XX,__,
    XX,__,XX,
    XX,__,XX,
    XX,__,XX,
    XX,XX,__
  },

  {
    __,XX,__,
    __,__,__,
    __,XX,__,
    __,XX,__,
    __,XX,__
  },

  {
    XX,__,XX,
    XX,XX,XX,
    XX,XX,XX,
    XX,XX,XX,
    XX,__,XX
  },

  {
    __,__,__,
    __,XX,__,
    XX,__,XX,
    XX,__,XX,
    __,XX,__
  },

  {
    XX,XX,__,
    XX,__,XX,
    XX,XX,__,
    XX,__,__,
    XX,__,__
  },
};



extern Adafruit_SSD1306 display;
int cursorX = 0;
int cursorY = 0;


void setMinifontCursor(int x, int y) {
  cursorX = x;
  cursorY = y;
}



void printMinifontChar(u_int minifontId) {
  for (int j = 0; j < MINIFONT_HEIGHT; j++) {
    for (int i = 0; i < MINIFONT_WIDTH; i++) {
      display.drawPixel(cursorX + i, cursorY + j, minifont[minifontId][i + j*MINIFONT_WIDTH] ? WHITE : BLACK);
    }
  }
  cursorX += 4;
}


void printMinifont(String string) {
  for (int i = 0, l = string.length(); i < l; i++) {
    Serial.print(string[i]);
    switch (string.charAt(i)) {
      case '0':
        printMinifontChar(MINIFONT_0);
        break;

      case '1':
        printMinifontChar(MINIFONT_1);
        break;

      case '2':
        printMinifontChar(MINIFONT_2);
        break;

      case '3':
        printMinifontChar(MINIFONT_3);
        break;

      case '4':
        printMinifontChar(MINIFONT_4);
        break;

      case '5':
        printMinifontChar(MINIFONT_5);
        break;

      case '6':
        printMinifontChar(MINIFONT_6);
        break;

      case '7':
        printMinifontChar(MINIFONT_7);
        break;

      case '8':
        printMinifontChar(MINIFONT_8);
        break;

      case '9':
        printMinifontChar(MINIFONT_9);
        break;



      case '.':
        printMinifontChar(MINIFONT_DOT);
        break;



      case 'A':
        printMinifontChar(MINIFONT_A);
        break;
    }
  }
}



void printMinifontIp(IPAddress ip) {
  printMinifont(String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
}




void printMinifontIo() {
  int cursorXStart = cursorX + 6;
  int cursorYStart = cursorY;

  // SECTION TITLE
  display.drawLine(0, cursorYStart, 63, cursorYStart, WHITE);

  // io ID
  setMinifontCursor(cursorXStart, cursorYStart + 8);

  printMinifontChar(MINIFONT_0);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_1);
  printMinifontChar(MINIFONT_2);
  printMinifontChar(MINIFONT_3);
  printMinifontChar(MINIFONT_4);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_5);
  printMinifontChar(MINIFONT_6);
  printMinifontChar(MINIFONT_7);
  printMinifontChar(MINIFONT_8);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_A);

  // OUT
  setMinifontCursor(cursorXStart, cursorYStart + 2);

  printMinifontChar(MINIFONT_UP);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_UP);
  printMinifontChar(MINIFONT_UP);
  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_ANALOG_UP);

  // IN
  setMinifontCursor(cursorXStart, cursorYStart + 14);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_UP);
  printMinifontChar(MINIFONT_SPACE);
  printMinifontChar(MINIFONT_UP);

  printMinifontChar(MINIFONT_SPACE);

  printMinifontChar(MINIFONT_ANALOG_UP);

}
