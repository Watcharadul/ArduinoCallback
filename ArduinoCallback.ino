#define BTN_R 9
#define BTN_D 10
#define BTN_U 11

class ParentController;

struct ListenerController{
  virtual ~ListenerController() = default;
  virtual void onSelect(){};
  virtual void onEvent(){};
};

class ParentController{
  private:
    ListenerController *fnc = nullptr;
  public:
    ParentController callSelect(){
      if(fnc) fnc->onSelect();
    }
    ParentController callEvent(){
      if(fnc) fnc->onEvent();
    }
    ParentController setListener(ListenerController *fnc){ this->fnc = fnc;   }
    ParentController setListener(ListenerController &fnc){ setListener(&fnc); }
};

enum ButtonEnum{
  UP, DOWN, RIGHT, NONE
};

struct ButtonController : ParentController{
  private:
    unsigned long timer = 0;
    ButtonEnum direction = NONE;
  public:
    ButtonController(){
      pinMode(BTN_U,INPUT_PULLUP);
      pinMode(BTN_D,INPUT_PULLUP);
      pinMode(BTN_R,INPUT_PULLUP);
    }
    void onListener(){
      if((millis() - timer) > 150){

        direction = getValue();

        if(direction == RIGHT){
          callSelect();
        }else if(direction != NONE){
          callEvent();
        }

        timer  = millis();
      }
    }
    ButtonEnum getValue(){
      if(digitalRead(BTN_U) == 0){
        return UP;
      }
      if(digitalRead(BTN_D) == 0){
        return DOWN;
      }
      if(digitalRead(BTN_R) == 0){
        return RIGHT;
      }
      return NONE;
    }
    ButtonEnum getDirection(){
      return direction;
    }
};

String fruitList[3] = {"Apple","Coconut","Banana"};

struct FruitController : ListenerController , ButtonController{
  private:
    byte index = 0;
  public:
    FruitController(){
      setListener(this);
      Serial.println("< Please select fruit juice >");
    }
    void onEvent() override{
      ButtonEnum button = getDirection();
      if(button == UP && index >= 0){
        index++;
      }else if(button == DOWN && index <= 2){
        index--;
      }

      Serial.println(getFruit());
    }
    void onSelect() override{
      Serial.print("Selected : ");
      Serial.println(getFruit());
    }
    String getFruit(){
      return fruitList[index];
    }
};

FruitController* fruitCtl;

void setup(){
  Serial.begin(9600);
  fruitCtl = new FruitController();
}

void loop(){
  fruitCtl->onListener();
}
