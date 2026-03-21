#include "LedControl.h" 		// incluye libreria LedControl

LedControl lc=LedControl(11,13,10,1);	// crea objeto

//Variables
int snake_size = 2; //Size of the snake

//Constants
const int buttonLEFT = 4;
const int buttonRIGHT = 3;

//Directions
int up_bt = 0;
int le_ri = 1;

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


//Snake head and tail
node *head = NULL; //like list
node *tail = NULL;

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

//Create the body of snake
//void create_initial_snake();

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


//Eat
void eat_apple();

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false);			// enciende la matriz
  lc.setIntensity(0,4);			// establece brillo
  lc.clearDisplay(0);			// blanquea matriz
  pinMode(buttonLEFT, INPUT);
  pinMode(buttonRIGHT, INPUT);

  //Aleatory sequence
  randomSeed(analogRead(0));

  //Create the first apple
  ap = malloc(sizeof(apple));
  ap->row = random(0,8); //random row 
  ap->col = random(0,8); //random col 
  
  //Create snake of 2
  int initial_row = 0;
  int initial_col = 3;
  enqueue(initial_row,initial_col);
  lc.setLed(0,head->row,head->col,true);			// enciende LED

  enqueue(initial_row + up_bt,initial_col + le_ri);
  lc.setLed(0,head->row,head->col,true);			// enciende LED
  
  delay(500);
}

void loop()
{
  //Move the snake between the limits
  while((head->row >= 0 && head->row <= 7) && (head->col >= 0 && head->col <= 7))
  {
    avanzar();
    eat_apple();

    if (digitalRead(buttonLEFT) == HIGH)
      {
        go_left();
      }
    else if (digitalRead(buttonRIGHT) == HIGH)
    {
      go_right();
    }
    else if (digitalRead(buttonLEFT) == HIGH)
    {
      go_up();
    }
    else if (digitalRead(buttonRIGHT) == HIGH)
    {
      go_bottom();
    }

    //Picture snake
    lc.setLed(0,head->row,head->col,true);			// enciende LED
    //Picture apple
    lc.setLed(0,ap->row,ap->col,true);			// enciende LED
    delay(300);

  }
  
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
  return;
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
}


void dequeue(void) //Is there a more simple way?
{
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
}

void go_right()
{
  le_ri = 1;
}

void go_up()
{
  up_bt = -1;
}

void go_bottom()
{
  up_bt = 1;
}


void eat_apple()
{
  if (ap->row == head->row && ap->col == head->col)
  {
    Serial.println("Collision");
    enqueue(ap->row + up_bt, ap->col + le_ri);
    delay(50);
    ap->row = random(0,8);
    ap->col = random(0,8);
  }
}

/*
void create_initial_snake()
{

  //First node of snake
  node *n = malloc(sizeof(node));
  if (n == NULL)
  {
    Serial.println("Couldn't allocate memory");
    return 1;
  } 
  //Initial position
  n->row = 0;
  n->col = 1;
  n->prev = NULL;
  n->next = NULL;

  //Initial node
  if (head == NULL)
  {
    head = n;
    tail = n;
  }
  enqueue(n->row, n->col-1);
} */

