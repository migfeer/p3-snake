#include "LedControl.h" 		// incluye libreria LedControl

LedControl lc=LedControl(11,13,10,1);	// crea objeto


/* Button inputs */
const int buttonLEFT = 7;
const int buttonRIGHT = 6;
const int buttonUP = 5;
const int buttonBOTTOM = 4;

/* Buzzer */
const int buzzer = 8;

/* Read buttons time*/
const unsigned long eventInterval= 50;
unsigned long previousTime = 0;

/* Movement time */
const unsigned long eventInterval2= 300;
unsigned long previusTime2 = 0;

//Directions
int up_bt = 0;
int le_ri = 1;

// Active position
bool left = false, right = true, up = false, bt = false;

//Snake struct
typedef struct node
{
  int row;
  int col;
  struct node *prev;
  struct node *next;
} node;

//Apple struct
typedef struct
{
  int row;
  int col;
} apple;


//Snake variables
node *head = NULL; //like list
node *tail = NULL;
int snake_size;
int initial_row;
int initial_col;


//Declare an apple
apple *ap;


//Game over animation
byte animation[8]= {				// array que contiene todos los elementos de las
  B11111111,				// filas necesarias por mostrar el digito cero
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};
byte off[8]= {				// array que contiene todos los elementos de las
  B00000000,				// filas necesarias por mostrar el digito cero
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};


//Prototypes

//Queueue operations
void enqueue(int row, int column);
void dequeue(void);

//Move the snake
void avanzar();

//Set direction
void go_left();
void go_right();
void go_up();
void go_bottom();


// Reset
void (*resetFunc) (void) = 0;

//Eat
void eat_apple();

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false);			// enciende la matriz
  lc.setIntensity(0,4);			// establece brillo
  lc.clearDisplay(0);			// blanquea matriz
  
  pinMode(buttonLEFT, INPUT_PULLUP);
  pinMode(buttonRIGHT, INPUT_PULLUP);
  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(buttonBOTTOM, INPUT_PULLUP);
  noTone(buzzer);  

  //Aleatory sequence
  randomSeed(analogRead(0));
  
  //Create snake of 2
  snake_size = 0;
  // First position
  initial_row = 0;
  initial_col = 1;

  enqueue(initial_row,initial_col);
  lc.setLed(0,head->row,head->col,true);			// enciende LED
  enqueue(initial_row + up_bt,initial_col + le_ri);
  lc.setLed(0,head->row,head->col,true);			// enciende LED
  
  //Create the first apple
  ap = malloc(sizeof(apple));
  
  // Detectar posicion no ocupada
  node *ptr;
  while (ptr != NULL)
  {
    ap->row = random(0,8); //random row 
    ap->col = random(0,8); //random col 

    if (ap->row != ptr->row && ap->col != ptr->col)
    {
      return;
    }
    ptr = ptr->next;
  }
  delay(300);
}

void loop()
{
  unsigned long currentTime;

  //Move the snake between the limits
  while ((head->row >= 0 && head->row <= 7) && (head->col >= 0 && head->col <= 7))
  {
    /* Update millis */
    currentTime = millis(); //Millis return the number of ms arduino has been on

    if (currentTime - previousTime >= eventInterval)
    {
      /* Execute this code */
      
      if (digitalRead(buttonLEFT) == LOW && right == false)
        {
          go_left();
        }
      else if (digitalRead(buttonRIGHT) == LOW && left == false)
      {
        go_right();
      }
      else if (digitalRead(buttonUP) == LOW && bt == false)
      {
        go_up();
      }
      else if (digitalRead(buttonBOTTOM) == LOW && up == false)
      {
        go_bottom();
      }

      /* Update the mark*/
      previousTime = currentTime;
    }

    if (currentTime - previusTime2 >= eventInterval2)
    {
       avanzar();
       eat_apple();

      //Picture snake
      lc.setLed(0,head->row,head->col,true);			// enciende LED
      //Picture apple
      lc.setLed(0,ap->row,ap->col,true);			// enciende LED
      previusTime2 = currentTime;
    }

   
  }
  tone(buzzer, 1046, 350); 

  //Game over
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      lc.setRow(0,j,animation[j]);
    }
    delay(500);
    for (int k = 0; k < 8; k++)
    {
      lc.setRow(0,k,off[k]);
    }
    delay(400);
  }
  lc.clearDisplay(0);			// blanquea matriz
  lc.shutdown(0,true);

  delay(2000);
  resetFunc();
}


void enqueue(int row, int col)
{
  //Serial.println("Enqueue");
  node *n = malloc(sizeof(node));
  if (n == NULL)
  {
    Serial.println("Couldn't allocate memory");
    return;
  }

  //Row and column values
  n->row = row;
  n->col = col;
  n->prev = NULL;

  //Initial node
  if (head == NULL)
  {
    head = n;
    tail = n;  
    n->next = NULL;
  }
  
  else
  {
    //Prepending
    n->next = head;
    head->prev = n;
    head = n;
  }  
  Serial.print("Row: ");
  Serial.print(n->row);
  Serial.print(" Col: ");
  Serial.print(n->col);
  Serial.println();
  snake_size++;

}


void dequeue(void) //Is there a more simple way?
{
  snake_size--;
  //Serial.println("Dequeue");
  lc.setLed(0,tail->row,tail->col,false);     // apagar LED
  node *ptr = NULL;
  tail->prev->next = NULL; //Avoid dangling pointer
  //To update tail to the previous node
  ptr = tail->prev;
  free(tail);
  tail = ptr;
}

void avanzar()
{
  enqueue(head->row + up_bt, head->col + le_ri);
  dequeue();
}

void go_left()
{
  le_ri = -1;
  up_bt = 0; //Re start for not add new rows
  left = true; right = false, up = false, bt = false;
}

void go_right()
{
  le_ri = 1;
  up_bt = 0;   //Re start for not add new rows
  left = false; right = true, up = false, bt = false;
}

void go_up()
{
  up_bt = -1;
  le_ri = 0;   //Re start for not add new cols
  left = false; right = false, up = true, bt = false;
}

void go_bottom()
{
  up_bt = 1;
  le_ri = 0;   //Re start for not add new cols
  left = false; right = false, up = false, bt = true;
}


void eat_apple()
{
  if (ap->row == head->row && ap->col == head->col)
  {
    Serial.println("Collision");
    enqueue(ap->row + up_bt, ap->col + le_ri);
    delay(50);
    
    tone(buzzer, 1046, 300); 
    // Detectar posicion no ocupada
    node *ptr;
    while (ptr != NULL)
    {
      ap->row = random(0,8); //random row 
      ap->col = random(0,8); //random col 

      if (ap->row != ptr->row && ap->col != ptr->col)
      {
        return;
      }
      ptr = ptr->next;
    }
  }
}
