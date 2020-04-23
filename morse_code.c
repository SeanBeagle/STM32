#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/gpio.h>

/* INTERNATIONAL MORSE CODE:
Each position represents one unit. 
ON  : '='
OFF : '.'
"foo bar":
 
=.=.===.=...===.===.===...===.===.===.......===.=.=.=...=.===...=.===.=
    f            o             o                b         a        r

duration | component 
__________________________
1 unit   | dot
3 units  | dash
1 unit   | between elements of a character (dot-dot-dash)
3 units  | between characters
7 units  | between words

1) The duration of a dot  '.' is 1 unit
2) The duration of a dash '_' is 3 units
3) The duration between dots/dashes of a sequence is 1 unit
4) The duration between characters is 3 units
5) The duration between words is 7 units

*/

static void sleep(int units) {
  int time = units * (2e6 * 0.75); 
  while(time-- > 0)
    __asm__("nop"); // assembly instruction: No OPeration (...do nothing)
}

static void dash(void) {
  gpio_toggle(GPIOC, GPIO13); // ON
  sleep(3);
  gpio_toggle(GPIOC, GPIO13); // OFF
}

static void dot(void) {
  gpio_toggle(GPIOC, GPIO13); // ON
  sleep(1);
  gpio_toggle(GPIOC, GPIO13); // OFF
}

/* Transmit a sequence of dot(.) and dash(_) */
static void transmit(char* seq) {
  sleep(2);
  for (int i = 0; seq[i] != '\0'; i++) {
    if (seq[i] == '.') {
      sleep(1);
      dot();
    } else if (seq[i] == '_') {
      sleep(1);
      dash();
    }
  }
}

static void send_message(char* message) {
  for (int i = 0; message[i] != '\0'; i++) {
    switch(message[i]) {
      case 'A':
      case 'a': transmit("._"); break;
      case 'B':
      case 'b': transmit("_..."); break;
      case 'C':
      case 'c': transmit("_._."); break;
      case 'D':
      case 'd': transmit("_._"); break;
      case 'E':
      case 'e': transmit("."); break;
      case 'F':
      case 'f': transmit(".._."); break;
      case 'G':
      case 'g': transmit("__."); break;
      case 'H':
      case 'h': transmit("...."); break;
      case 'I':
      case 'i': transmit(".."); break;
      case 'J':
      case 'j': transmit(".___"); break;
      case 'K':
      case 'k': transmit("_._"); break;
      case 'L':
      case 'l': transmit("._.."); break;
      case 'M':
      case 'm': transmit("__"); break;
      case 'N':
      case 'n': transmit("_."); break;
      case 'O':
      case 'o': transmit("___"); break;
      case 'P':
      case 'p': transmit(".__."); break;
      case 'Q':
      case 'q': transmit("__._"); break;
      case 'R':
      case 'r': transmit("._."); break;
      case 'S':
      case 's': transmit("..."); break;
      case 'T':
      case 't': transmit("_"); break;
      case 'U':
      case 'u': transmit(".._"); break;
      case 'V':
      case 'v': transmit("..._"); break;
      case 'W':
      case 'w': transmit(".__"); break;
      case 'X':
      case 'x': transmit("_.._"); break;
      case 'Y':
      case 'y': transmit("_.__"); break;
      case 'Z':
      case 'z': transmit("__.."); break;
      case ' ': sleep(4); break;  // 4 + 3 IF next char = 7 units
      case '0': transmit("_____"); break;
      case '1': transmit(".____"); break;
      case '2': transmit("..___"); break;
      case '3': transmit("...__"); break;
      case '4': transmit("...._"); break;
      case '5': transmit("....."); break;
      case '6': transmit("_...."); break;
      case '7': transmit("__..."); break;
      case '8': transmit("___.."); break;
      case '9': transmit("____."); break;
      default: break; 
    }
  }
}



/* Blink "Hello World" in morse code from onboard LED */
int main(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();    // Set STM32 to 72MHz
  rcc_periph_clock_enable( RCC_GPIOC );       // Enable GPIOC clock
    
  /* Set GPIO13 Mode: OUTPUT PUSHPULL */
  gpio_set_mode(
    GPIOC,                      // GPIO Port IDs
    GPIO_MODE_OUTPUT_2_MHZ,     // GPIO Pin Mode
    GPIO_CNF_OUTPUT_PUSHPULL,   // GPIO Pin Configuration
    GPIO13                      // GPIO Pin Identifiers
  );

  gpio_toggle(GPIOC, GPIO13);
  while(1) {
    send_message("Hello World");
    sleep(30);
  }
}
